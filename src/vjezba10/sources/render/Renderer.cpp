#include "render/Renderer.h"

namespace eng {

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

void Renderer::render() {
  if (_camera == nullptr) {
    std::cerr << "ERROR: Renderer cannot render without a linked camera." << std::endl;
    return;
  }
  if (_light == nullptr) {
    std::cerr << "ERROR: Renderer cannot render without a registered light source."
              << std::endl;
    return;
  }
  for (const auto &obj : _objects) {
    obj->render(_camera->perspective_matrix(), _camera->view_matrix(), _light,
                _camera->get_position());
  }
}

}  // namespace eng
