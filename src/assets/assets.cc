#include "raycaster/resources.h"
#include "assets/wolf3d_walls_spritesheet_16x16.h"

std::unordered_map<Spritesheet::Kind, Spritesheet> Spritesheet::spritesheets = {
  {Kind::WOLF3D, Spritesheet(wolf3d_spritesheet_16x16, 6, 16)}
};
