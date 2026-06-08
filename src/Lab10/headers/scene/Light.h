#ifndef LIGHT_H
#define LIGHT_H

#pragma once
#include <glm/glm.hpp>

#include "math/Transform.h"
#include "math/TransformGenerator.h"
namespace eng {
class Light : public Transform {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  Light() = default;
  explicit Light(const glm::vec3 &intensity, const glm::vec3 &ambient)
      : _intensity(intensity), _ambient(ambient) {}

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] const glm::vec3 &get_intensity() const noexcept { return _intensity; }
  void set_intensity(const glm::vec3 &intensity) { _intensity = intensity; }
  [[nodiscard]] const glm::vec3 &get_ambient() const noexcept { return _ambient; }
  void set_ambient(const glm::vec3 &ambient) { _ambient = ambient; }

  // ----------------------------------
  // METHODS
  // ----------------------------------
  [[nodiscard]] static glm::mat4 projection_matrix() {
    float near = 0.1;
    float far = 50.0F;
    return TransformGenerator::frustum(-10.0F, 10.0F, -10.0F, 10.0F, near, far);
  }

 private:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  glm::vec3 _intensity{1.0F, 1.0F, 1.0F};
  glm::vec3 _ambient{1.0F, 1.0F, 1.0F};
};
}  // namespace eng

#endif
