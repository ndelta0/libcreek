#include "tcp_stream.hpp"

#include <utility>

using namespace creek;
using namespace creek::types;

TcpStream::TcpStream(socket_t socket, std::shared_ptr<InetAddress> remoteEndpoint)
    : m_socket(socket), m_remote_endpoint(std::move(remoteEndpoint)) {
}

TcpStream::~TcpStream() {
  native::shutdown(m_socket, native::SHUT_RDWR);
  native::close(m_socket);
}

bool TcpStream::isClosed() const {
  int error = 0;
  socket_length_t len = sizeof(error);
  auto retval = native::getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error, &len);

  return retval != 0 || error != 0;
}

size_t TcpStream::peek(uint8_t *buffer, size_t bufSize) const {
  return native::recv(m_socket, buffer, bufSize, native::MSG_PEEK);
}

size_t TcpStream::numBytesAvailable() const {
  int available = 0;
  if (native::ioctl(m_socket, FIONREAD, &available) == -1) { // NOLINT(*-pro-type-vararg)
    return 0;
  }
  return available;
}

size_t TcpStream::read(uint8_t *buffer, size_t bufSize) const {
  return native::recv(m_socket, buffer, bufSize, 0);
}

size_t TcpStream::write(const uint8_t *buffer, size_t bufSize) const {
  return native::send(m_socket, buffer, bufSize, 0);
}

bool TcpStream::writeWouldBlock(size_t bufSize) const {
  return native::send(m_socket, nullptr, 0, native::MSG_DONTWAIT) == -1 && errno == EWOULDBLOCK;
}



