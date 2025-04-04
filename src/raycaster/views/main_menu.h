#pragma once

#include "picosystem.hpp"
#include "util/util.h"
#include "ui/view.h"
#include "ui/menu.h"
#include <string>

namespace views {

struct MainMenu : ui::View {
  static constexpr const char * TITLE = "PICO RAYCASTER";
  static constexpr const char * VERSION = "v0.1";

  ui::Menu menu;

  bool ignore_inputs = true;

  MainMenu() {
    menu = ui::MenuData{
      .on_select = [this](auto e) { this->menu_on_select(e); },
      .flags = ui::Menu::CENTER,
      .geometry = {.start = {0, 40}},
      .options = {"start", "map edit", "options", "dfu"}
    };
  }

  ~MainMenu() override = default;

  void init() {
    menu.init();
    ignore_inputs = true;
  }

  void update(uint32_t tick) {
    if (ignore_inputs) {
      ignore_inputs = false;
      return;
    }

    menu.update(tick);
  }

  void draw(uint32_t tick) {
    using namespace picosystem;

    int32_t w, h;

    measure(TITLE, w, h);
    pen(0xF, 0xF, 0xF);
    text(TITLE, (SCREEN->w - w) / 2, 10);

    measure(VERSION, w, h);
    pen(0xF, 0xF, 0xF);
    text(VERSION, (SCREEN->w - w) / 2, 20);

    menu.draw(tick);
  }

  void menu_on_select(const std::string& element) {
    if (element == "start") {
      events::publish("renderer_enter");
    }

    if (element == "map edit") {
      events::publish("map_edit_enter", this);
    }

    if (element == "options") {
      events::publish("options_enter");
    }

    if (element == "dfu") {
      events::publish("reset_to_dfu");
    }
  }
};

}
