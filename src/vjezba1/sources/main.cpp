#pragma once
#include <bitset>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <print>

#include "Global.h"
#include "Graphics.h"

static constexpr int32 SWIDTH = 97;
static constexpr int32 SHEIGHT = 97;
static int32 cropped_x0 = SWIDTH / 3;
static int32 cropped_y0 = SHEIGHT / 3;
static int32 cropped_x1 = 2 * SWIDTH / 3;
static int32 cropped_y1 = 2 * SHEIGHT / 3;
static bool crop = false;
static std::vector<std::pair<int, int>> mouse_clicks;

namespace detail {
void bresenham_line_1(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color);
void bresenham_line_2(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color);
void draw_cropped_lines(eng::Graphics &screen);
void draw_full_lines(eng::Graphics &screen);
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
  if (crop) {
    detail::draw_cropped_lines(screen);
  } else {
    detail::draw_full_lines(screen);
  }
}

inline void draw_boundary(eng::Graphics &screen) {
  cropped_x0 = screen.get_width() / 3;
  cropped_y0 = screen.get_height() / 3;
  cropped_x1 = 2 * screen.get_width() / 3;
  cropped_y1 = 2 * screen.get_height() / 3;

  draw_line(screen, cropped_x0, cropped_y0, cropped_x1, cropped_y0);
  draw_line(screen, cropped_x0, cropped_y0, cropped_x0, cropped_y1);
  draw_line(screen, cropped_x1, cropped_y0, cropped_x1, cropped_y1);
  draw_line(screen, cropped_x0, cropped_y1, cropped_x1, cropped_y1);
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

    // Drawing boundary has to go first
    // It has a side effect on the width and height variables
    if (crop) {
      draw_boundary(screen);
    }
    draw_mouse_clicks(screen);

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

void draw_cropped_lines(eng::Graphics &screen) {
  // Cohen Sutherland algorithm
  for (int32 cnt = 0; cnt < static_cast<int32>(mouse_clicks.size()); ++cnt) {
    if ((cnt & 1) == 1) {
      std::pair<int32, int32> p0 = mouse_clicks[cnt - 1];  // Left point
      std::pair<int32, int32> p1 = mouse_clicks[cnt];      // Right point

      std::bitset<4> code0;
      std::bitset<4> code1;
      code0[0] = p0.second > cropped_y1;  // Above
      code0[1] = p0.second < cropped_y0;  // Below
      code0[2] = p0.first > cropped_x1;   // Right
      code0[3] = p0.first < cropped_x0;   // Left
      code1[0] = p1.second > cropped_y1;  // Above
      code1[1] = p1.second < cropped_y0;  // Below
      code1[2] = p1.first > cropped_x1;   // Right
      code1[3] = p1.first < cropped_x0;   // Left
      std::bitset<4> result = code0 & code1;
      if (result.any()) {
        continue;
      }

      int32 dx = p1.first - p0.first;
      int32 dy = p1.second - p0.second;
      double a = static_cast<double>(dy) / static_cast<double>(dx);
      double b = static_cast<double>(p0.second) - (a * static_cast<double>(p0.first));
      while (code0.any()) {
        if (code0[0]) {
          // y = y1_cropped, x = (y1_cropped - b) / a
          p0.first = static_cast<int32>((cropped_y1 - b) / a);
          p0.second = cropped_y1;
          code0[0] = false;
        } else if (code0[1]) {
          p0.first = static_cast<int32>((cropped_y0 - b) / a);
          p0.second = cropped_y0;
        } else if (code0[2]) {
          p0.second = static_cast<int32>(a * cropped_x1 + b);
          p0.first = cropped_x1;
        } else if (code0[3]) {
          p0.second = static_cast<int32>(a * cropped_x0 + b);
          p0.first = cropped_x0;
        }
      }
      while (code1.any()) {
        if (code1[0]) {
          p1.first = static_cast<int32>((cropped_y1 - b) / a);
          p1.second = cropped_y1;
        } else if (code1[1]) {
          p1.first = static_cast<int32>((cropped_y0 - b) / a);
          p1.second = cropped_y0;
        } else if (code1[2]) {
          p1.second = static_cast<int32>(a * cropped_x1 + b);
          p1.first = cropped_x1;
        } else if (code1[3]) {
          p1.second = static_cast<int32>(a * cropped_x0 + b);
          p1.first = cropped_x0;
        }
      }
      draw_line(screen, p0.first, p0.second, p1.first, p1.second);
    }
  }
}

void draw_full_lines(eng::Graphics &screen) {
  for (int32 cnt = 0; cnt < static_cast<int32>(mouse_clicks.size()); ++cnt) {
    if ((cnt & 1) == 1) {
      draw_line(screen, mouse_clicks[cnt - 1].first,
                screen.get_height() - mouse_clicks[cnt - 1].second - 1,
                mouse_clicks[cnt].first,
                screen.get_height() - mouse_clicks[cnt].second - 1);
    }
  }
}

}  // namespace detail
