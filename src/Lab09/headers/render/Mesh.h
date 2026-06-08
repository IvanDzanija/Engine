#ifndef MESH_H
#define MESH_H

#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "Renderable.h"
#include "Texture.h"
#include "Vertex.h"

namespace eng {
struct BoundingBox {
  glm::vec3 min;
  glm::vec3 max;
};
class Mesh : public Renderable {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  // Copy constructor
  Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32> &indices,
       const std::vector<std::shared_ptr<Texture>> &textures, uint32 material_index);
  // Move constructor
  Mesh(std::vector<Vertex> &&vertices, std::vector<uint32> &&indices,
       std::vector<std::shared_ptr<Texture>> &&textures, uint32 material_index);

  // Rule of 5
  Mesh(const Mesh &other) = delete;
  Mesh &operator=(const Mesh &other) = delete;
  Mesh(Mesh &&other) noexcept;
  Mesh &operator=(Mesh &&other) noexcept;
  ~Mesh() override;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] const std::vector<Vertex> &get_vertices() const noexcept;
  [[nodiscard]] const std::vector<uint32> &get_indices() const noexcept;
  [[nodiscard]] const std::vector<std::shared_ptr<Texture>> &get_textures()
      const noexcept;
  [[nodiscard]] uint32 get_material_index() const noexcept;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void draw(std::shared_ptr<Shader> shader) const override;
  [[nodiscard]] RenderableType get_type() const override;
  [[nodiscard]] BoundingBox get_bounding_box() const;

  void apply_transform(const glm::mat4 &matrix);

 private:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  std::vector<Vertex> _vertices;
  std::vector<uint32> _indices;
  std::vector<std::shared_ptr<Texture>> _textures;
  uint32 _material_index = 0;
  GLuint _vbo = 0;
  GLuint _ebo = 0;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void _setup_mesh();
};
}  // namespace eng

#endif
