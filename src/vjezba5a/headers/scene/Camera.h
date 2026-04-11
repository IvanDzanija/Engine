#ifndef CAMERA_H
#define CAMERA_H

#pragma once
#include "math/Transform.h"
#include "math/TransformGenerator.h"

namespace eng {

class Camera : public Transform {
 private:
  float _near = 1.0f;
  float _far = 100.0f;
  float _left = -0.5f, _right = 0.5f, _bottom = -0.5f, _top = 0.5f;

 public:
  [[nodiscard]] glm::mat4 perspective_matrix() const {
    return glm::frustum(_left, _right, _bottom, _top, _near, _far);
  }
};
}  // namespace eng
#endif
