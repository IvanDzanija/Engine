#include "scene/Object.h"

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Object::Object(std::shared_ptr<Shader> shader) : _shader(std::move(shader)) {}
Object::Object(const std::shared_ptr<Model> &model, std::shared_ptr<Shader> shader)
    : _shader(std::move(shader)) {
  for (const auto &mesh_ptr : model->meshes) {
    _renderables.push_back(mesh_ptr);
  }
  if (!model->materials.empty()) {
    _material = model->materials[0];
  }
}

// Copy constructors
Object::Object(const Object &other)
    : _renderables(other._renderables),
      _material(other._material),
      _shader(other._shader),
      _use_uniform_color(other._use_uniform_color) {}

Object &Object::operator=(const Object &other) {
  if (this != &other) {
    _renderables = other._renderables;
    _material = other._material;
    _shader = other._shader;
    _use_uniform_color = other._use_uniform_color;
  }
  return *this;
}
// Move constructors
Object::Object(Object &&other) noexcept
    : _renderables(std::move(other._renderables)),
      _material(std::move(other._material)),
      _shader(std::move(other._shader)),
      _use_uniform_color(other._use_uniform_color) {}

Object &Object::operator=(Object &&other) noexcept {
  if (this != &other) {
    _renderables = std::move(other._renderables);
    _material = std::move(other._material);
    _shader = std::move(other._shader);
    _use_uniform_color = other._use_uniform_color;
  }
  return *this;
}

// ----------------------------------
// METHODS
// ----------------------------------
void Object::render(const glm::mat4 &projection_matrix, const glm::mat4 &view_matrix,
                    const std::shared_ptr<Light> &light,
                    const std::optional<glm::vec3> &camera_position) const {
  if (!_shader) {
    std::cerr << "ERROR: Object cannot be rendered without a shader or renderables."
              << std::endl;
    return;
  }

  if (_renderables.empty()) {
    std::cerr << "WARNING: Object has no renderables to draw." << std::endl;
  }
  if (light == nullptr) {
    std::cerr << "WARNING: Object is rendered without a light source." << std::endl;
  }

  _shader->use();
  _shader->set_uniform("u_projection", projection_matrix);
  _shader->set_uniform("u_view", view_matrix);
  _shader->set_uniform("u_model", model_matrix());
  if (camera_position.has_value()) {
    _shader->set_uniform("u_eye_pos", camera_position.value());
  }

  // Draw object
  if (_use_uniform_color) {
    _shader->set_uniform("u_use_vertex_color", false);
    _shader->set_uniform("u_color", glm::vec3(1.0F, 0.0F, 0.0F));
  } else {
    _shader->set_uniform("u_use_vertex_color", true);
  }

  if (_material != nullptr) {
    _shader->set_uniform("u_material.ambient", _material->get_ambient());
    _shader->set_uniform("u_material.diffuse", _material->get_diffuse());
    _shader->set_uniform("u_material.specular", _material->get_specular());
    _shader->set_uniform("u_material.shininess", _material->get_shininess());
  }

  _shader->set_uniform("u_light.position", light->get_position());
  _shader->set_uniform("u_light.intensity", light->get_intensity());
  _shader->set_uniform("u_light.ambient", light->get_ambient());

  for (const auto &renderable : _renderables) {
    renderable->draw();
  }

  // Draw local axes
  // _shader->set_uniform("u_use_vertex_color", true);
  // _local_axis.draw();
}

void Object::add_renderable(std::shared_ptr<Renderable> renderable) {
  _renderables.push_back(std::move(renderable));
}

}  // namespace eng
