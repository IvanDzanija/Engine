#ifndef TRANSFORM_GENERATOR_H
#define TRANSFORM_GENERATOR_H

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace eng {
class TransformGenerator {
 public:
  static glm::mat4 translate_3D(glm::vec3 t_vec) {
    // clang-format off
    return {
            1.0F, 0.0F, 0.0F, 0.0F,
            0.0F, 1.0F, 0.0F, 0.0F,
            0.0F, 0.0F, 1.0F, 0.0F,
            t_vec.x,  t_vec.y,  t_vec.z,  1.0F
    };
    // clang-format on
  }

  static glm::mat4 scale_3D(glm::vec3 s_vec) {
    // clang-format off
    return {
            s_vec.x, 0.0F, 0.0F, 0.0F,
            0.0F, s_vec.y, 0.0F, 0.0F,
            0.0F, 0.0F, s_vec.z, 0.0F,
            0.0F,  0.0F,  0.0F, 1.0F
    };
    // clang-format on
  }

  static glm::mat4 rotate_3D(glm::vec3 axis, float deg_angle) {
    float rad_angle = glm::radians(deg_angle);
    float cos_angle = glm::cos(rad_angle);
    float sin_angle = glm::sin(rad_angle);
    axis = glm::normalize(axis);

    glm::mat4 mat(1.0F);
    mat[0][0] = cos_angle + (axis.x * axis.x * (1.0F - cos_angle));
    mat[0][1] = (axis.x * axis.y * (1.0F - cos_angle)) - (axis.z * sin_angle);
    mat[0][2] = (axis.x * axis.z * (1.0F - cos_angle)) + (axis.y * sin_angle);
    mat[1][0] = (axis.y * axis.x * (1.0F - cos_angle)) + (axis.z * sin_angle);
    mat[1][1] = cos_angle + (axis.y * axis.y * (1.0F - cos_angle));
    mat[1][2] = (axis.y * axis.z * (1.0F - cos_angle)) - (axis.x * sin_angle);
    mat[2][0] = (axis.z * axis.x * (1.0F - cos_angle)) - (axis.y * sin_angle);
    mat[2][1] = (axis.z * axis.y * (1.0F - cos_angle)) + (axis.x * sin_angle);
    mat[2][2] = cos_angle + (axis.z * axis.z * (1.0F - cos_angle));

    return mat;
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
