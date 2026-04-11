#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <vector>

#include "Global.h"

namespace eng {

class AppState {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  AppState() = default;
  AppState(int32 width, int32 height);

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  // Width
  [[nodiscard]] int32 get_width() const noexcept;
  void set_width(int32 width) noexcept;
  // Height
  [[nodiscard]] int32 get_height() const noexcept;
  void set_height(int32 height) noexcept;
  // Framebuffer size
  [[nodiscard]] std::pair<int32, int32> get_framebuffer_size() const noexcept;
  void set_framebuffer_size(int32 width, int32 height) noexcept;
  // Cursor position
  [[nodiscard]] glm::vec3 get_cursor_position() const noexcept;
  void set_cursor_position(int32 x, int32 y) noexcept;
  // Scale factors
  [[nodiscard]] float get_xscale() const noexcept;
  void set_xscale(float xscale) noexcept;
  [[nodiscard]] float get_yscale() const noexcept;
  void set_yscale(float yscale) noexcept;
  [[nodiscard]] std::pair<float, float> get_scale_factors() const noexcept;
  void set_scale_factors(float xscale, float yscale) noexcept;

 private:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  int32 _width = 0;
  int32 _height = 0;
  float _xscale = 1.0F;
  float _yscale = 1.0F;

  glm::vec3 _cursor_position = glm::vec3(0.0F, 0.0F, 0.0F);
};

}  // namespace eng
