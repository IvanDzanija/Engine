#ifndef RENDERER_H
#define RENDERER_H

#pragma once
#include <memory>
#include <vector>

#include "scene/Camera.h"
#include "scene/Object.h"

namespace eng {

class Renderer {
 private:
  std::vector<std::shared_ptr<Object>> _objects;
  std::shared_ptr<Camera> _camera;

 public:
  Renderer() = default;

  void register_object(std::shared_ptr<Object> obj);
  void unregister_object(const std::shared_ptr<Object> &obj);
  void link_camera(std::shared_ptr<Camera> camera);
  void unlink_camera();
  void render();
};

}  // namespace eng
#endif
