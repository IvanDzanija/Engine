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

void Renderer::render() {
  if (_camera == nullptr) {
    std::cerr << "ERROR: Renderer cannot render without a linked camera." << std::endl;
    return;
  }
  for (const auto &obj : _objects) {
    obj->render(_camera->perspective_matrix(), _camera->view_matrix(),
                _camera->get_position());
  }
}

}  // namespace eng
