#pragma once

#include <cstdint>
#include <string>

struct Option {
  enum {
    SHOW_DEBUG   = 0,
    SHOW_FPS     = 1,
    SHOW_BATTERY = 2,
    BACKLIGHT    = 3,

    OPTIONS_MAX,
  };

  enum Type {
    BOOL,
    RANGE
  };

  union Value {
    bool boolean;
    struct {
      int32_t from, to, value;
    } range;
  };

  Type type;
  Value value;

  Option() : type(BOOL) {
    value.boolean = false;
  }

  Option(bool value) : type(BOOL) {
    this->value.boolean = value;
  }

  Option(int32_t from, int32_t to, int32_t value) : type(RANGE) {
    this->value.range.from  = from;
    this->value.range.to    = to;
    this->value.range.value = value;
  }

  static std::string kindToString(uint8_t opt) {
    switch (opt) {
      case SHOW_DEBUG:
        return "Show Debug";
      case SHOW_FPS:
        return "Show FPS";
      case SHOW_BATTERY:
        return "Show Battery";
      case BACKLIGHT:
        return "Backlight";
      default:
        return "?";
    }
  }
};
