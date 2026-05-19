#include "infra/ResourceManager.h"

#include "infra/ObjectLoader.h"

namespace eng {
std::shared_ptr<Shader> ResourceManager::get_shader(const std::string &name) {
  if (_shaders.contains(name)) {
    return _shaders[name];
  }

  auto shader = std::make_shared<Shader>(name);
  _shaders[name] = shader;
  return shader;
}

std::shared_ptr<Shader> ResourceManager::get_shader(const std::string &name,
                                                    const std::string &vertex_name,
                                                    const std::string &fragment_name,
                                                    const std::string &geometry_name) {
  if (_shaders.contains(name)) {
    return _shaders[name];
  }

  auto shader = std::make_shared<Shader>(vertex_name, fragment_name, geometry_name);
  _shaders[name] = shader;
  return shader;
}

std::shared_ptr<Model> ResourceManager::get_model(const std::string &name) {
  if (_models.contains(name)) {
    return _models[name];
  }

  _models[name] = std::make_shared<Model>(ObjectLoader::load_model(name));
  for (auto x : _models) {
    std::cout << x.first << std::endl;
  }
  return _models[name];
}

}  // namespace eng
