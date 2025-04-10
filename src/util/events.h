#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <string>

#ifndef SAVE_LAST_EVENT
#define SAVE_LAST_EVENT 1
#endif

using Event = std::string;
using EventSubscriber = std::function<void(void*)>;

struct events {
  static std::unordered_map<Event, std::vector<EventSubscriber>> subscribers;

#if SAVE_LAST_EVENT
  static std::string last_published;
#endif

  static void subscribe(const Event & ev, EventSubscriber sub) {
    if (subscribers.find(ev) == subscribers.end()) {
      subscribers[ev] = {};
    }

    subscribers[ev].push_back(sub);
  }

  static void publish(const Event & ev, void * payload = nullptr) {
#if SAVE_LAST_EVENT
    last_published = ev;
#endif
    for (auto & s : subscribers[ev]) {
      s(payload);
    }
  }

};
