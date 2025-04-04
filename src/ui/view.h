#pragma once

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <vector>

namespace ui {

struct View {
  bool activated = true;

  virtual ~View() = default;

  virtual void init() = 0;
  virtual void update(uint32_t tick) = 0;
  virtual void draw(uint32_t tick) = 0;
};

struct ViewStack {
  struct ViewContainer {
    const char * name;
    View * view;
  };

  std::vector<ViewContainer> views;

  void push(const char * name, View * view) {
    views.push_back({name, view});
  }

  void pop() {
    views.pop_back();
  }

  void remove(const char * name) {
    [[maybe_unused]] auto it = std::remove_if(
      views.begin(), views.end(),
      [name](auto & e) -> bool {
        return !strcmp(name, e.name);
      }
    );
  }

  void update(uint32_t tick) {
    for (auto & [name, view] : views) {
      if (view->activated) {
        view->update(tick);
      }
    }
  }

  void draw(uint32_t tick) {
    for (auto & [name, view] : views) {
      if (view->activated) {
        view->draw(tick);
      }
    }
  }
};

}
