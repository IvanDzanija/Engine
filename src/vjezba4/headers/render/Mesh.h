#ifndef MESH_H
#define MESH_H

#pragma once
#include <glm/glm.hpp>
#include <vector>

#include "Global.h"
#include "Renderable.h"
#include "Texture.h"
#include "Vertex.h"

namespace eng {
class Mesh : public Renderable {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  // Copy constructor
  Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32> &indices,
       const std::vector<Texture> &textures);
  // Move constructor
  Mesh(std::vector<Vertex> &&vertices, std::vector<uint32> &&indices,
       std::vector<Texture> &&textures);
  ~Mesh() override;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] const std::vector<Vertex> &get_vertices() const noexcept;
  [[nodiscard]] const std::vector<uint32> &get_indices() const noexcept;
  [[nodiscard]] const std::vector<Texture> &get_textures() const noexcept;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void draw() const override;

 private:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  std::vector<Vertex> _vertices;
  std::vector<uint32> _indices;
  std::vector<Texture> _textures;
  GLuint _vbo = 0;
  GLuint _ebo = 0;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void _setup_mesh();
};
}  // namespace eng

#endif
