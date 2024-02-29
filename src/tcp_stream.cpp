#include "tcp_stream.hpp"
#include "internal.hpp"
#include "types/address.hpp"
#include "types/natives.hpp"

#include <memory>
#include <sys/socket.h>
#include <utility>

using namespace creek;
using namespace creek::types;

TcpStream::TcpStream(socket_t socket, std::shared_ptr<InetAddress> remoteEndpoint)
  : m_socket(socket), m_remote_endpoint(std::move(remoteEndpoint))
{
}

TcpStream::~TcpStream()
{
  native::shutdown(m_socket, native::SHUT_RDWR);
  native::close(m_socket);
}

TcpStream TcpStream::connect(const std::shared_ptr<creek::types::InetAddress> &endpoint)
{
  socket_t socket = native::socket(static_cast<int>(endpoint->type()), native::SOCK_STREAM, 0);
  if (socket == INVALID_SOCKET)
  {
    throw std::runtime_error("Failed to create socket: " + internal::lastError());
  }

  TcpStream stream{socket, endpoint};

  if (native::connect(stream.m_socket, endpoint->data(), endpoint->size()) == -1)
  {
    throw std::runtime_error("Failed to connect to " + endpoint->ip() + ":" + std::to_string(endpoint->port()) + ": " +
                             internal::lastError());
  }

  return stream;
}

TcpStream TcpStream::connect(const std::string &host, uint16_t port)
{
  std::string service = std::to_string(port);

  auto resolved = internal::resolve(host, service, EAddressType::Unknown, ESocketType::TCP);

  if (resolved.empty())
  {
    throw std::runtime_error("No addresses were resolved for '" + host + "'");
  }

  for (const auto &addr : resolved)
  {
    try
    {
      auto stream = TcpStream::connect(addr);
      return stream;
    }
    catch (std::exception) // NOLINT(bugprone-empty-catch)
    {
      // ignored
    }
  }

  throw std::runtime_error("Could not connect to any of resolved addresses for '" + host + "'");
}

bool TcpStream::isClosed() const
{
  int error = 0;
  socket_length_t len = sizeof(error);
  auto retval = native::getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error, &len);

  return retval != 0 || error != 0;
}

size_t TcpStream::peek(byte_t *buffer, size_t bufSize) const
{
  return native::recv(m_socket, buffer, bufSize, native::MSG_PEEK);
}

size_t TcpStream::available() const
{
  int available = 0;
  if (native::ioctl(m_socket, FIONREAD, &available) == -1) // NOLINT(*-pro-type-vararg)
  {
    return 0;
  }
  return available;
}

int TcpStream::readOne()
{
  uint8_t val = 0;
  if (read(&val, 1) == 1)
  {
    return val;
  }
  return -1;
}

size_t TcpStream::read(byte_t *buffer, size_t bufSize)
{
  return native::recv(m_socket, buffer, bufSize, 0);
}

bool TcpStream::writeOne(byte_t byte)
{
  return write(&byte, 1) == 1;
}

size_t TcpStream::write(const byte_t *buffer, size_t bufSize)
{
  return native::send(m_socket, buffer, bufSize, 0);
}

// bool TcpStream::writeWouldBlock(size_t bufSize) const
// {
//   return native::send(m_socket, nullptr, 0, native::MSG_DONTWAIT) == -1 && errno == EWOULDBLOCK;
// }
