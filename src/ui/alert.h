#pragma once

#include "picosystem.hpp"
#include "ui/view.h"
#include "util/util.h"

namespace ui {

struct Alert : View {
  enum Flags {
    NONE    = 0,
    CENTER  = (1 << 0),
    FADEIN  = (1 << 1),
    FADEOUT = (1 << 2),
  };

  std::string text;
  struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } color;
  uint32_t    time;
  uint32_t    fade_time;
  uint8_t     flags;

  uint8_t alpha;
  util::Timeout timeout;

  Alert() {}

  Alert(const std::string& text, uint8_t r, uint8_t g, uint8_t b, uint32_t time, uint32_t fade_time, uint8_t flags)
    : text(text), color({r, g, b}), time(time), fade_time(fade_time), flags(flags) {}

  ~Alert() override = default;

  void init() {
    alpha = 0xF;
    timeout = util::Timeout(time);
  }

  void update(uint32_t tick) {
    using namespace util;

    if (!timeout.expired()) {
      if ((flags & FADEIN) && timeout.passed() < fade_time) {
        alpha = scale<float>(
          (float) timeout.passed(),
          {0, (float) fade_time},
          {0, 0xF});
      } else {
        alpha = 0xF;
      }

      if ((flags & FADEOUT) && timeout.left() < fade_time) {
        alpha = scale<float>(
          (float) timeout.left(),
          {0, (float) fade_time},
          {0, 0xF});
      }
    } else {
      activated = false;
    }
  }

  void draw(uint32_t tick) {
    using namespace picosystem;

    if (!timeout.expired()) {
      pen(color.r, color.g, color.b, alpha);

      if (flags & CENTER) {
        int32_t w, h;
        measure(text, w, h);
        picosystem::text(text, (SCREEN->w - w) / 2, (SCREEN->h - h) / 2);
      } else {
        picosystem::text(text, 0, 0);
      }
    }
  }
};

}

