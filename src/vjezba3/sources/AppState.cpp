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
// Current color
[[nodiscard]] const glm::vec3 &AppState::get_current_color() const noexcept {
  return _current_color;
}
void AppState::set_current_color(const glm::vec3 &color) noexcept {
  _current_color = color;
  _clamp_current_color();
  _mark_dirty();
}
// Active color component
[[nodiscard]] color_component AppState::get_active_component() const noexcept {
  return _active_component;
}
void AppState::set_active_component(color_component component) noexcept {
  _active_component = component;
}
// Geometry data
[[nodiscard]] const std::vector<glm::vec3> &AppState::get_vertices() const noexcept {
  return _vertices;
}
[[nodiscard]] const std::vector<glm::vec3> &AppState::get_colors() const noexcept {
  return _colors;
}
[[nodiscard]] const std::vector<uint32> &AppState::get_indices() const noexcept {
  return _indices;
}
// Counts / helpers
[[nodiscard]] uint32 AppState::get_vertex_count() const noexcept {
  return static_cast<uint32>(_vertices.size());
}
[[nodiscard]] uint32 AppState::get_indices_count() const noexcept {
  return static_cast<uint32>(_indices.size());
}
[[nodiscard]] uint32 AppState::get_triangle_count() const noexcept {
  return get_indices_count() / 3;
}
[[nodiscard]] bool AppState::empty() const noexcept {
  return _vertices.empty() || _colors.empty() || _indices.empty();
}
// Dirty flag
[[nodiscard]] bool AppState::is_dirty() const noexcept { return _dirty; }
void AppState::clear_dirty() noexcept { _dirty = false; }

// ----------------------------------
// METHODS
// ----------------------------------
void AppState::adjust_active_component(float delta) noexcept {
  switch (_active_component) {
    case color_component::red:
      _current_color.r += delta;
      break;
    case color_component::green:
      _current_color.g += delta;
      break;
    case color_component::blue:
      _current_color.b += delta;
      break;
  }

  _clamp_current_color();
  _mark_dirty();
}

void AppState::add_point_screen(int32 x, int32 y) {
  if (_width <= 0 || _height <= 0) {
    return;
  }
  add_point_ndc(_normalize_coords(x, y, 0));
}

void AppState::add_point_ndc(const glm::vec3 coords) {
  _vertices.push_back(coords);
  _colors.push_back(_current_color);

  const auto point_count = static_cast<uint32>(_vertices.size());

  if (point_count >= 3) {
    _indices.push_back(point_count - 3);
    _indices.push_back(point_count - 2);
    _indices.push_back(point_count - 1);
  }

  _mark_dirty();
}

void AppState::clear_geometry() noexcept {
  _vertices.clear();
  _colors.clear();
  _indices.clear();
  _mark_dirty();
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
void AppState::_clamp_current_color() noexcept {
  _current_color.r = std::clamp(_current_color.r, 0.0F, 1.0F);
  _current_color.g = std::clamp(_current_color.g, 0.0F, 1.0F);
  _current_color.b = std::clamp(_current_color.b, 0.0F, 1.0F);
}

void AppState::_mark_dirty() noexcept { _dirty = true; }

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
