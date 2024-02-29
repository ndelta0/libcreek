#include "tcp_listener.hpp"
#include "internal.hpp"
#include <memory>
#include <stdexcept>

using namespace creek;
TcpListener::~TcpListener()
{
  close();
  native::close(m_epoll_fd);
}

bool setopt(types::socket_t socket, int level, int optname, const void *optval, types::socket_length_t optlen)
{
  return native::setsockopt(socket, level, optname, static_cast<const char *>(optval), optlen) == 0;
}

TcpListener TcpListener::bind(const types::InetAddress &address)
{
  TcpListener listener;
  listener.m_socket = native::socket(static_cast<int>(address.type()), native::SOCK_STREAM, 0);
  if (listener.m_socket == INVALID_SOCKET)
  {
    throw std::runtime_error("Failed to create socket: " + internal::lastError());
  }

  int optval = 1;
  if (!setopt(listener.m_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)))
  {
    throw std::runtime_error("Failed to set REUSEADDR socket option: " + internal::lastError());
  }
  if (!setopt(listener.m_socket, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(int)))
  {
    throw std::runtime_error("Failed to set REUSEPORT socket option: " + internal::lastError());
  }

  if (address.type() == types::EAddressType::IPv4)
  {
    if (native::bind(listener.m_socket, address.data(), sizeof(types::socket_address_ipv4_t)) != 0)
    {
      throw std::runtime_error("Failed to bind socket: " + internal::lastError());
    }
  }
  else if (address.type() == types::EAddressType::IPv6)
  {
    if (native::bind(listener.m_socket, address.data(), sizeof(types::socket_address_ipv6_t)) != 0)
    {
      throw std::runtime_error("Failed to bind socket: " + internal::lastError());
    }
  }
  else
  {
    throw std::runtime_error("Unsupported address type");
  }

  if (native::listen(listener.m_socket, SOMAXCONN) != 0)
  {
    throw std::runtime_error("Failed to listen on socket: " + internal::lastError());
  }

  listener.m_epoll_fd = native::epoll_create1(0);
  if (listener.m_epoll_fd == -1)
  {
    throw std::runtime_error("Failed to create epoll instance: " + internal::lastError());
  }
  native::epoll_event event{};
  event.events = native::EPOLLIN;
  event.data.fd = listener.m_socket;
  if (native::epoll_ctl(listener.m_epoll_fd, EPOLL_CTL_ADD, listener.m_socket, &event) == -1)
  {
    throw std::runtime_error("Failed to add socket to epoll instance: " + internal::lastError());
  }

  return listener;
}

bool TcpListener::canAccept() const
{
  native::epoll_event event{};
  int ret = native::epoll_wait(m_epoll_fd, &event, 1, 0);
  if (ret == -1)
  {
    throw std::runtime_error("Failed to wait for events: " + internal::lastError());
  }
  return ret > 0;
}

TcpStream TcpListener::accept() const
{
  native::sockaddr_storage addr{};
  types::socket_length_t addrlen = sizeof(native::sockaddr_storage);
  types::socket_t clientSock = native::accept(
    m_socket,
    reinterpret_cast<native::sockaddr *>(&addr), // NOLINT(*-pro-type-reinterpret-cast) must be used to write to addr
    &addrlen);
  if (clientSock == INVALID_SOCKET)
  {
    throw std::runtime_error("Failed to accept connection: " + internal::lastError());
  }

  std::shared_ptr<types::InetAddress> address;
  if (addr.ss_family == static_cast<int>(types::EAddressType::IPv4))
  {
    const types::socket_address_ipv4_t &ipv4Native =
      internal::bitCastNum<types::socket_address_t, types::socket_address_ipv4_t>(addr, addrlen);
    address = std::make_shared<types::IPv4Address>(ipv4Native);
  }
  else if (addr.ss_family == static_cast<int>(types::EAddressType::IPv6))
  {
    const types::socket_address_ipv6_t &ipv6Native =
      internal::bitCastNum<types::socket_address_t, types::socket_address_ipv6_t>(addr, addrlen);
    address = std::make_shared<types::IPv6Address>(ipv6Native);
  }
  else
  {
    throw std::runtime_error("Unsupported address type: " + std::to_string(addr.ss_family));
  }

  return {clientSock, address};
}

void TcpListener::close()
{
  native::shutdown(m_socket, native::SHUT_RDWR);
  m_socket = INVALID_SOCKET;
}
