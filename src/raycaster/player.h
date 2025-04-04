#pragma once

#include "util/util.h"

struct Player {
  Vec2<double> position       = {0, 0};
  Vec2<double> velocity       = {0, 0};
  float        angle          = 0.0f;
  float        rotation_speed = 0.2f;
  float        movement_speed = 0.2f;
};
