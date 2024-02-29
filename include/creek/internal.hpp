#pragma once

#include "types/address.hpp"
#include "types/natives.hpp"
#include <cstddef>
#include <cstring>
#include <memory>
#include <string>
#include <vector>

namespace creek
{
namespace internal
{
template <typename TSrc, typename TDest>
TDest bitCast(const TSrc &src)
{
  const size_t srcSize = sizeof(TSrc);
  const size_t destSize = sizeof(TDest);
  static_assert(destSize >= srcSize,
                "Size of destination type must be greater than or equal to the size of the source type");
  TDest dest;
  if (destSize > srcSize)
  {
    auto toFill = destSize - srcSize;
    std::memset(&dest + srcSize, 0, toFill);
  }
  std::memcpy(&dest, &src, sizeof(TSrc));
  return dest;
}

template <typename TSrc, typename TDest>
TDest bitCastNum(const TSrc &src, size_t count)
{
  const size_t srcSize = sizeof(TSrc);
  const size_t destSize = sizeof(TDest);
  TDest dest;
  if (destSize > count)
  {
    auto toFill = destSize - count;
    std::memset(&dest + srcSize, 0, toFill);
  }
  std::memcpy(&dest, &src, count);
  return dest;
}

std::string lastError();

std::vector<std::shared_ptr<types::InetAddress>> resolve(const std::string &host,
                                                         const std::string &service,
                                                         types::EAddressType inetType,
                                                         types::ESocketType sockType);
} // namespace internal
} // namespace creek
