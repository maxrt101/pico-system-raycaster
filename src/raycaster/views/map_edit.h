#pragma once

#include "picosystem.hpp"
#include "raycaster/state.h"
#include "util/util.h"

#define USE_BISTABLE_MAP_TILE 0

namespace views {

struct MapEdit : ui::View {
  Vec2<int32_t> pos;

  MapEdit() {}
  ~MapEdit() override = default;

  void init() {
    pos = {0, 0};
    ignore_inputs_for_next_frame = true;
  }

  void update(uint32_t tick) {
    using namespace picosystem;
    using namespace util;

    if (pressed(A)) {
      events::publish("map_edit_exit");
    }

    if (pressed(UP)) {
      pos.y = cap<int32_t>(pos.y - 1, 0, State::map.size() - 1);
    }

    if (pressed(DOWN)) {
      pos.y = cap<int32_t>(pos.y + 1, 0, State::map.size() - 1);
    }

    if (pressed(LEFT)) {
      pos.x = cap<int32_t>(pos.x - 1, 0, State::map.size() - 1);
    }

    if (pressed(RIGHT)) {
      pos.x = cap<int32_t>(pos.x + 1, 0, State::map.size() - 1);
    }

#if USE_BISTABLE_MAP_TILE
    if (pressed(B)) {
      State::map.get(pos.x, pos.y).type = State::map.get(pos.x, pos.y).type == TILE_WALL ? TILE_EMPTY : TILE_WALL;
    }
#else
    if (pressed(B)) {
      State::map.get(pos.x, pos.y).type++;
    }

    if (pressed(Y)) {
      State::map.get(pos.x, pos.y).type--;
    }
    auto spritesheet = Spritesheet::get(State::spritesheet);

    if (State::map.get(pos.x, pos.y).type < 0) {
      State::map.get(pos.x, pos.y).type = spritesheet->size * spritesheet->size - 1;
    }

    if (State::map.get(pos.x, pos.y).type >= spritesheet->size * spritesheet->size) {
      State::map.get(pos.x, pos.y).type = 0;
    }
#endif
  }

  void draw(uint32_t tick) {
    using namespace picosystem;
    using namespace util;

    int32_t scale = util::SCREEN_SIZE / State::map.size();

    auto screen_height = SCREEN->h;
    auto screen_width = SCREEN->w;

    auto spritesheet = Spritesheet::get(State::spritesheet);

    color_t texture_data[spritesheet->element_size * spritesheet->element_size];
    buffer_t texture;

    buffer_init(&texture, spritesheet->element_size, spritesheet->element_size, texture_data);

    for (int32_t y = 0; y < State::map.size(); ++y) {
      for (int32_t x = 0; x < State::map.size(); ++x) {
        if (State::map.get(x, y).isSolid()) {
#if USE_BISTABLE_MAP_TILE
          pen(0xF, 0xF, 0xF);
          frect(x * scale, y * scale, scale, scale);
#else
          spritesheet->select(&texture, State::map.get(x, y).type - 1);

          blit(&texture,
            0, 0,
            spritesheet->element_size, spritesheet->element_size,
            x * scale, y * scale,
            scale, scale
          );
#endif
        }
        if (x == pos.x && y == pos.y) {
          pen(0x8, 0xF, 0x8);
          rect(x * scale, y * scale, scale, scale);
        }
      }
    }
  }
};

}
