#pragma once
#include "Edge.h"
#include "Point.h"
#include "Polygon.h"

namespace eng {
[[nodiscard]] inline static bool point_on_segment(const Point2D &p, const Point2D &a,
                                                  const Point2D &b) {
  // (B-A) x (P-A)
  int64 cross = (int64(b.x - a.x) * (p.y - a.y)) - (int64(b.y - a.y) * (p.x - a.x));
  if (cross != 0) {
    return false;
  }

  return std::min(a.x, b.x) <= p.x && p.x <= std::max(a.x, b.x) &&
         std::min(a.y, b.y) <= p.y && p.y <= std::max(a.y, b.y);
}
}  // namespace eng
