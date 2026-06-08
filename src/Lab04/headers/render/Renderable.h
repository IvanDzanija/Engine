#ifndef RENDERABLE_H
#define RENDERABLE_H

#pragma once
#include <glad/glad.h>

namespace eng {
class Renderable {
 public:
  virtual ~Renderable() = default;
  virtual void draw() const = 0;

  Renderable() = default;
  Renderable(Renderable &&other) noexcept : _vao(other._vao) { other._vao = 0; }
  Renderable &operator=(Renderable &&other) noexcept {
    if (this != &other) {
      _vao = other._vao;
      other._vao = 0;
    }
    return *this;
  }
  Renderable(const Renderable &) = delete;
  Renderable &operator=(const Renderable &) = delete;

 protected:
  GLuint _vao = 0;
};
}  // namespace eng

#endif
