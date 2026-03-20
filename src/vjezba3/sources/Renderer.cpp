#pragma once
#include "Renderer.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Renderer::Renderer(Shader &shader) : _shader(shader) { _setup_color_indicator(); }
Renderer::~Renderer() {
  glDeleteBuffers(1, &_color_indicator_vbo);
  glDeleteBuffers(1, &_color_indicator_ebo);
  glDeleteVertexArrays(1, &_color_indicator_vao);
};

// ----------------------------------
// METHODS
// ----------------------------------
void Renderer::render(const AppState &state) const { _draw_color_indicator(state); };

void Renderer::_setup_color_indicator() {
  glGenVertexArrays(1, &_color_indicator_vao);
  glGenBuffers(1, &_color_indicator_vbo);
  glGenBuffers(1, &_color_indicator_ebo);

  glBindVertexArray(_color_indicator_vao);
  {
    // Load vertex data into vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, _color_indicator_vbo);
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * COLOR_INDICATOR_VERTICES_COUNT * 3,
                   COLOR_INDICATOR_VERTICES.data(), GL_STATIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
      glEnableVertexAttribArray(0);
    }
  }
  glBindVertexArray(0);
}
void Renderer::_draw_color_indicator(const AppState &state) const {
  // nacrtaj kvadrat u gornjem lijevom kutu koji pokazuje trenutnu boju
  _shader.use();
  _shader.set_uniform("u_color", state.get_current_color());
  auto eye = glm::mat4(1);
  _shader.set_uniform("transform_matrix", eye);

  glBindVertexArray(_color_indicator_vao);
  {
    glDrawArrays(GL_TRIANGLE_FAN, 0, COLOR_INDICATOR_VERTICES_COUNT);
  }
  glBindVertexArray(0);
}

}  // namespace eng
