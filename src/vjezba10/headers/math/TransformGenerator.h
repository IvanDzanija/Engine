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
    mat[1][0] = (axis.x * axis.y * (1.0F - cos_angle)) - (axis.z * sin_angle);
    mat[2][0] = (axis.x * axis.z * (1.0F - cos_angle)) + (axis.y * sin_angle);
    mat[0][1] = (axis.y * axis.x * (1.0F - cos_angle)) + (axis.z * sin_angle);
    mat[1][1] = cos_angle + (axis.y * axis.y * (1.0F - cos_angle));
    mat[2][1] = (axis.y * axis.z * (1.0F - cos_angle)) - (axis.x * sin_angle);
    mat[0][2] = (axis.z * axis.x * (1.0F - cos_angle)) - (axis.y * sin_angle);
    mat[1][2] = (axis.z * axis.y * (1.0F - cos_angle)) + (axis.x * sin_angle);
    mat[2][2] = cos_angle + (axis.z * axis.z * (1.0F - cos_angle));

    return mat;
  }

  static glm::mat4 look_at_matrix(glm::vec3 eye, glm::vec3 center, glm::vec3 view_up) {
    glm::vec3 f = glm::normalize(center - eye);
    glm::vec3 s = glm::normalize(glm::cross(f, view_up));
    glm::vec3 u = glm::cross(s, f);

    glm::mat4 T = translate_3D(-eye);

    glm::mat4 R(1.0F);
    R[0][0] = s.x;
    R[1][0] = s.y;
    R[2][0] = s.z;
    R[0][1] = u.x;
    R[1][1] = u.y;
    R[2][1] = u.z;
    R[0][2] = -f.x;
    R[1][2] = -f.y;
    R[2][2] = -f.z;

    return R * T;
  }

  static glm::mat4 frustum(float left, float right, float bottom, float top, float near,
                           float far) {
    glm::mat4 mat(0.0F);
    mat[0][0] = (2.0F * near) / (right - left);
    mat[1][1] = (2.0F * near) / (top - bottom);
    mat[2][0] = (right + left) / (right - left);
    mat[2][1] = (top + bottom) / (top - bottom);
    mat[2][2] = -(far + near) / (far - near);
    mat[2][3] = -1.0F;
    mat[3][2] = -(2.0F * far * near) / (far - near);
    return mat;
  }

  static glm::mat4 ortho(float left, float right, float bottom, float top, float near,
                         float far) {
    glm::mat4 mat(1.0F);
    mat[0][0] = 2.0F / (right - left);
    mat[1][1] = 2.0F / (top - bottom);
    mat[2][2] = -2.0F / (far - near);
    mat[3][0] = -(right + left) / (right - left);
    mat[3][1] = -(top + bottom) / (top - bottom);
    mat[3][2] = -(far + near) / (far - near);
    return mat;
  }
};
}  // namespace eng

#endif
