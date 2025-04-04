#pragma once

#include "picosystem.hpp"
#include "ui/view.h"
#include "util/util.h"
#include <functional>
#include <vector>

namespace ui {

struct MenuData {
  std::function<void(const std::string& element)> on_select;

  uint8_t flags;

  struct {
    int32_t       spacing = 0;
    Vec2<int32_t> start   = {0, 0};
    Vec2<int32_t> size    = {util::SCREEN_SIZE, util::SCREEN_SIZE};
  } geometry;

  std::vector<std::string> options;
  int32_t selected = 0;

  struct {
    uint32_t prev   = picosystem::UP;
    uint32_t next   = picosystem::DOWN;
    uint32_t select = picosystem::B;
  } inputs;

  struct {
    picosystem::color_t option   = picosystem::rgb(0xF, 0xF, 0xF);
    picosystem::color_t selected = picosystem::rgb(0x8, 0xF, 0x8);
  } colors;
};

struct Menu : View {
  enum Flags {
    NONE   = 0,
    CENTER = (1 << 0),
  };

  MenuData menu;

  Menu() = default;
  Menu(const MenuData& m) : menu(m) {}
  ~Menu() override = default;

  void add(const std::string& element) {
    menu.options.push_back(element);
  }

  void init() {}

  void update(uint32_t tick) {
    if (picosystem::pressed(menu.inputs.prev)) {
      menu.selected = util::cap<int32_t>(menu.selected - 1, 0, menu.options.size() - 1);
    }

    if (picosystem::pressed(menu.inputs.next)) {
      menu.selected = util::cap<int32_t>(menu.selected + 1, 0, menu.options.size() - 1);
    }

    if (picosystem::pressed(menu.inputs.select)) {
      if (menu.on_select) {
        menu.on_select(menu.options[menu.selected]);
      }
    }
  }

  void draw(uint32_t tick) {
    int32_t y = menu.geometry.start.y;

    for (size_t i = 0; i < menu.options.size(); ++i) {
      int32_t w, h;
      picosystem::measure(menu.options[i], w, h);

      picosystem::pen(i == menu.selected ? menu.colors.selected : menu.colors.option);

      int32_t x = menu.geometry.start.x;

      if (menu.flags & CENTER) {
        x = (menu.geometry.size.x - w) / 2;
      }

      picosystem::text(menu.options[i], x, y);

      y += menu.geometry.spacing + h;
    }
  }
};

}

