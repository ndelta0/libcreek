#pragma once

#include "../internal.hpp"
#include "natives.hpp"
#include <array>
#include <string>
#include <stdexcept>
#include <ostream>

namespace creek { namespace types {
  struct Address {
    Address() = default;

    Address(const Address &) = default;

    Address &operator=(const Address &) = default;

    Address(Address &&) = default;

    Address &operator=(Address &&) = default;

    virtual ~Address() = default;

    virtual EAddressType type() const {
      throw std::runtime_error("Called base Address::type() method. This should not happen.");
    }

    virtual socket_length_t size() const {
      throw std::runtime_error("Called base Address::size() method. This should not happen.");
    }

    virtual native::sockaddr *data() const {
      throw std::runtime_error("Called base Address::data() method. This should not happen.");
    }
  };

  struct InetAddress : public Address {
    virtual std::string ip() const {
      throw std::runtime_error("Called base InetAddress::ip() method. This should not happen.");
    }

    virtual uint16_t port() const {
      throw std::runtime_error("Called base InetAddress::port() method. This should not happen.");
    }
  };

  struct IPv4Address : public InetAddress {
    IPv4Address(const std::string &ip, uint16_t port) // NOLINT(*-identifier-length)
    {
      address.sin_family = static_cast<native::sa_family_t>(EAddressType::IPv4);
      address.sin_port = native::htons(port);
      inet_pton(static_cast<int>(EAddressType::IPv4), ip.c_str(), &address.sin_addr);
    }

    explicit IPv4Address(types::socket_address_ipv4_t addr) : address(addr) {}

    socket_address_ipv4_t address{};

    EAddressType type() const override {
      return EAddressType::IPv4;
    }

    socket_length_t size() const override {
      return sizeof(socket_address_t);
    }

    native::sockaddr *data() const override {
      return (native::sockaddr *) &address; // NOLINT(*-pro-type-cstyle-cast)
    }

    std::string ip() const override {
      std::array<char, INET_ADDRSTRLEN> buffer{};
      inet_ntop(static_cast<int>(EAddressType::IPv4), &address.sin_addr, buffer.data(), INET_ADDRSTRLEN);
      return buffer.data();
    }

    uint16_t port() const override {
      return native::ntohs(address.sin_port);
    }

    static IPv4Address any(uint16_t port) {
      return {"0.0.0.0", port};
    }

    static IPv4Address loopback(uint16_t port) {
      return {"127.0.0.1", port};
    }
  };

  struct IPv6Address : public InetAddress {
    IPv6Address(const std::string &ip, uint16_t port) // NOLINT(*-identifier-length)
    {
      address.sin6_family = static_cast<native::sa_family_t>(EAddressType::IPv6);
      address.sin6_port = native::htons(port);
      inet_pton(static_cast<int>(EAddressType::IPv6), ip.c_str(), &address.sin6_addr);
    }

    explicit IPv6Address(types::socket_address_ipv6_t addr) : address(addr) {}

    socket_address_ipv6_t address{};

    EAddressType type() const override {
      return EAddressType::IPv6;
    }

    socket_length_t size() const override {
      return sizeof(socket_address_t);
    }

    native::sockaddr *data() const override {
      return (native::sockaddr *) &address; // NOLINT(*-pro-type-cstyle-cast)
    }

    std::string ip() const override {
      std::array<char, INET6_ADDRSTRLEN> buffer{};
      inet_ntop(static_cast<int>(EAddressType::IPv6), &address.sin6_addr, buffer.data(), INET6_ADDRSTRLEN);
      return buffer.data();
    }

    uint16_t port() const override {
      return native::ntohs(address.sin6_port);
    }

    static IPv6Address any(uint16_t port) {
      return {"::", port};
    }

    static IPv6Address loopback(uint16_t port) {
      return {"::1", port};
    }
  };
}}