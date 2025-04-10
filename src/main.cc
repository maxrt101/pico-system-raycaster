#include "picosystem.hpp"
#include "util/util.h"
#include "raycaster/raycaster.h"
#include "raycaster/resources.h"
#include "raycaster/state.h"

std::unordered_map<Event, std::vector<EventSubscriber>> events::subscribers;
// Debug
std::string events::last_published;

Option            State::options[Option::OPTIONS_MAX];
Player            State::player;
Map               State::map;
Spritesheet::Kind State::spritesheet = DEFAULT_SPRITESHEET;

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
