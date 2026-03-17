#pragma once
#include "Graphics.h"

namespace eng {
/**
 * Bresenham's line algorithm implementation.
 * Note: This implementation assumes that the input coordinates are within the screen
 * bounds.
 * Note: Current viewport is defined so the y-axis is inverted, Graphics class takes
 * care of correctly inverting the y-coordinate when shading the fragment, so we can use
 * the standard Bresenham's line algorithm without any modifications.
 */
inline void draw_line(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1);

// Implementation details
namespace detail {
// 0 to 90 degrees
void bresenham_line_1(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color);
// 0 to -90 degrees
void bresenham_line_2(eng::Graphics &screen, int32 x0, int32 y0, int32 x1, int32 y1,
                      const glm::vec3 &color);
}  // namespace detail
}  // namespace eng
