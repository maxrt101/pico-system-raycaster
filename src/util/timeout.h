#pragma once

#include "picosystem.hpp"

namespace util
{
struct Timeout {
  uint32_t start;
  uint32_t duration;

  Timeout() : start(0), duration(0) {}

  explicit Timeout(uint32_t duration) : start(picosystem::time()), duration(duration) {}

  bool expired() const {
    return picosystem::time() > start + duration;
  }

  void expire() {
    start = 0;
  }

  void restart() {
    start = picosystem::time();
  }

  uint32_t passed() const {
    return picosystem::time() - start;
  }

  uint32_t left() const {
    if (start + duration < picosystem::time()) {
      return 0;
    }

    return start + duration - picosystem::time();
  }
};

}
