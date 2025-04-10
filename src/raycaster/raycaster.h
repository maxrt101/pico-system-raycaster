#pragma once

#include "util/util.h"
#include "raycaster/views/main_menu.h"
#include "raycaster/views/options.h"
#include "raycaster/views/renderer.h"
#include "raycaster/views/map_edit.h"

struct Raycaster : ui::View {
  ui::ViewStack stack;

  struct {
    views::MainMenu main_menu;
    views::Options  options;
    views::Renderer renderer;
    views::MapEdit  map_edit;
  } views;

  int32_t last_backlight;
  void * map_edit_enter_payload = nullptr;

  Raycaster() {
    events::subscribe("options_enter", [this](void * p) {
      this->views.main_menu.activated = false;
      this->views.options.activated = true;
      this->views.options.init();
    });

    events::subscribe("options_exit", [this](void * p) {
      this->views.main_menu.activated = true;
      this->views.options.activated = false;
      this->views.main_menu.init();
    });

    events::subscribe("map_edit_enter", [this](void * p) {
      map_edit_enter_payload = p;
      this->views.main_menu.activated = false;
      this->views.renderer.activated = false;
      this->views.map_edit.activated = true;
      this->views.map_edit.init();
    });

    events::subscribe("map_edit_exit", [this](void * p) {
      this->views.map_edit.activated = false;

      if (map_edit_enter_payload == &views.renderer) {
        this->views.renderer.activated = true;
        this->views.renderer.init();
      } else {
        this->views.main_menu.activated = true;
        this->views.main_menu.init();
      }
    });

    events::subscribe("renderer_enter", [this](void * p) {
      this->views.main_menu.activated = false;
      this->views.renderer.activated = true;
      this->views.renderer.init();
    });

    events::subscribe("renderer_exit", [this](void * p) {
      this->views.main_menu.activated = true;
      this->views.renderer.activated = false;
      this->views.main_menu.init();
    });

    events::subscribe("reset_to_dfu", [this](void * p) {
      picosystem::_reset_to_dfu();
    });
  }

  void init() {
    views.options.activated  = false;
    views.renderer.activated = false;
    views.map_edit.activated = false;

    stack.push("startup", &views.main_menu);
    stack.push("options", &views.options);
    stack.push("renderer", &views.renderer);
    stack.push("map_edit", &views.map_edit);

    last_backlight = State::getOptRangeVal(Option::BACKLIGHT);
  }

  void update(uint32_t tick) {
    using namespace picosystem;

    stack.update(tick);

    if (views.options.activated) {
      if (last_backlight != State::getOptRangeVal(Option::BACKLIGHT)) {
        last_backlight = State::getOptRangeVal(Option::BACKLIGHT);
        backlight(last_backlight);
      }

      if (State::getOptRangeVal(Option::MOVE_SPEED) / OPT_MOVE_SPEED_SCALE != State::player.movement_speed) {
        State::player.movement_speed = State::getOptRangeVal(Option::MOVE_SPEED) / OPT_MOVE_SPEED_SCALE;
      }

      if (State::getOptRangeVal(Option::ROT_SPEED) / OPT_ROT_SPEED_SCALE != State::player.rotation_speed) {
        State::player.rotation_speed = State::getOptRangeVal(Option::ROT_SPEED) / OPT_ROT_SPEED_SCALE;
      }
    }
  }

  void draw(uint32_t tick) {
    using namespace picosystem;

    pen(0, 0, 0);
    clear();

    stack.draw(tick);

    if (State::options[Option::SHOW_DEBUG].value.boolean) {
      pen(0xF, 0xF, 0xF);
      text(
        "views: " +
          std::to_string(views.main_menu.activated) +
          std::to_string(views.options.activated)   +
          std::to_string(views.renderer.activated)  +
          std::to_string(views.map_edit.activated),
        1, SCREEN->h - 10);

      pen(0xF, 0xF, 0xF);
      text("ev: " + events::last_published, 1, SCREEN->h - 20);
    }

    if (State::options[Option::SHOW_FPS].value.boolean) {
      auto fps_str = std::to_string(stats.fps);
      int32_t w, h;
      measure(fps_str, w, h);
      pen(0xF, 0xF, 0xF);
      text(fps_str, SCREEN->w - w - 1, 1);
    }

    if (State::options[Option::SHOW_BATTERY].value.boolean) {
      auto bat_str = std::to_string(battery()) + "%";
      int32_t w, h;
      measure(bat_str, w, h);
      pen(0xF, 0xF, 0xF);
      text(bat_str, SCREEN->w - w - 1, SCREEN->h - h - 1);
    }
  }
};
