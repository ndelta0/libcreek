#include "internal.hpp"
#include "types/address.hpp"
#include "types/natives.hpp"
#include <cstring>
#include <memory>
#include <netdb.h>
#include <sys/socket.h>

using namespace creek::types;

namespace creek
{
namespace internal
{
std::string lastError()
{
#ifdef _WIN32
#error "creek::internal::lastError() -> Not implemented"
#elif defined(__linux__)
  return {strerror(errno)};
#else
#error "creek::internal::lastError() -> Unsupported platform"
#endif
}

std::vector<std::shared_ptr<InetAddress>> resolve(const std::string &host,
                                                  const std::string &service,
                                                  EAddressType inetType,
                                                  ESocketType sockType)
{
  std::vector<std::shared_ptr<InetAddress>> resolved;

#ifdef _WIN32
#error                                                                                                                 \
  "creek::internal::resolve(const std::string &, const std::string &, creek::types::EAddressType, creek::types::ESocketType) -> Not implemented"
#elif defined(__linux__)
  struct native::addrinfo hints = {};
  struct native::addrinfo *servInfo = nullptr;

  hints.ai_family = (int)inetType;
  hints.ai_socktype = (int)sockType;
  hints.ai_flags |= AI_ADDRCONFIG | AI_IDN;

  if (native::getaddrinfo(host.c_str(), service.c_str(), &hints, &servInfo) != 0)
  {
    return resolved;
  }

  struct native::addrinfo *p = nullptr; // NOLINT(readability-identifier-length)
  for (p = servInfo; p != nullptr; p = p->ai_next)
  {
    switch (p->ai_family)
    {
    case (int)types::EAddressType::IPv4:
    {
      types::socket_address_ipv4_t local{};
      auto *ipv4 = reinterpret_cast<types::socket_address_ipv4_t *>(p->ai_addr);
      std::memcpy(&local, ipv4, sizeof(types::socket_address_ipv4_t));
      resolved.push_back(std::make_shared<types::IPv4Address>(local));
    }
    break;
    case (int)EAddressType::IPv6:
    {
      types::socket_address_ipv6_t local{};
      auto *ipv6 = reinterpret_cast<types::socket_address_ipv6_t *>(p->ai_addr);
      std::memcpy(&local, ipv6, sizeof(types::socket_address_ipv6_t));
      resolved.push_back(std::make_shared<types::IPv6Address>(local));
    }
    break;
    default:
      break;
    }
  }

  native::freeaddrinfo(servInfo);
#else
#error                                                                                                                 \
  "creek::internal::resolve(const std::string &, const std::string &, creek::types::EAddressType, creek::types::ESocketType) -> Unsupported platform"
#endif

  return resolved;
}
} // namespace internal
} // namespace creek
