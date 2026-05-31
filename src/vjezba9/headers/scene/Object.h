#ifndef OBJECT_H
#define OBJECT_H

#pragma once
#include <glm/glm.hpp>
#include <memory>

#include "Light.h"
#include "infra/Model.h"
#include "math/Transform.h"
#include "render/Axis.h"
#include "render/Shader.h"

namespace eng {
class Object : public Transform {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  Object() = default;
  Object(std::shared_ptr<Shader> shader);
  Object(const std::shared_ptr<Model> &model, std::shared_ptr<Shader> shader);

  // RUle of 5
  Object(const Object &other);
  Object &operator=(const Object &other);
  Object(Object &&other) noexcept;
  Object &operator=(Object &&other) noexcept;
  ~Object() override = default;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  void set_shader(std::shared_ptr<Shader> shader) { _shader = std::move(shader); }
  void use_uniform_color(bool use_uniform) { _use_uniform_color = use_uniform; }
  void set_uniform_color(const glm::vec3 &color) { _uniform_color = color; }

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void render(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix,
              const std::shared_ptr<Light> &light,
              const std::optional<glm::vec3> &camera_position = std::nullopt) const;
  void add_renderable(std::shared_ptr<Renderable> renderable);

 private:
  std::vector<std::shared_ptr<Renderable>> _renderables;
  std::vector<std::shared_ptr<Material>> _materials;
  std::vector<std::shared_ptr<Texture>> _textures;
  std::shared_ptr<Shader> _shader;
  Axis _local_axis;

  glm::vec3 _uniform_color{1.0F, 0.0F, 0.0F};
  bool _use_uniform_color = true;

  // ----------------------------------
  // PRIVATE METHODS
  // ----------------------------------
  void _draw_axes(const glm::mat4 &projection_matrix,
                  const glm::mat4 &view_matrix) const;
};

}  // namespace eng

#endif
