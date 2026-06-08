#include "infra/ResourceManager.h"

#include "infra/ObjectLoader.h"

namespace eng {
// ----------------------------------
// GETTERS & SETTERS
// ----------------------------------
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

std::shared_ptr<Model> ResourceManager::get_model(const std::string &directory,
                                                  const std::string &name) {
  auto key = directory + "/" + name;
  if (_models.contains(key)) {
    return _models[key];
  }

  _models[key] = std::make_shared<Model>(ObjectLoader::load_model(directory, name));
  return _models[key];
}

std::shared_ptr<Texture> ResourceManager::get_texture(const std::string &type,
                                                      const std::string &path) {
  auto key = path + ":" + type;
  if (_textures.contains(key)) {
    return _textures[key];
  }
  return nullptr;
}

void ResourceManager::add_texture(const std::string &type, const std::string &path,
                                  std::shared_ptr<Texture> texture) {
  auto key = path + ":" + type;
  _textures[key] = std::move(texture);
}

}  // namespace eng
