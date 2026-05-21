#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Model.h"
#include "render/Shader.h"

namespace eng {

class ResourceManager {
 public:
  static std::shared_ptr<Shader> get_shader(const std::string &name);
  static std::shared_ptr<Shader> get_shader(const std::string &name,
                                            const std::string &vertex_name,
                                            const std::string &fragment_name,
                                            const std::string &geometry_name = "");
  static std::shared_ptr<Model> get_model(const std::string &name);

 private:
  inline static std::unordered_map<std::string, std::shared_ptr<Shader>> _shaders;
  inline static std::unordered_map<std::string, std::shared_ptr<Model>> _models;
};

}  // namespace eng
#endif
