#pragma once

#include "picosystem.hpp"
#include "util/util.h"
#include "ui/view.h"
#include "ui/menu.h"
#include <string>

namespace views {

struct MainMenu : ui::View {
  static constexpr const char * TITLE   = "PICO RAYCASTER";
  static constexpr const char * VERSION = "v0.1";

  struct MenuEvent {
    std::string entry;
    std::string event;
  };

  ui::Menu menu;

  const std::vector<MenuEvent> menu_events = {
    {"start", "renderer_enter"},
    {"map edit", "map_edit_enter"},
    {"options", "options_enter"},
    {"dfu", "reset_to_dfu"},
  };

  MainMenu() {
    menu = ui::MenuData{
      .on_select = [this](auto e) { this->menu_on_select(e); },
      .flags = ui::Menu::CENTER,
      .geometry = {.start = {0, 40}},
    };

    for (auto& [entry, event] : menu_events) {
      menu.menu.options.push_back(entry);
    }
  }

  ~MainMenu() override = default;

  void init() {
    menu.init();
    ignore_inputs_for_next_frame = true;
  }

  void update(uint32_t tick) {
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
    for (auto& [entry, event] : menu_events) {
      if (entry == element) {
        events::publish(event, this);
        return;
      }
    }
  }
};

}
