#pragma once

#include "picosystem.hpp"
#include "util/util.h"

enum class SpritesheetKind {
  NONE,
  WOLF3D
};

struct Resource {};

struct Spritesheet : Resource {
  picosystem::buffer_t buffer;
  int32_t size;
  int32_t element_size;

  Spritesheet() {
    buffer.alloc = false;
    buffer.data = nullptr;
    buffer.h = 0;
    buffer.w = 0;
    size = 0;
    element_size = 0;
  }

  Spritesheet(uint8_t * data, int32_t size, int32_t element_size) : size(size), element_size(element_size) {
    util::buffer_init(&buffer, size * element_size, size * element_size, (picosystem::color_t *) data);
  }

  void select(picosystem::buffer_t * buf, int32_t idx) {
    using namespace picosystem;

    int x = idx % size, y = idx / size;

    target(buf);
    blit(&buffer, x * element_size, y * element_size, element_size, element_size, 0, 0);
    target();
  }

  static Spritesheet * get(SpritesheetKind kind) {
    return &spritesheets[kind];
  }

  static std::unordered_map<SpritesheetKind, Spritesheet> spritesheets;
};

