#ifndef RENDERER_H
#define RENDERER_H

#pragma once
#include <memory>
#include <vector>

#include "scene/Camera.h"
#include "scene/Object.h"

namespace eng {

class Renderer {
 public:
  Renderer() = default;

  void register_object(std::shared_ptr<Object> obj);
  void unregister_object(const std::shared_ptr<Object> &obj);
  void link_camera(std::shared_ptr<Camera> camera);
  void unlink_camera();
  void register_light_source(std::shared_ptr<Light> light);
  void unregister_light_source(const std::shared_ptr<Light> &light);
  void render();

 private:
  std::vector<std::shared_ptr<Object>> _objects;
  std::shared_ptr<Light> _light;
  std::shared_ptr<Camera> _camera;
};

}  // namespace eng
#endif
