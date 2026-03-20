#pragma once

#include <glm/glm.hpp>
#include <utility>
#include <vector>

#include "Global.h"

namespace eng {

enum class color_component : uint8 { red, green, blue };

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
  // Current color
  [[nodiscard]] const glm::vec3 &get_current_color() const noexcept;
  void set_current_color(const glm::vec3 &color) noexcept;
  // Active color component
  [[nodiscard]] color_component get_active_component() const noexcept;
  void set_active_component(color_component component) noexcept;
  // Geometry data
  [[nodiscard]] const std::vector<glm::vec3> &get_vertices() const noexcept;
  [[nodiscard]] const std::vector<glm::vec3> &get_colors() const noexcept;
  [[nodiscard]] const std::vector<uint32> &get_indices() const noexcept;
  // Counts / helpers
  [[nodiscard]] uint32 get_vertex_count() const noexcept;
  [[nodiscard]] uint32 get_indices_count() const noexcept;
  [[nodiscard]] uint32 get_triangle_count() const noexcept;
  [[nodiscard]] bool empty() const noexcept;
  // Dirty flag
  [[nodiscard]] bool is_dirty() const noexcept;
  void clear_dirty() noexcept;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void adjust_active_component(float delta) noexcept;

  void add_point_screen(int32 x, int32 y);
  void add_point_ndc(float x, float y);

  void clear_geometry() noexcept;

 private:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  int32 _width = 0;
  int32 _height = 0;

  glm::vec3 _current_color = glm::vec3(1.0F, 0.0F, 0.0F);
  color_component _active_component = color_component::red;

  std::vector<glm::vec3> _vertices;
  std::vector<glm::vec3> _colors;
  std::vector<uint32> _indices;

  bool _dirty = true;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void _clamp_current_color() noexcept;
  void _mark_dirty() noexcept;
};

}  // namespace eng
