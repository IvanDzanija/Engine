#include "render/Axis.h"

#include "render/Renderable.h"

namespace eng {

// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Axis::Axis() {
  // Init buffers
  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);

  glBindVertexArray(_vao);
  {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(_data), _data.data(), GL_STATIC_DRAW);

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }
  glBindVertexArray(0);
}

Axis::~Axis() {
  glDeleteBuffers(1, &_vbo);
  glDeleteVertexArrays(1, &_vao);
}

// ----------------------------------
// METHODS
// ----------------------------------
void Axis::draw(std::shared_ptr<Shader> shader) const {
  glDisable(GL_DEPTH_TEST);
  glBindVertexArray(_vao);
  glDrawArrays(GL_LINES, 0, 6);
  glBindVertexArray(0);
  glEnable(GL_DEPTH_TEST);
}

RenderableType Axis::get_type() const { return RenderableType::AXIS; }

}  // namespace eng
