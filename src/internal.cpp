#include "internal.hpp"

std::string creek::internal::lastError() {
#ifdef _WIN32
#error "creek::internal::lastError() -> Not implemented"
#elif defined(__linux__)
  return {strerror(errno)};
#else
#error "creek::internal::lastError() -> Unsupported platform"
#endif
}
