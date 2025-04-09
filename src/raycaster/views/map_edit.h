#pragma once

#include "picosystem.hpp"
#include "raycaster/state.h"
#include "util/util.h"

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

    if (pressed(B)) {
      State::map.get(pos.x, pos.y).type = State::map.get(pos.x, pos.y).type == TILE_WALL ? TILE_EMPTY : TILE_WALL;
    }
  }

  void draw(uint32_t tick) {
    using namespace picosystem;

    int32_t scale = util::SCREEN_SIZE / State::map.size();

    for (int32_t y = 0; y < State::map.size(); ++y) {
      for (int32_t x = 0; x < State::map.size(); ++x) {
        if (State::map.get(x, y).type == TILE_WALL) {
          pen(0xF, 0xF, 0xF);
          frect(x * scale, y * scale, scale, scale);
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
