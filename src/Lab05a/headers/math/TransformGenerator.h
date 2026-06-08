#ifndef TRANSFORM_GENERATOR_H
#define TRANSFORM_GENERATOR_H

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace eng {
class TransformGenerator {
 public:
  static glm::mat4 translate_3D(glm::vec3 translate_vector) {
    return glm::translate(glm::mat4(1.0F), translate_vector);
  }

  static glm::mat4 scale_3D(glm::vec3 scale_vector) {
    return glm::scale(glm::mat4(1.0F), scale_vector);
  }

  static glm::mat4 rotate_3D(glm::vec3 axis, float deg_angle) {
    return glm::rotate(glm::mat4(1.0F), glm::radians(deg_angle), axis);
  }

  static glm::mat4 look_at_matrix(glm::vec3 eye, glm::vec3 center, glm::vec3 view_up) {
    return glm::lookAt(eye, center, view_up);
  }

  static glm::mat4 frustum(float left, float right, float bottom, float top, float near,
                           float far) {
    return glm::frustum(left, right, bottom, top, near, far);
  }
};
}  // namespace eng

#endif
