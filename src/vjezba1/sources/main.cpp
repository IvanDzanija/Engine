#pragma once
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <print>

#include "Global.h"
#include "Graphics.h"

static constexpr int32 SWIDTH = 97;
static constexpr int32 SHEIGHT = 97;
static bool crop = false;
static std::vector<std::pair<int, int>> mouse_clicks;

namespace detail {
void bresenham_line_1(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color);
void bresenham_line_2(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color);
}  // namespace detail

void mouse_click(int32 x, int32 y, int32 button) {
  switch (button) {
    case 0:
      std::cout << "Left, " << std::endl;
      mouse_clicks.emplace_back(x, y);
      break;
    case 1:
      std::cout << "Right, " << std::endl;
      crop = !crop;
      break;
    default:
      std::cout << "Unknown:";
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

inline void draw_line(eng::Graphics &screen, int x0, int y0, int x1, int y1) {
  // Color based on
  float red = static_cast<float>(x0) / screen.get_width();
  float green =
      std::abs(static_cast<float>(y1) - static_cast<float>(y0)) / screen.get_height();
  float blue =
      std::abs(static_cast<float>(x1) - static_cast<float>(x0)) / screen.get_width();
  glm::vec3 color(red, green, blue);

  std::cout << "Drawing line from (" << x0 << ", " << y0 << ") to (" << x1 << ", " << y1
            << ")" << std::endl;

  if (x0 <= x1) {
    if (y0 <= y1) {
      detail::bresenham_line_1(screen, x0, y0, x1, y1, color);
    } else {
      detail::bresenham_line_2(screen, x0, y0, x1, y1, color);
    }
  } else {
    if (y0 <= y1) {
      detail::bresenham_line_2(screen, x1, y1, x0, y0, color);
    } else {
      detail::bresenham_line_1(screen, x1, y1, x0, y0, color);
    }
  }
}

inline void draw_mouse_clicks(eng::Graphics &screen) {
  for (int32 cnt = 0; cnt < static_cast<int32>(mouse_clicks.size()); ++cnt) {
    // if (crop) {
    //   int x = mouse_clicks[cnt].first;
    //   int y = HEIGHT - mouse_clicks[cnt].second - 1;
    //   if (x < WIDTH / 2 && y < HEIGHT / 2) {
    //     screen.shade_fragment(x, y, glm::vec3(0.6, 0.2, 0));
    //   }
    // }
    auto click = mouse_clicks[cnt];
    screen.shade_fragment(click.first, screen.get_height() - click.second - 1,
                          glm::vec3(0.6, 0.2, 0));
    if ((cnt & 1) == 1) {
      draw_line(screen, mouse_clicks[cnt - 1].first,
                screen.get_height() - mouse_clicks[cnt - 1].second - 1, click.first,
                screen.get_height() - click.second - 1);
    }
  }
}

inline void draw_boundary(eng::Graphics &screen) {
  int32 x0 = screen.get_width() / 3;
  int32 y0 = screen.get_height() / 3;
  int32 x1 = 2 * screen.get_width() / 3;
  int32 y1 = 2 * screen.get_height() / 3;

  draw_line(screen, x0, y0, x1, y0);
  draw_line(screen, x0, y0, x0, y1);
  draw_line(screen, x1, y0, x1, y1);
  draw_line(screen, x0, y1, x1, y1);
}

int main(int argc, char *argv[]) {
  eng::Graphics screen(SWIDTH, SHEIGHT, glm::vec3(0, 0, 0), argv[0]);

  eng::Graphics::register_mouse_click_method(mouse_click);
  eng::Graphics::register_framebuffer_resize_method(framebuffer_resize);

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
    if (crop) {
      draw_boundary(screen);
    }

    screen.draw_raster();

    // kako program ne bi trosio previse resursa, na kraj petlje se moze dodati poziv
    // funkcije za cekanje kao npr Sleep ili od c++11 na dalje
    // this_thread::sleep_for(chrono::milliseconds(16));
  }

  return EXIT_SUCCESS;
}

namespace detail {
// 0 to 90 degrees
void bresenham_line_1(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color) {
  const int32 dy = y1 - y0;
  const int32 dx = x1 - x0;
  if (dy <= dx) {
    const int32 change = dy << 1;       // 2 * dy
    const int32 correction = -dx << 1;  // -2 * dx

    int32 y_curr = y0;
    int32 y_cal = -dx;
    for (int32 x_curr = x0; x_curr <= x1; ++x_curr) {
      screen.shade_fragment(x_curr, y_curr, color);
      y_cal += change;
      if (y_cal >= 0) {
        ++y_curr;
        y_cal += correction;
      }
    }
  } else {
    const int32 change = dx << 1;       // 2 * dx
    const int32 correction = -dy << 1;  // -2 * dx

    int32 x_curr = x0;
    int32 x_cal = -dy;
    for (int32 y_curr = y0; y_curr <= y1; ++y_curr) {
      screen.shade_fragment(x_curr, y_curr, color);
      x_cal += change;
      if (x_cal >= 0) {
        ++x_curr;
        x_cal += correction;
      }
    }
  }
}

// 0 to -90 degrees
void bresenham_line_2(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color) {
  const int32 dy = y1 - y0;
  const int32 dx = x1 - x0;
  if (-dy <= dx) {
    const int32 change = dy << 1;      // 2 * dy
    const int32 correction = dx << 1;  // 2 * dy

    int32 y_curr = y0;
    int32 y_cal = dx;
    for (int32 x_curr = x0; x_curr <= x1; ++x_curr) {
      screen.shade_fragment(x_curr, y_curr, color);
      y_cal += change;
      if (y_cal <= 0) {
        --y_curr;
        y_cal += correction;
      }
    }
  } else {
    const int32 change = -dx << 1;      // 2 * dx
    const int32 correction = -dy << 1;  // 2 * dy

    int32 x_curr = x1;
    int32 x_cal = -dy;
    for (int32 y_curr = y1; y_curr <= y0; ++y_curr) {
      screen.shade_fragment(x_curr, y_curr, color);
      x_cal += change;
      if (x_cal <= 0) {
        --x_curr;
        x_cal += correction;
      }
    }
  }
}
}  // namespace detail
