#ifndef CAMERA_H
#define CAMERA_H

#pragma once
#include "math/Transform.h"
#include "math/TransformGenerator.h"

namespace eng {

class Camera : public Transform {
 private:
  float _near = 1.0F, _far = 100.0F;
  float _left = -0.5F, _right = 0.5F, _bottom = -0.5F, _top = 0.5F;

 public:
  [[nodiscard]] glm::mat4 perspective_matrix() const {
    return TransformGenerator::frustum(_left, _right, _bottom, _top, _near, _far);
  }
};
}  // namespace eng
#endif
