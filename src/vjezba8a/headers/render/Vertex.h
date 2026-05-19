#ifndef VERTEX_H
#define VERTEX_H

#pragma once
#include <glm/glm.hpp>

namespace eng {
struct Vertex {
  glm::vec3 coords = glm::vec3(0.0F);
  glm::vec3 normal = glm::vec3(0.0F);
  glm::vec2 tex_coords = glm::vec2(0.0F);

  Vertex() = default;
  Vertex(const glm::vec3 &coords, const glm::vec3 &normal, const glm::vec2 &tex_coords)
      : coords(coords), normal(normal), tex_coords(tex_coords) {}
};
}  // namespace eng

#endif
