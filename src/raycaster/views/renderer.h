#pragma once

#include "util/util.h"
#include "raycaster/state.h"
#include "raycaster/resources.h"
#include <cmath>

#define MAP_TILE_SCALE          (SCREEN_SIZE / MAP_SIZE)
#define USE_SHADED_WALL         0
#define USE_CAP_CEILING         0
#define USE_SAMPLE_MAP          1
#define USE_2D_MAP_RENDER       1
#define MAP_RENDER_SCALE        1

namespace views {

struct Renderer : ui::View {
  enum Side {
    NORTH, SOUTH, WEST, EAST, TOP, BOTTOM
  };

  struct TileHit {
    Vec2<int>     tile_position = {0, 0};
    Vec2<double>  hit_position  = {0, 0};
    float         ray_length    = 0.0f;
    float         sample_x      = 0.0f;
    Side          side          = NORTH;
  };

  struct DDAResult {
    bool hit_wall = false;

    TileHit tile;
  };
  
  float fov   = 3.14159f / 6.0f;
  float depth = 30.0f;
  float step  = 0.01f;

  SpritesheetKind spritesheet_kind = SpritesheetKind::WOLF3D;

  Renderer() {}
  ~Renderer() override = default;

  void init() {
    State::player.position.x = State::map.size() / 2;
    State::player.position.y = State::map.size() / 2;
    ignore_inputs_for_next_frame = true;
  }

  void update(uint32_t tick) {
    using namespace picosystem;

    if (pressed(UP) && pressed(X)) {
      events::publish("renderer_exit");
    }

    if (pressed(Y)) {
      events::publish("map_edit_enter", this);
    }

    if (button(LEFT)) {
      State::player.angle -= State::player.rotation_speed; //* frameTime;
      if (State::player.angle < -2.0f * M_PI) { // kinda works?
        State::player.angle += 2.0f * M_PI;
      }
    }

    if (button(RIGHT)) {
      State::player.angle += State::player.rotation_speed; //* frameTime;
      if (State::player.angle > 2.0f * M_PI) { // kinda works?
        State::player.angle -= 2.0f * M_PI;
      }
    }

    if (button(UP)) {
      State::player.position.x += sinf(State::player.angle) * State::player.movement_speed; //* frameTime;
      State::player.position.y += cosf(State::player.angle) * State::player.movement_speed; //* frameTime;

      if (State::map.get((int) State::player.position.x, (int) State::player.position.y).type == TILE_WALL) {
        State::player.position.x -= sinf(State::player.angle) * State::player.movement_speed; //* frameTime;
        State::player.position.y -= cosf(State::player.angle) * State::player.movement_speed; //* frameTime;
      }
    }

    if (button(DOWN)) {
      State::player.position.x -= sinf(State::player.angle) * State::player.movement_speed; //* frameTime;
      State::player.position.y -= cosf(State::player.angle) * State::player.movement_speed; //* frameTime;

      if (State::map.get((int) State::player.position.x, (int) State::player.position.y).type == TILE_WALL) {
        State::player.position.x += sinf(State::player.angle) * State::player.movement_speed; //* frameTime;
        State::player.position.y += cosf(State::player.angle) * State::player.movement_speed; //* frameTime;
      }
    }
  }

  void draw(uint32_t tick) {
    using namespace picosystem;
    using namespace util;

    auto screen_height = SCREEN->h;
    auto screen_width = SCREEN->w;

    auto spritesheet = Spritesheet::get(spritesheet_kind);

    color_t texture_data[spritesheet->element_size * spritesheet->element_size];
    buffer_t texture;

    buffer_init(&texture, spritesheet->element_size, spritesheet->element_size, texture_data);

    for (int x = 0; x < screen_width; x++) {
      float ray_angle = (State::player.angle - fov/2.0f) + (x / (float)screen_width) * fov;
      Vec2<double> ray_direction = {sinf(ray_angle), cosf(ray_angle)};

      DDAResult result = cast_ray(State::player.position, ray_direction);

      if (result.hit_wall) {
        Vec2<double> ray = {
          result.tile.hit_position.x - State::player.position.x,
          result.tile.hit_position.y - State::player.position.y
        };

        float ray_length = sqrt(ray.x * ray.x + ray.y * ray.y) * cos(ray_angle - State::player.angle);

        float ceiling = (screen_height / 2.0f) - screen_height / ray_length;
        float floor = screen_height - ceiling;
        float wall_height = cap<float>(floor - ceiling, 0, screen_height);;

#if USE_SHADED_WALL
        int32_t color = scale(
          wall_height,
          {0, (float) screen_height},
          {1, 0xF}
        );

        pen(color, color, color, 0xF);
        vline(x, cap<float>(ceiling, 1, screen_height), wall_height);
#else
        int32_t mx = result.tile.hit_position.x, my = result.tile.hit_position.y;

        if (result.tile.side == SOUTH) {
          my -= 1;
        }

        if (result.tile.side == EAST) {
          mx -= 1;
        }

        spritesheet->select(&texture, State::map.get(mx, my).type - 1);

        float whole;
        int texture_x = std::modf(result.tile.sample_x, &whole) * spritesheet->element_size;

        // if (result.tile.side == SOUTH || result.tile.side == WEST) {
          // texture_x = spritesheet->element_size - texture_x - 1;
        // }

        blit(&texture,
          texture_x, 0,
          1, spritesheet->element_size,
#if USE_CAP_CEILING
          x, util::cap<int32_t>(ceiling, 0, screen_height),
#else
          x, ceiling,
#endif
          1, wall_height
        );
#endif
      }
    }

#if USE_2D_MAP_RENDER
    pen(0x8, 0x8, 0xF);
    for (int32_t y = 0; y < State::map.size(); ++y) {
      for (int32_t x = 0; x < State::map.size(); ++x) {
        if (State::map.get(x, y).isSolid()) {
          rect(x * MAP_RENDER_SCALE, y * MAP_RENDER_SCALE, MAP_RENDER_SCALE, MAP_RENDER_SCALE);
        }
      }
    }

    pen(0xF, 0, 0);
    pixel(State::player.position.x, State::player.position.y);
#endif
  }

private:
  DDAResult cast_ray(Vec2<double> src, Vec2<double> direction) {
    DDAResult result;

    Vec2<double> ray_delta = {
      sqrt(1 + (direction.y / direction.x) * (direction.y / direction.x)),
      sqrt(1 + (direction.x / direction.y) * (direction.x / direction.y))
    };

    Vec2<int> map_check = {(int)src.x, (int)src.y};
    Vec2<double> side_distance, step_distance;

    if (direction.x < 0) {
      step_distance.x = -1;
      side_distance.x = (src.x - (float)map_check.x) * ray_delta.x;
    } else {
      step_distance.x = 1;
      side_distance.x = ((float)map_check.x + 1 - src.x) * ray_delta.x;
    }

    if (direction.y < 0) {
      step_distance.y = -1;
      side_distance.y = (src.y - (float)map_check.y) * ray_delta.y;
    } else {
      step_distance.y = 1;
      side_distance.y = ((float)map_check.y + 1 - src.y) * ray_delta.y;
    }

    Vec2<double> intersection;
    Vec2<int> hit_tile;
    float max_distance  = 100.0f;
    float distance      = 0.0f;
    TileHit hit;

    while (!result.hit_wall && distance < max_distance) {
      if (side_distance.x < side_distance.y) {
        side_distance.x += ray_delta.x;
        map_check.x += step_distance.x;
      } else {
        side_distance.y += ray_delta.y;
        map_check.y += step_distance.y;
      }

      Vec2<double> ray_distance = {(float)map_check.x - src.x, (float)map_check.y - src.y};
      distance = sqrt(ray_distance.x * ray_distance.x + ray_distance.y * ray_distance.y);

      if (State::map.get(map_check).isSolid()) {
        hit_tile = map_check;

        result.hit_wall = true;

        hit.tile_position = map_check;

        float m = direction.y / direction.x;

        if (src.y <= map_check.y) {
          if (src.x <= map_check.x) {
            hit.side = WEST;
            intersection.y = m * (map_check.x - src.x) + src.y;
            intersection.x = float(map_check.x);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else if (src.x >= (map_check.x + 1)) {
            hit.side = EAST;
            intersection.y = m * ((map_check.x + 1) - src.x) + src.y;
            intersection.x = float(map_check.x + 1);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else {
            hit.side = NORTH;
            intersection.y = float(map_check.y);
            intersection.x = (map_check.y - src.y) / m + src.x;
            hit.sample_x = intersection.x - std::floor(intersection.x);
          }

          if (intersection.y < map_check.y) {
            hit.side = NORTH;
            intersection.y = float(map_check.y);
            intersection.x = (map_check.y - src.y) / m + src.x;
            hit.sample_x = intersection.x - std::floor(intersection.x);
          }
        } else if (src.y >= map_check.y + 1) {
          if (src.x <= map_check.x) {
            hit.side = WEST;
            intersection.y = m * (map_check.x - src.x) + src.y;
            intersection.x = float(map_check.x);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else if (src.x >= (map_check.x + 1)) {
            hit.side = EAST;
            intersection.y = m * ((map_check.x + 1) - src.x) + src.y;
            intersection.x = float(map_check.x + 1);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else {
            hit.side = SOUTH;
            intersection.y = float(map_check.y + 1);
            intersection.x = ((map_check.y + 1) - src.y) / m + src.x;
            hit.sample_x = intersection.x - std::floor(intersection.x);
          }

          if (intersection.y > (map_check.y + 1)) {
            hit.side = SOUTH;
            intersection.y = float(map_check.y + 1);
            intersection.x = ((map_check.y + 1) - src.y) / m + src.x;
            hit.sample_x = intersection.x - std::floor(intersection.x);
          }
        } else {
          if (src.x <= map_check.x) {
            hit.side = WEST;
            intersection.y = m * (map_check.x - src.x) + src.y;
            intersection.x = float(map_check.x);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          } else if (src.x >= (map_check.x + 1)) {
            hit.side = EAST;
            intersection.y = m * ((map_check.x + 1) - src.x) + src.y;
            intersection.x = float(map_check.x + 1);
            hit.sample_x = intersection.y - std::floor(intersection.y);
          }
        }

        hit.hit_position = intersection;

        result.tile = hit;
      }
    }

    return result;
  }
};

}