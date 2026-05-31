#ifndef GLOBAL_H
#define GLOBAL_H

#pragma once
#include <cstdint>
#include <iostream>
#include <numbers>
#include <print>

#include "glm/glm.hpp"

using int8 = int8_t;
using uint8 = uint8_t;
using int16 = int16_t;
using uint16 = uint16_t;
using int32 = int32_t;
using uint32 = uint32_t;
using int64 = int64_t;
using uint64 = uint64_t;
using char8 = char8_t;
using char16 = char16_t;

namespace eng {
inline void matrix_print(const glm::mat4 &matrix) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << matrix[i][j] << " ";
    }
    std::cout << std::endl;
  }
}

inline void vector_print(const glm::vec3 &vector) {
  std::cout << vector.x << " " << vector.y << " " << vector.z << std::endl;
}
inline void vector_print(const glm::vec4 &vector) {
  std::cout << vector.x << " " << vector.y << " " << vector.z << " " << vector.w
            << std::endl;
}

}  // namespace eng

#define CHECK_GL()                  \
  while (GLenum err = glGetError()) \
    std::cout << "GL error at line " << __LINE__ << ": " << err << std::endl;

#endif
