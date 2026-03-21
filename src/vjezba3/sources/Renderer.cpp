#pragma once
#include "Renderer.h"

#include "Graphics.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Renderer::Renderer(Shader &color_indicator_shader, Shader &triangles_shader)
    : _color_indicator_shader(color_indicator_shader),
      _triangles_shader(triangles_shader) {
  _setup_color_indicator();
  _setup_triangles();
  _setup_preview_triangle();
}
Renderer::~Renderer() {
  glDeleteBuffers(1, &_color_indicator_vbo);
  glDeleteVertexArrays(1, &_color_indicator_vao);
  glDeleteBuffers(1, &_triangles_vertices_vbo);
  glDeleteBuffers(1, &_triangles_colors_vbo);
  glDeleteBuffers(1, &_triangles_ebo);
  glDeleteVertexArrays(1, &_triangles_vao);
};

// ----------------------------------
// METHODS
// ----------------------------------
void Renderer::render(AppState &state) const {
  _draw_color_indicator(state);
  _draw_triangles(state);
};

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
// Color indicator
void Renderer::_setup_color_indicator() {
  glGenVertexArrays(1, &_color_indicator_vao);
  glGenBuffers(1, &_color_indicator_vbo);

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
  _color_indicator_shader.use();
  _color_indicator_shader.set_uniform("u_color", state.get_current_color());
  // auto eye = glm::mat4(1);
  // _color_indicator_shader.set_uniform("transform_matrix", eye);

  glBindVertexArray(_color_indicator_vao);
  {
    glDrawArrays(GL_TRIANGLE_FAN, 0, COLOR_INDICATOR_VERTICES_COUNT);
  }
  glBindVertexArray(0);
}

// Triangles
void Renderer::_setup_triangles() {
  glGenVertexArrays(1, &_triangles_vao);
  glGenBuffers(1, &_triangles_vertices_vbo);
  glGenBuffers(1, &_triangles_colors_vbo);
  glGenBuffers(1, &_triangles_ebo);

  // Triangles VAO
  glBindVertexArray(_triangles_vao);
  {
    // Vertices VBO
    glBindBuffer(GL_ARRAY_BUFFER, _triangles_vertices_vbo);
    {
      glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
      glEnableVertexAttribArray(0);
    }
    // Colors VBO
    glBindBuffer(GL_ARRAY_BUFFER, _triangles_colors_vbo);
    {
      glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
      glEnableVertexAttribArray(1);
    }
    // Triangles EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangles_ebo);
    {
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    }
  }
  glBindVertexArray(0);
}

void Renderer::_update_triangles(AppState &state) const {
  glBindVertexArray(_triangles_vao);
  {
    // Vertices VBO
    glBindBuffer(GL_ARRAY_BUFFER, _triangles_vertices_vbo);
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * state.get_vertex_count() * 3,
                   state.get_vertices().data(), GL_DYNAMIC_DRAW);
      std::println("Updated triangles with {} vertices", state.get_vertex_count());
    }
    // Colors VBO
    glBindBuffer(GL_ARRAY_BUFFER, _triangles_colors_vbo);
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * state.get_vertex_count() * 3,
                   state.get_colors().data(), GL_DYNAMIC_DRAW);
      std::println("Updated triangles with {} vertices", state.get_vertex_count());
    }
    // Triangles EBO
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _triangles_ebo);
    //{
    //  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32) *
    //  state.get_indices_count(),
    //               state.get_indices().data(), GL_DYNAMIC_DRAW);
    //  std::println("Updated triangles with {} indices", state.get_indices_count());
    //}
  }
  glBindVertexArray(0);
  state.clear_dirty();
}

// Preview triangle
void Renderer::_setup_preview_triangle() {
  glGenVertexArrays(1, &_preview_triangle_vao);
  glGenBuffers(1, &_preview_triangle_vertices_vbo);
  glGenBuffers(1, &_preview_triangle_colors_vbo);

  // Preview VAO
  glBindVertexArray(_preview_triangle_vao);
  {
    // Vertices VBO
    glBindBuffer(GL_ARRAY_BUFFER, _preview_triangle_vertices_vbo);
    {
      // 3 vertices x 3 coordinates
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 3, nullptr, GL_DYNAMIC_DRAW);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
      glEnableVertexAttribArray(0);
    }
    // Colors VBO
    glBindBuffer(GL_ARRAY_BUFFER, _preview_triangle_colors_vbo);
    {
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 3, nullptr, GL_DYNAMIC_DRAW);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
      glEnableVertexAttribArray(1);
    }
  }
  glBindVertexArray(0);
}
void Renderer::_update_preview_triangle(const AppState &state) const {
  if (state.get_vertex_count() < 2) {
    return;  // Need at least 2 vertices to form a triangle
  }

  std::array<glm::vec3, 3> preview_vertices;
  preview_vertices[0] = state.get_vertices()[state.get_vertex_count() - 2];
  preview_vertices[1] = state.get_vertices()[state.get_vertex_count() - 1];
  preview_vertices[2] = state.get_cursor_position();
  std::array<glm::vec3, 3> preview_colors;
  preview_colors[0] = state.get_colors()[state.get_vertex_count() - 2];
  preview_colors[1] = state.get_colors()[state.get_vertex_count() - 1];
  preview_colors[2] = state.get_current_color();

  std::cout << "Preview triangle vertices: " << std::endl;
  for (const auto &vertex : preview_vertices) {
    std::cout << "\t(" << vertex.x << ", " << vertex.y << ", " << vertex.z << ")"
              << std::endl;
  }

  std::cout << "Preview triangle colors: " << std::endl;
  for (const auto &color : preview_colors) {
    std::cout << "\t(" << color.r << ", " << color.g << ", " << color.b << ")"
              << std::endl;
  }

  // Preview VAO
  glBindVertexArray(_preview_triangle_vao);
  {
    // Vertices VBO
    glBindBuffer(GL_ARRAY_BUFFER, _preview_triangle_vertices_vbo);
    {
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * 3,
                      preview_vertices.data());
    }
    // Colors VBO
    glBindBuffer(GL_ARRAY_BUFFER, _preview_triangle_colors_vbo);
    {
      glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 3 * 3, preview_colors.data());
    }
  }
}

void Renderer::_draw_triangles(AppState &state) const {
  if (state.is_dirty()) {
    _update_triangles(state);
  }
  _update_preview_triangle(state);
  _triangles_shader.use();

  glBindVertexArray(_triangles_vao);
  {
    // glDrawElements(GL_TRIANGLE_STRIP, state.get_indices_count(), GL_UNSIGNED_INT,
    //                nullptr);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, state.get_vertex_count());
    // glDrawElements(GL_TRIANGLES, state.get_indices_count(), GL_UNSIGNED_INT,
    // nullptr);
  }
  glBindVertexArray(0);
  glBindVertexArray(_preview_triangle_vao);
  {
    glDrawArrays(GL_TRIANGLES, 0, 3);
  }

  glBindVertexArray(0);
}

}  // namespace eng
