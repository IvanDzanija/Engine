#include "infra/ResourceManager.h"

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

std::vector<std::shared_ptr<Renderable>> ResourceManager::get_model(
    const std::string &name) {
  if (_models.contains(name)) {
    return _models[name];
  }

  std::vector<eng::Mesh> raw_meshes = ObjectLoader::load_model(name);

  // TODO: Possibly faster with emplace_back and move semantics or RAII
  std::vector<std::shared_ptr<Renderable>> renderables;
  renderables.reserve(raw_meshes.size());

  for (auto &mesh : raw_meshes) {
    renderables.push_back(std::make_shared<Mesh>(std::move(mesh)));
  }

  _models[name] = renderables;
  return renderables;
}

}  // namespace eng
