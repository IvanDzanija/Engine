#pragma once
#include "Global.h"

namespace eng {
struct Point2D {
  int x;
  int y;
  int homogeneous = 1;

  Point2D() : x(0), y(0) {}
  Point2D(int x, int y) : x(x), y(y) {}
  Point2D(int x, int y, int homogeneous) : x(x), y(y), homogeneous(homogeneous) {}
};

struct Point3D {
  int x;
  int y;
  int z;
  int homogeneous = 1;

  Point3D() : x(0), y(0), z(0) {}
  Point3D(int x, int y, int z) : x(x), y(y), z(z) {}
  Point3D(int x, int y, int z, int homogeneous)
      : x(x), y(y), z(z), homogeneous(homogeneous) {}
};

bool is_near(const Point2D &a, const Point2D &b, int32 radius = 1) {
  int dx = a.x - b.x;
  int dy = a.y - b.y;
  return (dx * dx) + (dy * dy) <= radius * radius;
}
}  // namespace eng
