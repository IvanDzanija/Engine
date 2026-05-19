#ifndef OBJECT_LOADER_H
#define OBJECT_LOADER_H

#pragma once
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <assimp/Importer.hpp>

#include "Model.h"
#include "stb_image.h"

namespace eng {
class ObjectLoader {
 public:
  // Load a 3D model from a file and return Model object
  static Model load_model(const std::string &name);

 private:
  ObjectLoader() = default;  // Private constructor to prevent instantiation

  // ----------------------------------
  // CONSTANTS
  // ----------------------------------
  static constexpr std::string RESOURCES_DIR = "/resources/";

  // ----------------------------------
  // STATIC FIELDS
  // ----------------------------------
  static Assimp::Importer &get_importer() {
    static Assimp::Importer importer;
    return importer;
  }

  // ----------------------------------
  // PRIVATE METHODS
  // ----------------------------------
  static void _process_node(aiNode *node, const aiScene *scene,
                            std::vector<std::shared_ptr<Mesh>> &meshes);
  static void _process_materials(const aiScene *scene,
                                 std::vector<std::shared_ptr<Material>> &materials);
  static Mesh _process_mesh(aiMesh *mesh);
  static Material _process_material(aiMaterial *material);

  static void _normalize(std::vector<std::shared_ptr<Mesh>> &meshes);
};
}  // namespace eng

#endif
