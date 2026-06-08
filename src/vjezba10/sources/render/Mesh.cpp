#include "render/Mesh.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
// Copy constructor
Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<uint32> &indices,
           const std::vector<std::shared_ptr<Texture>> &textures, uint32 material_index)
    : _vertices(vertices),
      _indices(indices),
      _textures(textures),
      _material_index(material_index) {
  _setup_mesh();
}
// Move constructor
Mesh::Mesh(std::vector<Vertex> &&vertices, std::vector<uint32> &&indices,
           std::vector<std::shared_ptr<Texture>> &&textures, uint32 material_index)
    : _vertices(std::move(vertices)),
      _indices(std::move(indices)),
      _textures(std::move(textures)),
      _material_index(material_index) {
  _setup_mesh();
}

// Move constructor
Mesh::Mesh(Mesh &&other) noexcept
    : _vertices(std::move(other._vertices)),
      _indices(std::move(other._indices)),
      _textures(std::move(other._textures)),
      _material_index(other._material_index),
      _vbo(other._vbo),
      _ebo(other._ebo) {
  _vao = other._vao;
  other._vao = 0;
  other._vbo = 0;
  other._ebo = 0;
}

// Move assignment operator
Mesh &Mesh::operator=(Mesh &&other) noexcept {
  if (this != &other) {
    // Clean up current resources
    glDeleteVertexArrays(1, &_vao);
    glDeleteBuffers(1, &_vbo);
    glDeleteBuffers(1, &_ebo);

    // Move data
    _vertices = std::move(other._vertices);
    _indices = std::move(other._indices);
    _textures = std::move(other._textures);
    _material_index = other._material_index;
    _vao = other._vao;
    _vbo = other._vbo;
    _ebo = other._ebo;

    // Reset source
    other._material_index = 0;
    other._vao = 0;
    other._vbo = 0;
    other._ebo = 0;
  }
  return *this;
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
[[nodiscard]] const std::vector<std::shared_ptr<Texture>> &Mesh::get_textures()
    const noexcept {
  return _textures;
}
[[nodiscard]] uint32 Mesh::get_material_index() const noexcept {
  return _material_index;
}

// ----------------------------------
// METHODS
// ----------------------------------
void Mesh::draw(const std::shared_ptr<Shader> &shader) const {
  size_t diffuse_count = 0;
  size_t specular_count = 0;
  // Texture binding
  for (size_t i = 0, t = 1; i < _textures.size(); ++i, ++t) {
    glActiveTexture(GL_TEXTURE0 + t);  // Texture unit 0 is reserved for shadow map
    std::string number;
    std::string name = _textures[i]->get_type();
    if (name == "diffuse") {
      number = std::to_string(++diffuse_count);

    } else if (name == "specular") {
      number = std::to_string(++specular_count);
    }

    std::string uniform_name = "u_material.";
    uniform_name += name + number;
    shader->set_uniform(uniform_name, static_cast<int>(t));
    glBindTexture(GL_TEXTURE_2D, _textures[i]->get_id());
  }
  glActiveTexture(GL_TEXTURE0);

  // Mesh
  glBindVertexArray(_vao);
  glDrawElements(_draw_mode, _indices.size(), GL_UNSIGNED_INT, nullptr);
  glBindVertexArray(0);
}

RenderableType Mesh::get_type() const { return RenderableType::MESH; }

BoundingBox Mesh::get_bounding_box() const {
  glm::vec3 vmin = _vertices[0].coords;
  glm::vec3 vmax = _vertices[0].coords;

  for (const auto &vert : _vertices) {
    const auto &v = vert.coords;
    vmin = glm::min(vmin, v);
    vmax = glm::max(vmax, v);
  }
  return {.min = vmin, .max = vmax};
}

void Mesh::apply_transform(const glm::mat4 &matrix) {
  for (auto &vert : _vertices) {
    auto &v = vert.coords;
    v = glm::vec3(matrix * glm::vec4(v, 1.0F));
  }

  glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  // TODO: I dont think we need to copy over the entire vertex struct if only positions
  // changed, but this is simpler
  glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(),
               GL_STATIC_DRAW);
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
void Mesh::_setup_mesh() {
  _draw_mode = GL_TRIANGLES;
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
