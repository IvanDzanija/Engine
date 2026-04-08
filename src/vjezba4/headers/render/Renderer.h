#ifndef RENDERER_H
#define RENDERER_H

#pragma once
#include <algorithm>
#include <memory>
#include <vector>

#include "scene/Object.h"

namespace eng {

class Renderer {
 private:
  // Lista svih objekata koje treba iscrtati
  std::vector<std::shared_ptr<Object>> _objects;

 public:
  Renderer() = default;

  void register_object(std::shared_ptr<Object> obj) {
    _objects.push_back(std::move(obj));
  }

  void unregister_object(const std::shared_ptr<Object> &obj) {
    std::erase(_objects, obj);
  }

  // Dijagram: update(deltaTime)
  void update(float deltaTime) {}

  // Dijagram: render() - SRCE SUSTAVA
  void render() {
    for (const auto &obj : _objects) {
      obj->render();
    }
  }
};

}  // namespace eng
#endif
