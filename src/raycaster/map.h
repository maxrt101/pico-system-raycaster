#pragma once

#include "util/util.h"
#include <cstdint>
#include <vector>

#define TILE_EMPTY              0
#define TILE_WALL               1

#define USE_SAMPLE_MAP 1

struct Map {
  struct Tile {
    uint8_t type;

    Tile(uint8_t type) : type(type) {}
  };

  int32_t side_size = 0;
  std::vector<Tile> tiles;

  Map() {
#if USE_SAMPLE_MAP
    side_size = 12;
    tiles = {
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1,
      1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
      1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    };
#endif
  }

  Map(int32_t size) : side_size(size) {
    tiles.reserve(size * size);
  }

//  Map(Tile * tiles) {
//    memcpy(this->tiles, tiles, sizeof(Tile) * N * N);
//  }

  int32_t size() const {
    return side_size;
  }

  Tile& get(int32_t index) {
    return tiles[index];
  }

  Tile& get(int32_t x, int32_t y) {
    return tiles[y * side_size + x];
  }

  Tile& get(Vec2<int> pos) {
    return get(pos.x, pos.y);
  }
};
