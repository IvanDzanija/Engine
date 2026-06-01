#ifndef SHADOWMAP_H
#define SHADOWMAP_H

#pragma once
#include <glad/glad.h>

#include "Global.h"

namespace eng {
class ShadowMap {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  ShadowMap(uint32 width = 1024, uint32 height = 1024);
  // Copy constructors
  ShadowMap(const ShadowMap &) = delete;
  ShadowMap &operator=(const ShadowMap &) = delete;
  // Move constructors
  ShadowMap(ShadowMap &&other) noexcept;
  ShadowMap &operator=(ShadowMap &&other) noexcept;
  ~ShadowMap();

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void bind_for_writing() const;
  void unbind_for_writing() const;
  void bind_for_reading(uint32 texture_unit) const;

 protected:
  GLuint _fbo = 0;
  GLuint _depth_texture_id = 0;
  uint32 _width;
  uint32 _height;
};
}  // namespace eng

#endif
