#pragma once
#include "scene/Object.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Object::Object(std::shared_ptr<Shader> shader) : _shader(std::move(shader)) {}
Object::Object(std::vector<std::shared_ptr<Renderable>> meshes,
               std::shared_ptr<Shader> shader)
    : _renderables(std::move(meshes)), _shader(std::move(shader)) {}

// ----------------------------------
// METHODS
// ----------------------------------
void Object::render(const glm::mat4 &projection_matrix,
                    const glm::mat4 &view_matrix) const {
  if (!_shader) {
    std::cerr << "ERROR: Object cannot be rendered without a shader or renderables."
              << std::endl;
    return;
  }

  if (_renderables.empty()) {
    std::cerr << "WARNING: Object has no renderables to draw." << std::endl;
  }

  _shader->use();
  _shader->set_uniform("u_projection", projection_matrix);
  _shader->set_uniform("u_view", view_matrix);
  _shader->set_uniform("u_model", model_matrix());
  std::println("Projection matrix:");
  matrix_print(projection_matrix);
  std::println("View matrix:");
  matrix_print(view_matrix);
  std::println("Model matrix:");
  matrix_print(model_matrix());

  // Draw object
  _shader->set_uniform("u_use_vertex_color", false);
  _shader->set_uniform("u_color", glm::vec3(1.0F, 0.0F, 0.0F));
  for (const auto &renderable : _renderables) {
    renderable->draw();
  }

  // Draw local axes
  _shader->set_uniform("u_use_vertex_color", true);
  _local_axis.draw();
}

void Object::add_renderable(std::shared_ptr<Renderable> renderable) {
  _renderables.push_back(std::move(renderable));
}

}  // namespace eng
