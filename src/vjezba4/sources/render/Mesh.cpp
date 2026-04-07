#include "render/Mesh.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
// Copy constructor
Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32> &indices,
           const std::vector<Texture> &textures)
    : _vertices(vertices), _indices(indices), _textures(textures) {
  _setup_mesh();
}
// Move constructor
Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<uint32> &&indices,
           std::vector<Texture> &&textures)
    : _vertices(std::move(vertices)),
      _indices(std::move(indices)),
      _textures(std::move(textures)) {
  _setup_mesh();
}
// Destructor
Mesh::~Mesh() {
  glDeleteVertexArrays(1, &_vao);
  glDeleteBuffers(1, &_vbo);
  glDeleteBuffers(1, &_ebo);
}

// ----------------------------------
// GETTERS & SETTERS
// ----------------------------------
[[nodiscard]] const std::vector<Vertex> &Mesh::get_vertices() const noexcept {
  return _vertices;
}
[[nodiscard]] const std::vector<uint32> &Mesh::get_indices() const noexcept {
  return _indices;
}
[[nodiscard]] const std::vector<Texture> &Mesh::get_textures() const noexcept {
  return _textures;
}

// ----------------------------------
// METHODS
// ----------------------------------
void Mesh::draw() const {
  glBindVertexArray(_vao);
  glDrawElements(GL_TRIANGLES, _indices.size(), GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
void Mesh::_setup_mesh() {
  glGenVertexArrays(1, &_vao);
  glGenBuffers(1, &_vbo);
  glGenBuffers(1, &_ebo);

  glBindVertexArray(_vao);
  {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    {
      glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(),
                   GL_STATIC_DRAW);
      // Vertex coordinates
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
      // Vertex normals
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void *)offsetof(Vertex, normal));
      // Vertex texture coords
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                            (void *)offsetof(Vertex, tex_coords));
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ebo);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32),
                   _indices.data(), GL_STATIC_DRAW);
    }
  }

  glBindVertexArray(0);
}

}  // namespace eng
