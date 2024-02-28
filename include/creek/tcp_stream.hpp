#pragma once

#include "types/address.hpp"

#include <memory>

namespace creek {
  class TcpStream {
  private:
    TcpStream(types::socket_t socket, std::shared_ptr<creek::types::InetAddress> remoteEndpoint);

  public:
    TcpStream &operator=(const TcpStream &) = delete;

    TcpStream(TcpStream &&) = default;

    TcpStream &operator=(TcpStream &&) = delete;

    TcpStream(const TcpStream &) = delete;

    ~TcpStream();

    [[nodiscard]] auto remoteEndpoint() const -> std::shared_ptr<creek::types::InetAddress> {
      return m_remote_endpoint;
    }

    bool isClosed() const;

    size_t peek(uint8_t *buffer, size_t bufSize) const;

    size_t numBytesAvailable() const;

    size_t read(uint8_t *buffer, size_t bufSize) const;

    size_t write(const uint8_t *buffer, size_t bufSize) const;

    bool writeWouldBlock(size_t bufSize) const;

  private:
    types::socket_t m_socket;
    std::shared_ptr<creek::types::InetAddress> m_remote_endpoint;

    friend class TcpListener;
  };
}