#include "picosystem.hpp"
#include "util/util.h"
#include "raycaster/raycaster.h"
#include "raycaster/resources.h"
#include "raycaster/state.h"

#include "assets/wolf3d_walls_spritesheet_16x16.h"

std::unordered_map<Event, std::vector<EventSubscriber>> events::subscribers;
// Debug
std::string events::last_published;

Option          State::options[Option::OPTIONS_MAX];
Player          State::player;
Map             State::map;
SpritesheetKind State::spritesheet = SpritesheetKind::WOLF3D;

std::unordered_map<SpritesheetKind, Spritesheet> Spritesheet::spritesheets = {
  {SpritesheetKind::WOLF3D, Spritesheet(wolf3d_spritesheet_16x16, 6, 16)}
};

static Raycaster raycaster;

void init() {
  State::init();
  raycaster.init();
}

void update(uint32_t tick) {
  raycaster.update(tick);
}

void draw(uint32_t tick) {
  raycaster.draw(tick);
}
