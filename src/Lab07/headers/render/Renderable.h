#ifndef RENDERABLE_H
#define RENDERABLE_H

#pragma once
#include <glad/glad.h>

namespace eng {
class Renderable {
 public:
  virtual ~Renderable() = default;
  virtual void set_draw_mode(GLenum mode) { _draw_mode = mode; }
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
  GLenum _draw_mode = GL_TRIANGLES;
};
}  // namespace eng

#endif
