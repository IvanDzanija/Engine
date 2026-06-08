#include "render/Curve.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
// Copy constructor
Curve::Curve(const std::vector<CurveVertex> &vertices) : _vertices(vertices) {
  _setup_curve();
}
// Move constructor
Curve::Curve(std::vector<CurveVertex> &&vertices) : _vertices(std::move(vertices)) {
  _setup_curve();
}

Curve::Curve(Curve &&other) noexcept
    : _vertices(std::move(other._vertices)), _vbo(other._vbo) {
  _vao = other._vao;
  other._vao = 0;
  other._vbo = 0;
}
Curve &Curve::operator=(Curve &&other) noexcept {
  if (this != &other) {
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);

    // Move data
    _vertices = std::move(other._vertices);
    _vao = other._vao;
    _vbo = other._vbo;

    // Reset source
    other._vao = 0;
    other._vbo = 0;
  }
  return *this;
}

// Destructor
Curve::~Curve() {
  glDeleteVertexArrays(1, &_vao);
  glDeleteBuffers(1, &_vbo);
}

// ----------------------------------
// GETTERS & SETTERS
// ----------------------------------
[[nodiscard]] const std::vector<CurveVertex> &Curve::get_vertices() const noexcept {
  return _vertices;
}

// ----------------------------------
// METHODS
// ----------------------------------
void Curve::draw() const {
  glBindVertexArray(_vao);
  glDrawArrays(_draw_mode, 0, _vertices.size());
  glBindVertexArray(0);
}

RenderableType Curve::get_type() const { return RenderableType::CURVE; }

void Curve::update_vertices(std::vector<CurveVertex> &&vertices) {
  _vertices = std::move(vertices);
  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  {
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(CurveVertex),
                 _vertices.data(), GL_STATIC_DRAW);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
void Curve::_setup_curve() {
  _draw_mode = GL_LINE_STRIP;
  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);

  glBindVertexArray(_vao);
  {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    {
      glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(CurveVertex),
                   _vertices.data(), GL_STATIC_DRAW);
      // Vertex coordinates
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CurveVertex), nullptr);
      // Vertex colors
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(CurveVertex),
                            (void *)offsetof(CurveVertex, color));
    }
  }

  glBindVertexArray(0);
}

}  // namespace eng
