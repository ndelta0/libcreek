#pragma once

#include <cstddef>
#include <cstdint>
#include <pthread.h>

namespace creek
{
using byte_t = uint8_t;

namespace native
{
#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WS2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")
#elif __linux__
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#else
#error "TODO: Platform not implemented"
#endif
} // namespace native

namespace types
{
#ifdef _WIN32
using socket_t = SOCKET;

using socket_address_t = sockaddr;
using socket_address_ipv4_t = sockaddr_in;
using socket_length_t = int;

enum class EAddressType : short // NOLINT(*-enum-size)
{
  IPv4 = AF_INET,
  IPv6 = AF_INET6,
};
#elif __linux__
using socket_t = int;
#define INVALID_SOCKET (-1)

using socket_address_t = creek::native::sockaddr_storage;
using socket_address_ipv4_t = creek::native::sockaddr_in;
using socket_address_ipv6_t = creek::native::sockaddr_in6;
using socket_length_t = creek::native::socklen_t;

enum class EAddressType : creek::native::sa_family_t // NOLINT(*-enum-size)
{
  Unknown = AF_UNSPEC,
  Unix = AF_UNIX,
  IPv4 = AF_INET,
  IPv6 = AF_INET6,
};

enum class ESocketType
{
  TCP = native::SOCK_STREAM,
  UDP = native::SOCK_DGRAM,
};
#else
#error "TODO: Platform not implemented"
#endif
} // namespace types
} // namespace creek
