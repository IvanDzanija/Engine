#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H
#pragma once
#include <memory>
#include <string>
#include <unordered_map>

#include "Model.h"
#include "render/Shader.h"

namespace eng {
// Singleton class
class ResourceManager {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  ResourceManager(const ResourceManager &) = delete;
  ResourceManager &operator=(const ResourceManager &) = delete;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  static std::shared_ptr<Shader> get_shader(const std::string &name);
  static std::shared_ptr<Shader> get_shader(const std::string &name,
                                            const std::string &vertex_name,
                                            const std::string &fragment_name,
                                            const std::string &geometry_name = "");
  static std::shared_ptr<Model> get_model(const std::string &directory,
                                          const std::string &name);
  static std::shared_ptr<Texture> get_texture(const std::string &type,
                                              const std::string &path);
  static void add_texture(const std::string &type, const std::string &path,
                          std::shared_ptr<Texture> texture);

 private:
  ResourceManager() = default;  // Private constructor to prevent instantiation

  // ----------------------------------
  // STATIC FIELDS
  // ----------------------------------
  inline static std::unordered_map<std::string, std::shared_ptr<Shader>> _shaders;
  inline static std::unordered_map<std::string, std::shared_ptr<Model>> _models;
  // Key is like this: (type:path), for example: "diffuse:/path/to/texture.jpg"
  inline static std::unordered_map<std::string, std::shared_ptr<Texture>> _textures;
};

}  // namespace eng
#endif
