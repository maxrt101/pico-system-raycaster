#pragma once

#include "picosystem.hpp"
#include "util/math.h"
#include "util/vec2.h"
#include "util/bitmap.h"
#include "util/events.h"
#include "util/timeout.h"

namespace util {

#ifdef PIXEL_DOUBLE
constexpr int32_t SCREEN_SIZE = 120;
#else
constexpr int32_t SCREEN_SIZE = 240;
#endif

inline void buffer_init(picosystem::buffer_t * buffer, int32_t w, int32_t h, picosystem::color_t * data) {
  if (!buffer || !data) {
    return;
  }

  buffer->w = w;
  buffer->h = h;
  buffer->data = data;
  buffer->alloc = false;
}

}
