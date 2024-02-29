#include <creek/tcp_stream.hpp>

using namespace creek::internal;
using namespace creek::types;
using namespace creek;

#include <iostream>

auto main() -> int
{
  auto client = TcpStream::connect("k4rasudev.me", 80);

  std::cout << "Connected to " << client.remoteEndpoint()->ip() << "\n";

  return 0;
}
