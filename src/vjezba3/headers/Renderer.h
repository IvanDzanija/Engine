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
  explicit Renderer(Shader &uniform_color_shader);
  ~Renderer();

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void render(const AppState &state) const;

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
  GLuint _color_indicator_vao = 0;
  GLuint _color_indicator_vbo = 0;
  GLuint _color_indicator_ebo = 0;
  // ----------------------------------
  // METHODS
  // ----------------------------------
  void _setup_color_indicator();
  void _draw_color_indicator(const AppState &state) const;

  Shader *_load_uniform_color_shader(char *path);

  Shader &_shader;
};

}  // namespace eng
