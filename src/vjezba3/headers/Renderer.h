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
  explicit Renderer() = default;
  explicit Renderer(std::weak_ptr<Shader> uniform_color_shader);
  ~Renderer();

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void render(const AppState &state) const;

 private:
  // ----------------------------------
  // METHODS
  // ----------------------------------
  void _setup_color_indicator();
  void _draw_color_indicator(const AppState &state) const;

  Shader *_load_uniform_color_shader(char *path);

  // ----------------------------------
  // FIELDS
  // ----------------------------------
  uint32 _color_indicator_vao = 0;
  uint32 _color_indicator_vbo = 0;

  Shader *_uniform_color_shader = nullptr;
};

}  // namespace eng
