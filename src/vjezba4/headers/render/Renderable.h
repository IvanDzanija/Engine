#ifndef RENDERABLE_H
#define RENDERABLE_H

#pragma once
#include <glad/glad.h>

namespace eng {
class Renderable {
 public:
  virtual ~Renderable() = default;
  virtual void draw() const = 0;

 protected:
  GLuint _vao = 0;
};
}  // namespace eng

#endif
