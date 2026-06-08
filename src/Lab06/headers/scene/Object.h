#ifndef OBJECT_H
#define OBJECT_H

#pragma once
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <memory>

#include "math/Transform.h"
#include "render/Axis.h"
#include "render/Mesh.h"
#include "render/Shader.h"

namespace eng {
class Object : public Transform {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  Object() = default;
  Object(std::shared_ptr<Shader> shader);
  Object(std::vector<std::shared_ptr<Renderable>> meshes,
         std::shared_ptr<Shader> shader);
  ~Object() override = default;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  void set_texture(std::shared_ptr<Texture> texture) { _texture = std::move(texture); }

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void render(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix,
              const std::optional<glm::vec3> &camera_position = std::nullopt) const;
  void add_renderable(std::shared_ptr<Renderable> renderable);

 private:
  std::vector<std::shared_ptr<Renderable>> _renderables;
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<Texture> _texture;
  Axis _local_axis;

  // Material material;

  void _draw_axes(const glm::mat4 &projection_matrix,
                  const glm::mat4 &view_matrix) const;
};

}  // namespace eng

#endif
