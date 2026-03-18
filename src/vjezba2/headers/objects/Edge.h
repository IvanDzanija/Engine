#pragma once
#include "Global.h"
#include "Point.h"

namespace eng {
struct Edge2D {
  // Line equation coefficients: ax + by + ch = 0
  int32 a;
  int32 b;
  int32 c;
  int32 homogeneous = 1;

  Edge2D() : a(0), b(0), c(0) {}
  Edge2D(int32 a, int32 b, int32 c) : a(a), b(b), c(c) {}
  Edge2D(int32 a, int32 b, int32 c, int32 homogeneous)
      : a(a), b(b), c(c), homogeneous(homogeneous) {}

  // Find the line equation coefficients given two points
  Edge2D(const Point2D &p1, const Point2D &p2) {
    // Using cross product to find the line coefficients
    a = p1.y - p2.y;
    b = p2.x - p1.x;
    c = (p1.x * p2.y) - (p2.x * p1.y);
  };
};
}  // namespace eng
