#pragma once

#include "types/natives.hpp"

namespace creek
{
class NetStream
{
  public:
  virtual bool isClosed() const = 0;
  virtual size_t peek(byte_t *buffer, size_t bufSize) const = 0;
  virtual size_t available() const = 0;

  virtual int readOne() = 0;
  virtual size_t read(byte_t *buffer, size_t bufSize) = 0;

  virtual bool writeOne(byte_t byte) = 0;
  virtual size_t write(const byte_t *buffer, size_t bufSize) = 0;

  virtual ~NetStream() = default;
  NetStream &operator=(const NetStream &) = default;
  NetStream(NetStream &&) = default;
  NetStream &operator=(NetStream &&) = default;
  NetStream(const NetStream &) = default;

  protected:
  NetStream() = default;
};
} // namespace creek
