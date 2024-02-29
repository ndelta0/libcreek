#pragma once

#include "net_stream.hpp"
#include "types/address.hpp"

#include <memory>

namespace creek
{
class TcpStream : public NetStream
{
  private:
  TcpStream(types::socket_t socket, std::shared_ptr<creek::types::InetAddress> remoteEndpoint);

  public:
  TcpStream &operator=(const TcpStream &) = delete;

  TcpStream(TcpStream &&) = default;

  TcpStream &operator=(TcpStream &&) = delete;

  TcpStream(const TcpStream &) = delete;

  ~TcpStream() override;

  static TcpStream connect(const std::shared_ptr<creek::types::InetAddress> &endpoint);
  static TcpStream connect(const std::string &host, uint16_t port);

  [[nodiscard]] auto remoteEndpoint() const -> std::shared_ptr<creek::types::InetAddress>
  {
    return m_remote_endpoint;
  }

  bool isClosed() const override;

  size_t peek(byte_t *buffer, size_t bufSize) const override;

  size_t available() const override;

  int readOne() override;

  size_t read(byte_t *buffer, size_t bufSize) override;

  // template <typename T>
  // size_t readItems(T *buffer, size_t bufSize) const
  // {
  //   static_assert(std::is_pod<T>::value, "T must be easily castable to bytes");
  //   size_t offset = 0;
  //   const size_t tSize = sizeof(T);
  //   while (offset < bufSize &&
  //          read(reinterpret_cast<uint8_t *>(buffer + offset), tSize) == tSize) // NOLINT(*-pro-type-reinterpret-cast)
  //   {
  //     offset++;
  //   }
  //   return offset;
  // }

  // template <typename T>
  // size_t readUntil(std::vector<T> &buffer, size_t num) const
  // {
  //   static_assert(std::is_pod<T>::value, "T must be easily castable to bytes");
  //   size_t numRead = 0;
  //   while (true)
  //   {
  //     T item;
  //     auto bytesRead = read(reinterpret_cast<uint8_t *>(&item), sizeof(T)); // NOLINT(*-pro-type-reinterpret-cast)
  //     if (bytesRead == sizeof(T))
  //     {
  //       buffer.push_back(item);
  //       numRead++;
  //     }
  //     else
  //     {
  //       break;
  //     }
  //   }
  //   return numRead;
  // }

  bool writeOne(byte_t byte) override;
  size_t write(const byte_t *buffer, size_t bufSize) override;

  // bool writeWouldBlock(size_t bufSize) const;

  private:
  types::socket_t m_socket;
  std::shared_ptr<creek::types::InetAddress> m_remote_endpoint;

  friend class TcpListener;
};
} // namespace creek
