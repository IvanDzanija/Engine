#include "render/Renderer.h"

namespace eng {

// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Renderer::Renderer() : _shadow_map(2048, 2048) {
  _depth_shader = ResourceManager::get_shader("simple_depth_shader");
}

// ----------------------------------
// METHODS
// ----------------------------------
void Renderer::register_object(std::shared_ptr<Object> obj) {
  _objects.push_back(std::move(obj));
}

void Renderer::unregister_object(const std::shared_ptr<Object> &obj) {
  std::erase(_objects, obj);
}

void Renderer::link_camera(std::shared_ptr<Camera> camera) {
  _camera = std::move(camera);
}
void Renderer::unlink_camera() { _camera.reset(); }

void Renderer::register_light_source(std::shared_ptr<Light> light) {
  _light = std::move(light);
}
void Renderer::unregister_light_source(const std::shared_ptr<Light> &light) {
  if (_light == light) {
    _light.reset();
  }
}

void Renderer::render(uint32 width, uint32 height) {
  if (_camera == nullptr) {
    std::cerr << "ERROR: Renderer cannot render without a linked camera." << std::endl;
    return;
  }
  if (_light == nullptr) {
    std::cerr << "ERROR: Renderer cannot render without a registered light source."
              << std::endl;
    return;
  }
  if (_depth_shader == nullptr) {
    std::cerr << "ERROR: Renderer cannot render without a depth shader." << std::endl;
    return;
  }

  // First pass -> depth map generation
  _depth_shader->use();
  _shadow_map.bind_for_writing();
  _depth_shader->set_uniform("u_light_projection", _light->projection_matrix());
  _depth_shader->set_uniform("u_light_view", _light->view_matrix());

  for (const auto &obj : _objects) {
    obj->render_depth(_depth_shader);
  }
  _shadow_map.unbind_for_writing();

  // Second pass -> render scene with shadows
  glViewport(0, 0, width, height);  // Reset viewport to window dimensions
  _shadow_map.bind_for_reading(0);
  for (const auto &obj : _objects) {
    obj->render(_camera->perspective_matrix(), _camera->view_matrix(),
                _camera->get_position(), _light);
  }
}

}  // namespace eng
