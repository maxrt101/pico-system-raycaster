#pragma once

#include "picosystem.hpp"
#include "raycaster/state.h"
#include "util/util.h"

namespace views {

struct Options : ui::View {
  int32_t selected;

  struct {
    picosystem::color_t range    = picosystem::rgb(0xF, 0xF, 0xF);
    picosystem::color_t selected = picosystem::rgb(0x8, 0x8, 0xF);
    picosystem::color_t off      = picosystem::rgb(0xF, 0x8, 0x8);
    picosystem::color_t on       = picosystem::rgb(0x8, 0xF, 0x8);
  } colors;

  Options() {}
  ~Options() override = default;

  void init() {
    ignore_inputs_for_next_frame = true;
  }

  void update(uint32_t tick) {
    using namespace picosystem;

    if (pressed(A)) {
      events::publish("options_exit");
    }

    if (pressed(UP)) {
      selected = util::cap<int32_t>(selected - 1, 0, Option::OPTIONS_MAX - 1);
    }

    if (pressed(DOWN)) {
      selected = util::cap<int32_t>(selected + 1, 0, Option::OPTIONS_MAX - 1);
    }

    if (button(LEFT) && State::options[selected].type == Option::RANGE) {
      State::options[selected].value.range.value = util::cap<int32_t>(
        State::options[selected].value.range.value - 1,
        State::options[selected].value.range.from,
        State::options[selected].value.range.to
      );
    }

    if (button(RIGHT) && State::options[selected].type == Option::RANGE) {
      State::options[selected].value.range.value = util::cap<int32_t>(
        State::options[selected].value.range.value + 1,
        State::options[selected].value.range.from,
        State::options[selected].value.range.to
      );
    }

    if (pressed(B)) {
      if (State::options[selected].type == Option::BOOL) {
        State::options[selected].value.boolean = !State::options[selected].value.boolean;
      }
    }
  }

  void draw(uint32_t tick) {
    using namespace picosystem;

    pen(0xF, 0xF, 0xF);
    text("Options:", 0, 0);

    int32_t y = 20;

    for (int32_t i = 0; i < Option::OPTIONS_MAX; ++i) {
      auto s = Option::kindToString(i);

      int32_t w, h;
      measure(s, w, h);

      if (State::options[i].type == Option::BOOL) {
        pen(i == selected ? colors.selected :
          (State::options[i].value.boolean ? colors.on : colors.off)
        );
      } else if (State::options[i].type == Option::RANGE) {
        pen(i == selected ? colors.selected : colors.range);
      }

      text(s, 0, y);

      if (State::options[i].type == Option::RANGE) {
        s = std::to_string(State::options[i].value.range.value);
        measure(s, w, h);
        text(s, SCREEN->w - w, y);
      }

      y += h;
    }
  }
};

}