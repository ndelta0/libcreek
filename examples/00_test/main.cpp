#include <creek/tcp_listener.hpp>

#include <iostream>
#include <vector>
#include <thread>

const uint16_t C_DEFAULT_PORT = 6969;

using namespace creek;

auto main() -> int {
  TcpListener listener = TcpListener::bind(types::IPv4Address::any(C_DEFAULT_PORT));

  auto client = listener.accept();

  std::cout << "Accepted connection from " << client.remoteEndpoint()->ip() << ":"
            << client.remoteEndpoint()->port() << "\n";

//  while (!client.isClosed()) {
//    auto available = client.numBytesAvailable();
//    if (available > 0) {
//      std::vector<uint8_t> buffer(available);
//      auto read = client.read(buffer.data(), buffer.size());
//      std::cout << "Received " << read << " bytes: ";
//      for (size_t i = 0; i < read; i++) {
//        if (buffer[i] == '\n') {
//          std::cout << "\\n";
//        } else if (buffer[i] == '\r') {
//          std::cout << "\\r";
//        } else {
//          std::cout << buffer[i];
//        }
//      }
//      std::cout << "\n";
//
//      const std::string message = "Hello from server!\nYou sent: ";
//      client.write(reinterpret_cast<const uint8_t *>(message.data()), message.size()); // NOLINT(*-pro-type-reinterpret-cast)
//      client.write(buffer.data(), read);
//    }
//
//    std::this_thread::sleep_for(std::chrono::milliseconds(100));
//  }

//  size_t size = 0;
//  while (true) {
//    std::cout << "Would " << size << " bytes block? ";
//    if (client.writeWouldBlock(size)) {
//      std::cout << "Yes\n";
//      break;
//    }
//    std::cout << "No\n";
//    size++;
//  }

  return 0;
}