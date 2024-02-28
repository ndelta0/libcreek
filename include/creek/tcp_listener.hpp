#pragma once

#include "tcp_stream.hpp"
#include "types/address.hpp"

namespace creek
{
class TcpListener
{
  private:
  TcpListener() {}

  public:
  TcpListener(const TcpListener &) = delete;
  TcpListener &operator=(const TcpListener &) = delete;
  TcpListener(TcpListener &&) = default;
  TcpListener &operator=(TcpListener &&) = delete;
  ~TcpListener();

  /**
   * Creates a new TcpListener and binds it to the specified address
   * @param address The address to bind to
   * @return The listener
   * @throw std::runtime_error if the listener could not be created
   */
  static TcpListener bind(const types::InetAddress &address);

  /**
   * Accepts a new connection
   * @remark This method blocks until a new connection is available
   * @return The new connection
   * @throw std::runtime_error if the connection could not be accepted
   */
  TcpStream accept() const;

  /**
   * Checks if the listener can accept a new connection
   * @note This is useful to avoid blocking on accept
   * @return true if the listener can accept a new connection, false otherwise
   */
  bool canAccept() const;

  /**
   * Closes the listener
   */
  void close();

  private:
  types::socket_t m_socket = INVALID_SOCKET;
  int m_epoll_fd = -1;
};
} // namespace creek
