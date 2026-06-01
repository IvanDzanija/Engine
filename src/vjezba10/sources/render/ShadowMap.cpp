#include "render/ShadowMap.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
ShadowMap::ShadowMap(uint32 width, uint32 height) : _width(width), _height(height) {
  // Generate framebuffer and depth texture
  glGenFramebuffers(1, &_fbo);
  glGenTextures(1, &_depth_texture_id);

  // Setup depth texture
  glBindTexture(GL_TEXTURE_2D, _depth_texture_id);
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _width, _height, 0,
                 GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);
  }
  glBindTexture(GL_TEXTURE_2D, 0);

  // Setup framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
  {
    // Attach texture to buffer as depth attachment
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
                           _depth_texture_id, 0);
    glDrawBuffer(GL_NONE);  // Disable color buffer
    glReadBuffer(GL_NONE);
  }
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Move constructors
ShadowMap::ShadowMap(ShadowMap &&other) noexcept {
  _fbo = other._fbo;
  _depth_texture_id = other._depth_texture_id;
  _width = other._width;
  _height = other._height;

  other._fbo = 0;
  other._depth_texture_id = 0;
}

ShadowMap &ShadowMap::operator=(ShadowMap &&other) noexcept {
  if (this != &other) {
    glDeleteTextures(1, &_depth_texture_id);
    glDeleteFramebuffers(1, &_fbo);

    _fbo = other._fbo;
    _depth_texture_id = other._depth_texture_id;
    _width = other._width;
    _height = other._height;

    other._fbo = 0;
    other._depth_texture_id = 0;
  }
  return *this;
}

ShadowMap::~ShadowMap() {
  glDeleteTextures(1, &_depth_texture_id);
  glDeleteFramebuffers(1, &_fbo);
}

// ----------------------------------
// METHODS
// ----------------------------------
void ShadowMap::bind_for_writing() const {
  glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
  glViewport(0, 0, _width, _height);
  glClear(GL_DEPTH_BUFFER_BIT);
}

// CARE when using this since the resolution remains as the resolution of the ShadowMap
void ShadowMap::unbind_for_writing() const { glBindFramebuffer(GL_FRAMEBUFFER, 0); }

void ShadowMap::bind_for_reading(uint32 texture_unit) const {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(GL_TEXTURE_2D, _depth_texture_id);
}

}  // namespace eng
