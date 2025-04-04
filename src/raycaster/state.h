#pragma once

#include "raycaster/map.h"
#include "raycaster/player.h"
#include "raycaster/option.h"

struct State {
  static Option  options[Option::OPTIONS_MAX];
  static Player  player;
  static Map     map;

  static void init() {
    options[Option::SHOW_DEBUG] = {false};
    options[Option::SHOW_FPS] = {false};
    options[Option::SHOW_BATTERY] = {false};
    options[Option::BACKLIGHT] = {0, 100, 75};
  }
};
