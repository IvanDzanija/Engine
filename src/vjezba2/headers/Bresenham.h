#pragma once
#include "Graphics.h"

namespace eng {
namespace detail {
void bresenham_line_1(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color);
void bresenham_line_2(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color);
}  // namespace detail

/**
 * Bresenham's line algorithm implementation.
 * Note: This implementation assumes that the input coordinates are within the screen
 * bounds.
 * Note: Current viewport is defined so the y-axis is inverted, Graphics class takes
 * care of correctly inverting the y-coordinate when shading the fragment, so we can use
 * the standard Bresenham's line algorithm without any modifications.
 */
void draw_line(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1) {
  // Color based on
  float red = static_cast<float>(x0) / screen.get_width();
  float green =
      std::abs(static_cast<float>(y1) - static_cast<float>(y0)) / screen.get_height();
  float blue =
      std::abs(static_cast<float>(x1) - static_cast<float>(x0)) / screen.get_width();
  glm::vec3 color(red, green, blue);

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

namespace detail {
// 0 to 90 degrees
inline void bresenham_line_1(eng::Graphics &screen, int32 x0, int32 y0, int32 x1,
                             int32 y1, const glm::vec3 &color) {
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
inline void bresenham_line_2(eng::Graphics &screen, int32 x0, int32 y0, int32 x1,
                             int32 y1, const glm::vec3 &color) {
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
}  // namespace eng
