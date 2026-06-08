#pragma once
#include "math/Transform.h"

#include "Global.h"
#include "math/TransformGenerator.h"

namespace eng {
// ----------------------------------
// GETTERS & SETTERS
// ----------------------------------
// Position
[[nodiscard]] glm::vec3 Transform::get_position() const noexcept { return _origin; }
void Transform::set_position(glm::vec3 pos) { _origin = glm::vec4(pos, 1.0F); }
// Scale
[[nodiscard]] glm::vec3 Transform::get_scale() const noexcept { return _scale; }
void Transform::set_scale(glm::vec3 scale) { _scale = glm::vec4(scale, 1.0F); }

// ----------------------------------
// METHODS
// ----------------------------------
glm::mat4 Transform::model_matrix() const {
  glm::mat4 matrix(1.0F);
  matrix[0] = _x_axis * _scale.x;
  matrix[1] = _y_axis * _scale.y;
  matrix[2] = _z_axis * _scale.z;
  matrix[3] = _origin;
  return matrix;
}

glm::mat4 Transform::view_matrix() const {
  return TransformGenerator::look_at_matrix(
      glm::vec3(_origin), glm::vec3(_origin - _z_axis), glm::vec3(_y_axis));
}

void Transform::change_orientation(glm::vec3 look_at, glm::vec3 view_up) {
  glm::vec3 direction = glm::vec3(_origin) - look_at;
  if (glm::length(direction) < 1e-6F) {
    std::cerr << "WARNING: Look-at point is too close to the origin." << std::endl;
    return;
  }
  glm::vec3 z = glm::normalize(direction);
  glm::vec3 x = glm::normalize(glm::cross(view_up, z));
  glm::vec3 y = glm::normalize(glm::cross(z, x));

  _x_axis = glm::vec4(x, 0.0F);
  _y_axis = glm::vec4(y, 0.0F);
  _z_axis = glm::vec4(z, 0.0F);
}

void Transform::rotateFPS(float xoffset, float yoffset, bool constrain_pitch) {
  rotate_global_y(-xoffset);
  rotate_local_x(yoffset);
}

void Transform::rotate_global_y(float angle_deg) {
  glm::mat4 rot = TransformGenerator::rotate_3D(glm::vec3(0.0F, 1.0F, 0.0F), angle_deg);

  _x_axis = rot * _x_axis;
  _y_axis = rot * _y_axis;
  _z_axis = rot * _z_axis;

  _x_axis = glm::normalize(_x_axis);
  _y_axis = glm::normalize(_y_axis);
  _z_axis = glm::normalize(_z_axis);
}

void Transform::rotate_local_x(float angle_deg) {
  glm::mat4 rot = TransformGenerator::rotate_3D(glm::vec3(_x_axis), angle_deg);

  _y_axis = rot * _y_axis;
  _z_axis = rot * _z_axis;

  _y_axis = glm::normalize(_y_axis);
  _z_axis = glm::normalize(_z_axis);
}

void Transform::move_local_z(float delta) { _origin += _z_axis * delta; }
void Transform::move_local_x(float delta) { _origin += _x_axis * delta; }
void Transform::move_local_y(float delta) { _origin += _y_axis * delta; }

}  // namespace eng
