#pragma once
#include <memory>

#include "AppState.h"
#include "Global.h"
#include "Shader.h"

namespace eng {

class Renderer {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  explicit Renderer(Shader &color_indicator_shader, Shader &triangles_shader);
  ~Renderer();

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void render(AppState &state) const;

 private:
  // ----------------------------------
  // CONSTANTS
  // ----------------------------------
  static constexpr uint8 COLOR_INDICATOR_VERTICES_COUNT = 7;
  // clang-format off
  static constexpr std::array<float, COLOR_INDICATOR_VERTICES_COUNT * 3>
      COLOR_INDICATOR_VERTICES = {
          -1.0f, 0.0f, 0.0f,
           0.0f, 0.0f, 0.0f,
           0.0f, 1.0f, 0.0f,
           0.0f, 1.0f, 0.0f,
          -1.0f, 1.0f, 0.0f
          -1.0f, 0.0f, 0.0f
  };
  // clang-format on

  // ----------------------------------
  // FIELDS
  // ----------------------------------
  // Color indicator
  Shader &_color_indicator_shader;
  GLuint _color_indicator_vao = 0;
  GLuint _color_indicator_vbo = 0;
  // Triangles
  Shader &_triangles_shader;
  GLuint _triangles_vao = 0;
  GLuint _triangles_vertices_vbo = 0;
  GLuint _triangles_colors_vbo = 0;
  GLuint _triangles_ebo = 0;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void _setup_color_indicator();
  void _draw_color_indicator(const AppState &state) const;

  void _setup_triangles();
  void _update_triangles(AppState &state) const;
  void _draw_triangles(AppState &state) const;
};

}  // namespace eng
