#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "ObjectLoader.h"
#include "render/Mesh.h"
#include "render/Renderable.h"
#include "render/Shader.h"

namespace eng {

class ResourceManager {
 public:
  static std::shared_ptr<Shader> get_shader(const std::string &name);
  static std::shared_ptr<Shader> get_shader(const std::string &name,
                                            const std::string &vertex_name,
                                            const std::string &fragment_name,
                                            const std::string &geometry_name = "");
  static std::vector<std::shared_ptr<Renderable>> get_model(const std::string &path);

 private:
  inline static std::unordered_map<std::string, std::shared_ptr<Shader>> _shaders;
  inline static std::unordered_map<std::string,
                                   std::vector<std::shared_ptr<Renderable>>>
      _models;
};

}  // namespace eng
#endif
