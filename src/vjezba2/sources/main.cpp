#pragma once
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <print>

#include "Global.h"
#include "Graphics.h"
#include "objects/Polygon.h"

/* Coordinate system is such that (0,0) is in the top left corner */
static constexpr int32 SWIDTH = 97;
static constexpr int32 SHEIGHT = 97;
static std::vector<eng::Polygon> polygons(1);
static std::vector<eng::Point2D> testing_points;

enum class ScreenMode : uint8 { PICKING, DRAWING, TESTING };
static ScreenMode mode = ScreenMode::PICKING;

struct AppState {
  GLFWcursor *arrow_cursor = nullptr;
  GLFWcursor *crosshair_cursor = nullptr;
  bool hovering_close = false;
};

namespace detail {
void draw_full_lines(eng::Graphics &screen);
double calculate_angle(const eng::Polygon &polygon) {
  if (polygon.vertex_count() < 3) {
    return 0.0;
  }
  const auto &last = polygon[polygon.vertex_count() - 1];
  const auto &second_last = polygon[polygon.vertex_count() - 2];
  const auto &third_last = polygon[polygon.vertex_count() - 3];
  glm::vec2 v2 =
      glm::vec2(last.point.x - second_last.point.x, last.point.y - second_last.point.y);
  glm::vec2 v1 = glm::vec2(second_last.point.x - third_last.point.x,
                           second_last.point.y - third_last.point.y);
  double dot_product = glm::dot(v1, v2);
  double norm_v1 = glm::length(v1);
  double norm_v2 = glm::length(v2);
  if (norm_v1 == 0 || norm_v2 == 0) {
    return 0.0;
  }
  double cos_alpha = dot_product / (norm_v1 * norm_v2);
  return glm::degrees(std::acos(cos_alpha));
}
}  // namespace detail

void mouse_click(int32 x, int32 y, int32 button) {
  switch (button) {
    case 0:
      switch (mode) {
        case ScreenMode::TESTING:
          testing_points.emplace_back(x, y);
          break;
        default:
          polygons.back().add_vertex(eng::Point2D(x, y));
          double angle = detail::calculate_angle(polygons.back());
          std::println("Added vertex at ({}, {}) with angle {:.2f} degrees.", x, y,
                       angle);
          if (polygons.back().is_closed()) {
            std::println("Polygon closed with {} vertices.",
                         polygons.back().vertex_count());
            polygons.emplace_back();
          }
      }
      break;
    case 1:
      switch (mode) {
        case ScreenMode::PICKING:
          std::println("Switching to DRAWING mode.");
          mode = ScreenMode::DRAWING;
          break;
        case ScreenMode::DRAWING:
          mode = ScreenMode::TESTING;
          break;
        case ScreenMode::TESTING:
          testing_points.clear();
          polygons.clear();
          polygons.emplace_back();
          mode = ScreenMode::PICKING;
          break;
      }
      // std::println("Right click at ({}, {})", x, y);
      break;
    default:
      std::cout << "Unknown:";
  }
}

void cursor_position(GLFWwindow *window, double xpos, double ypos) {
  auto *screen = static_cast<eng::Graphics *>(glfwGetWindowUserPointer(window));
  if (screen == nullptr) {
    return;
  }
  auto *app = static_cast<AppState *>(screen->get_user_state());

  eng::Point2D mouse{static_cast<int32>(xpos), static_cast<int32>(ypos)};
  auto &polygon = polygons.back();

  const bool should_hover_close = polygon.would_close(mouse);

  if (should_hover_close == app->hovering_close) {
    return;
  }

  app->hovering_close = should_hover_close;
  if (should_hover_close) {
    glfwSetCursor(window, app->crosshair_cursor);
  } else {
    glfwSetCursor(window, app->arrow_cursor);
  }
}

void framebuffer_resize(GLFWwindow *window, int width, int height) {
  std::println("Framebuffer resizing to {}x{}", width, height);
  auto *screen = static_cast<eng::Graphics *>(glfwGetWindowUserPointer(window));
  if (screen != nullptr) {
    screen->resize_framebuffer(width, height);
  }
  auto [w, h] = screen->get_framebuffer_size();
  std::println("Framebuffer resized to {}x{}", w, h);
}

inline void draw_mouse_clicks(eng::Graphics &screen) {
  switch (mode) {
    case ScreenMode::PICKING:
      detail::draw_full_lines(screen);
      break;
    case ScreenMode::DRAWING:
      // Drawing mode is for filling up the polygons
      break;
    case ScreenMode::TESTING:
      // Testing mode is for testing if points are inside any of the polygons
      break;
  }
}

int main(int argc, char *argv[]) {
  eng::Graphics screen(SWIDTH, SHEIGHT, glm::vec3(0, 0, 0), argv[0]);

  AppState app{};
  app.arrow_cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
  app.crosshair_cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
  // Register callbacks
  eng::Graphics::register_mouse_click_method(mouse_click);
  eng::Graphics::register_framebuffer_resize_method(framebuffer_resize);
  eng::Graphics::register_cursor_position_method(cursor_position);
  // Register state of app
  screen.register_user_state(&app);

  while (eng::Graphics::should_close()) {
    screen.clear_window();

    // Checkboard pattern
    for (int i = 0; i < screen.get_height(); i += 1) {
      for (int j = 0; j < screen.get_width(); j += 1) {
        if ((i + j) % 2 == 0) {
          screen.shade_fragment(i, j, glm::vec3(0.1, 0.4, 0.1));
        }
        if (i % 10 == 0 && j % 10 == 0) {
          screen.shade_fragment(i, j, glm::vec3(0.15, 0.15, 0.2));
        }
      }
    }
    switch (mode) {
      case ScreenMode::PICKING:
        draw_mouse_clicks(screen);
        break;
      case ScreenMode::DRAWING:
        for (const auto &polygon : polygons) {
          polygon.draw_outline(screen);
        }
        // Drawing mode is for filling up the polygons
        break;
      case ScreenMode::TESTING:
        // Testing mode is for testing if points are inside any of the polygons
        break;
    }

    screen.draw_raster();

    // kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv
    // funkcije za cekanje kao npr Sleep ili od c++11 na dalje
    // this_thread::sleep_for(chrono::milliseconds(16));
  }

  return EXIT_SUCCESS;
}

namespace detail {
void draw_full_lines(eng::Graphics &screen) {
  for (const auto &polygon : polygons) {
    for (int32 cnt = 0; cnt < static_cast<int32>(polygon.vertex_count()); ++cnt) {
      const auto &curr = polygon[cnt];
      screen.shade_fragment(curr.point.x, curr.point.y, glm::vec3(0.8, 0, 0));

      if (cnt > 0) {
        const auto &prev = polygon[cnt - 1];
        eng::draw_line(screen, curr.point.x, curr.point.y, prev.point.x, prev.point.y);
      }
    }
  }
}

}  // namespace detail
