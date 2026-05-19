#ifndef MATERIAL_H
#define MATERIAL_H

#pragma once
#include <glm/glm.hpp>
#include <print>

namespace eng {
class Material {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  // Copy constructor
  Material() = default;
  explicit Material(const glm::vec3 &ambient, const glm::vec3 &diffuse,
                    const glm::vec3 &specular, float shininess)
      : _ambient(ambient),
        _diffuse(diffuse),
        _specular(specular),
        _shininess(shininess) {}

  // Rule of 5
  // Copy constructors
  Material(const Material &other) = delete;
  Material &operator=(const Material &other) = delete;
  // Move constructors
  Material(Material &&other) noexcept = default;
  Material &operator=(Material &&other) noexcept = default;
  ~Material() = default;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] const glm::vec3 &get_ambient() const noexcept { return _ambient; }
  void set_ambient(const glm::vec3 &ambient) { _ambient = ambient; }
  [[nodiscard]] const glm::vec3 &get_diffuse() const noexcept { return _diffuse; }
  void set_diffuse(const glm::vec3 &diffuse) { _diffuse = diffuse; }
  [[nodiscard]] const glm::vec3 &get_specular() const noexcept { return _specular; }
  void set_specular(const glm::vec3 &specular) { _specular = specular; }
  [[nodiscard]] float get_shininess() const noexcept { return _shininess; }
  void set_shininess(float shininess) { _shininess = shininess; }

 private:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  glm::vec3 _ambient;
  glm::vec3 _diffuse;
  glm::vec3 _specular;
  float _shininess;
};
}  // namespace eng

#endif
