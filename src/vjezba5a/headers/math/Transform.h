#ifndef TRANSFORM_H
#define TRANSFORM_H

#pragma once
#include "glm/glm.hpp"

namespace eng {
class Transform {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  virtual ~Transform() = default;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  // Position
  [[nodiscard]] glm::vec3 get_position() const noexcept;
  void set_position(glm::vec3 pos);
  // Scale
  [[nodiscard]] glm::vec3 get_scale() const noexcept;
  void set_scale(glm::vec3 scale);

  // ----------------------------------
  // METHODS
  // ----------------------------------

  // MATRIX GENERATION
  // Model matrix
  [[nodiscard]] virtual glm::mat4 model_matrix() const;
  // View matrix
  [[nodiscard]] virtual glm::mat4 view_matrix() const;

  // Orientation
  void change_orientation(glm::vec3 look_at, glm::vec3 view_up = {0, 1, 0});

  // Transformations
  // Rotation
  void rotate(float angle, const glm::vec3 &axis);
  void rotateFPS(float pitch, float yaw, bool constrain_pitch = true);

  // Local movement
  void move_local_z(float delta);
  void move_local_x(float delta);
  void move_local_y(float delta);

  // Global movement

 protected:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  glm::vec4 _origin{0.0F, 0.0F, 0.0F, 1.0F};
  glm::vec4 _x_axis{1.0F, 0.0F, 0.0F, 0.0F};
  glm::vec4 _y_axis{0.0F, 1.0F, 0.0F, 0.0F};
  glm::vec4 _z_axis{0.0F, 0.0F, 1.0F, 0.0F};
  glm::vec4 _scale{1.0F, 1.0F, 1.0F, 0.0F};
};

}  // namespace eng
#endif
