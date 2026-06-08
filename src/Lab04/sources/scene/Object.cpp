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
void Object::render() const {
  if (!_shader || _renderables.size() == 0) {
    std::cerr << "ERROR: Object cannot be rendered without a shader or renderables.\n";
    return;
  }

  _shader->use();
  _shader->set_uniform("u_color", glm::vec3(1.0F, 0.0F, 0.0F));

  for (const auto &renderable : _renderables) {
    renderable->draw();
  }
}

void Object::add_renderable(std::shared_ptr<Renderable> renderable) {
  _renderables.push_back(std::move(renderable));
}
}  // namespace eng
