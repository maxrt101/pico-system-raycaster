#pragma once

#include <unordered_map>
#include <functional>
#include <vector>
#include <string>

using Event = std::string;
using EventSubscriber = std::function<void(void*)>;

struct events {
  static std::unordered_map<Event, std::vector<EventSubscriber>> subscribers;

  static std::string last_published;

  static void subscribe(const Event & ev, EventSubscriber sub) {
    if (subscribers.find(ev) == subscribers.end()) {
      subscribers[ev] = {};
    }

    subscribers[ev].push_back(sub);
  }

  static void publish(const Event & ev, void * payload = nullptr) {
    last_published = ev;
    for (auto & s : subscribers[ev]) {
      s(payload);
    }
  }

};
