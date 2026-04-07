#pragma once

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

void mouse_click(int32 x, int32 y, int32 button) {
  switch (button) {
    case 0:
      switch (mode) {
        case ScreenMode::TESTING:
          testing_points.emplace_back(x, y);
          break;
        default:
          polygons.back().add_vertex(eng::Point2D(x, y));
          // Angle output
          double angle = polygons.back().calculate_angle();
          std::println("Added vertex at ({}, {}) with angle {:.2f} degrees.", x, y,
                       angle);

          // Convexity check
          std::optional<bool> convex = polygons.back().test_convex();
          if (polygons.back().is_closed()) {
            // std::println("Polygon closed with {} vertices.",
            //              polygons.back().vertex_count());
            // if (convex.has_value() && convex.value()) {
            //   std::println("Polygon is convex.");
            // } else {
            //   std::println("Polygon is not convex.");
            // }
            polygons.emplace_back();
          } else {
            if (convex.has_value() && !convex.value()) {
              std::println("This point made the polygon non-convex.");
            }
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
          std::println("Switching to TESTING mode.");
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
    screen->set_framebuffer_size(width, height);
  }
  auto [w, h] = screen->get_framebuffer_size();
  std::println("Framebuffer resized to {}x{}", w, h);
}

void draw_polygons(eng::Graphics &screen) {
  for (auto &polygon : polygons) {
    // std::optional<bool> convex = polygon.test_convex();
    // if (!convex.has_value() || (convex.has_value() && convex.value())) {
    polygon.draw_filled(screen);
    //}
  }
}

inline void draw_mouse_clicks(eng::Graphics &screen) {
  switch (mode) {
    case ScreenMode::PICKING:
      // Draw outlines of all closed polygons and vertices of the currently open polygon
      for (auto &polygon : polygons) {
        if (polygon.is_closed()) {
          polygon.draw_outline(screen);
        } else {
          for (int32 cnt = 0; cnt < static_cast<int32>(polygon.vertex_count()); ++cnt) {
            const auto &curr = polygon[cnt];
            screen.shade_fragment(curr.point.x, curr.point.y, glm::vec3(0.8, 0, 0));

            if (cnt > 0) {
              const auto &prev = polygon[cnt - 1];
              eng::draw_line(screen, curr.point.x, curr.point.y, prev.point.x,
                             prev.point.y);
            }
          }
        }
      }

      break;
    case ScreenMode::DRAWING:
      draw_polygons(screen);
      break;
    case ScreenMode::TESTING:
      draw_polygons(screen);
      for (const auto &point : testing_points) {
        bool inside_any = false;
        for (auto &polygon : polygons) {
          if (polygon.is_closed()) {
            if (polygon.test_point(point)) {
              inside_any = true;
            }
          }
        }
        if (inside_any) {
          screen.shade_fragment(point.x, point.y, glm::vec3(0, 0.8, 0));
        } else {
          screen.shade_fragment(point.x, point.y, glm::vec3(0.8, 0, 0));
        }
      }
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
    draw_mouse_clicks(screen);

    screen.draw_raster();

    // kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv
    // funkcije za cekanje kao npr Sleep ili od c++11 na dalje
    // this_thread::sleep_for(chrono::milliseconds(16));
  }

  return EXIT_SUCCESS;
}
