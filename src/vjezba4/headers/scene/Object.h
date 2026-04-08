#ifndef OBJECT_H
#define OBJECT_H

#pragma once
#include <assimp/Importer.hpp>
#include <glm/glm.hpp>
#include <memory>

#include "render/Mesh.h"
#include "render/Shader.h"

namespace eng {
class Object {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  Object() = default;
  Object(std::shared_ptr<Shader> shader);
  Object(std::vector<std::shared_ptr<Renderable>> meshes,
         std::shared_ptr<Shader> shader);
  ~Object() = default;

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  void set_texture(std::shared_ptr<Texture> texture) { _texture = std::move(texture); }

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void render() const;

  void add_renderable(std::shared_ptr<Renderable> renderable);

  // Metoda render prema specifikaciji iz dijagrama
  // void render(const glm::mat4 &projection, const glm::mat4 &view, const Light &light)
  // {
  //   if (!shader || !mesh) {
  //     return;
  //   }

  //   shader->use();

  //   // Slanje uniformi u shader
  //   shader->setMat4("projection", projection);
  //   shader->setMat4("view", view);
  //   shader->setMat4("model", getModelMatrix());  // Iz klase Transform

  //   // Slanje podataka o svjetlu (Light je klasa iz tvog dijagrama)
  //   shader->setVec3("lightPos", light.getPosition());
  //   shader->setVec3("lightColor", light.getIntensity());

  //   if (texture) {
  //     glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
  //   }

  //   mesh->draw();
  // }

 private:
  std::vector<std::shared_ptr<Renderable>> _renderables;
  std::shared_ptr<Shader> _shader;
  std::shared_ptr<Texture> _texture;
  // Material material;
};

}  // namespace eng

#endif
