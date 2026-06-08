#ifndef CURVE_VERTEX_H
#define CURVE_VERTEX_H

#pragma once
#include <glm/glm.hpp>

namespace eng {
struct CurveVertex {
  glm::vec3 coords;
  glm::vec3 color;

  CurveVertex() = default;
  CurveVertex(const glm::vec3 &coords, const glm::vec3 &color)
      : coords(coords), color(color) {}
};
}  // namespace eng

#endif
