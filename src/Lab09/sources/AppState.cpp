#pragma once
#include "AppState.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
AppState::AppState(int32 width, int32 height) : _width(width), _height(height) {}

// ----------------------------------
// GETTERS & SETTERS
// ----------------------------------
// Width
[[nodiscard]] int32 AppState::get_width() const noexcept { return _width; }
void AppState::set_width(int32 width) noexcept { _width = width; }
// Height
[[nodiscard]] int32 AppState::get_height() const noexcept { return _height; }
void AppState::set_height(int32 height) noexcept { _height = height; }
// Framebuffer size
[[nodiscard]] std::pair<int32, int32> AppState::get_framebuffer_size() const noexcept {
  return {_width, _height};
}
void AppState::set_framebuffer_size(int32 width, int32 height) noexcept {
  _width = width;
  _height = height;
}

// Cursor position
[[nodiscard]] glm::vec3 AppState::get_cursor_position() const noexcept {
  return _cursor_position;
}
void AppState::set_cursor_position(int32 x, int32 y) noexcept {
  _cursor_position = _normalize_coords(x, y, 0);
}
// Scale factors
[[nodiscard]] float AppState::get_xscale() const noexcept { return _xscale; }
void AppState::set_xscale(float xscale) noexcept { _xscale = xscale; }
[[nodiscard]] float AppState::get_yscale() const noexcept { return _yscale; }
void AppState::set_yscale(float yscale) noexcept { _yscale = yscale; }
[[nodiscard]] std::pair<float, float> AppState::get_scale_factors() const noexcept {
  return {_xscale, _yscale};
}
void AppState::set_scale_factors(float xscale, float yscale) noexcept {
  _xscale = xscale;
  _yscale = yscale;
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
glm::vec3 AppState::_normalize_coords(int32 x, int32 y, int32 z) const noexcept {
  // Scale
  glm::vec3 coords(static_cast<float>(x) * _xscale, static_cast<float>(y) * _yscale,
                   static_cast<float>(z));
  // Invert y-axis and convert to NDC
  coords.x = (2.0F * coords.x / static_cast<float>(_width)) - 1.0F;
  coords.y =
      (2.0F * (static_cast<float>(_height) - coords.y) / static_cast<float>(_height)) -
      1.0F;
  return coords;
}
}  // namespace eng
