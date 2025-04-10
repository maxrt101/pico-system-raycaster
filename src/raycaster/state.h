#pragma once

#include "raycaster/map.h"
#include "raycaster/player.h"
#include "raycaster/option.h"
#include "raycaster/resources.h"

#define OPT_MOVE_SPEED_SCALE 100.0f
#define OPT_ROT_SPEED_SCALE  100.0f

struct State {
  static Option          options[Option::OPTIONS_MAX];
  static Player          player;
  static Map             map;
  static SpritesheetKind spritesheet;

  static bool getOptBool(uint8_t opt) {
    return options[opt].value.boolean;
  }

  static int32_t getOptRangeMin(uint8_t opt) {
    return options[opt].value.range.from;
  }

  static int32_t getOptRangeMax(uint8_t opt) {
    return options[opt].value.range.to;
  }

  static int32_t getOptRangeVal(uint8_t opt) {
    return options[opt].value.range.value;
  }

  static void init() {
    options[Option::SHOW_DEBUG]   = {false};
    options[Option::SHOW_FPS]     = {false};
    options[Option::SHOW_BATTERY] = {false};
    options[Option::BACKLIGHT]    = {2, 100, 75};
    options[Option::MOVE_SPEED]   = {1, 100, (int32_t) (0.2 * OPT_MOVE_SPEED_SCALE)};
    options[Option::ROT_SPEED]    = {1, 100, (int32_t) (0.2 * OPT_ROT_SPEED_SCALE)};
  }
};
