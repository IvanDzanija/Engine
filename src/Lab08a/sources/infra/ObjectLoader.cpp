#include "infra/ObjectLoader.h"

#include <filesystem>

#include "math/TransformGenerator.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace eng {
// ----------------------------------
// METHODS
// ----------------------------------
Model ObjectLoader::load_model(const std::string &name) {
  std::string path = std::filesystem::current_path().string() + RESOURCES_DIR + name;

  const aiScene *scene = get_importer().ReadFile(
      path.c_str(), aiProcess_CalcTangentSpace | aiProcess_Triangulate |
                        aiProcess_JoinIdenticalVertices | aiProcess_SortByPType |
                        aiProcess_FlipUVs | aiProcess_GenNormals

  );
  if (scene == nullptr ||
      static_cast<bool>(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
      scene->mRootNode == nullptr) {
    std::cerr << get_importer().GetErrorString();
    return {};
  }

  assert(scene->HasMeshes());

  std::vector<std::shared_ptr<Mesh>> meshes;
  std::vector<std::shared_ptr<Material>> materials;
  _process_node(scene->mRootNode, scene, meshes);
  _process_materials(scene, materials);
  _normalize(meshes);

  return {.meshes = std::move(meshes), .materials = std::move(materials)};
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
void ObjectLoader::_process_node(aiNode *node, const aiScene *scene,
                                 std::vector<std::shared_ptr<Mesh>> &meshes) {
  for (size_t i = 0; i < node->mNumMeshes; ++i) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.emplace_back(std::make_shared<Mesh>(_process_mesh(mesh)));
  }
  for (size_t i = 0; i < node->mNumChildren; ++i) {
    _process_node(node->mChildren[i], scene, meshes);
  }
}

void ObjectLoader::_process_materials(
    const aiScene *scene, std::vector<std::shared_ptr<Material>> &materials) {
  for (size_t i = 0; i < scene->mNumMaterials; ++i) {
    aiMaterial *material = scene->mMaterials[i];
    materials.emplace_back(std::make_shared<Material>(_process_material(material)));
  }
}

Mesh ObjectLoader::_process_mesh(aiMesh *mesh) {
  // data to fill
  std::vector<Vertex> vertices;
  std::vector<uint32> indices;
  std::vector<Texture> textures;

  // Vertices
  for (size_t i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;

    // Coords
    vertex.coords.x = mesh->mVertices[i].x;
    vertex.coords.y = mesh->mVertices[i].y;
    vertex.coords.z = mesh->mVertices[i].z;
    // Normals
    if (mesh->HasNormals()) {
      vertex.normal.x = mesh->mNormals[i].x;
      vertex.normal.y = mesh->mNormals[i].y;
      vertex.normal.z = mesh->mNormals[i].z;
    } else {
      std::println("Mesh does not have normals!");
    }
    // Texture coords
    if (mesh->mTextureCoords[0] != nullptr) {
      vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
      vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
    }
    vertices.push_back(vertex);
  }

  // Indices
  for (uint32 i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];
    for (uint32 j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }
  uint32 material_index = mesh->mMaterialIndex;
  return {vertices, indices, textures, material_index};
}

Material ObjectLoader::_process_material(aiMaterial *material) {
  aiColor3D ambient(1.0F, 1.0F, 1.0F);
  aiColor3D diffuse(1.0F, 1.0F, 1.0F);
  aiColor3D specular(1.0F, 1.0F, 1.0F);
  aiColor3D reflective(0.0F, 0.0F, 0.0F);  // Not used
  aiColor3D emissive(0.0F, 0.0F, 0.0F);    // Not used
  float shininess = 32.0F;

  if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambient) != AI_SUCCESS) {
    std::println("Failed to get ambient color for material!");
  }
  if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse) != AI_SUCCESS) {
    std::println("Failed to get diffuse color for material!");
  }
  if (material->Get(AI_MATKEY_COLOR_SPECULAR, specular) != AI_SUCCESS) {
    std::println("Failed to get specular color for material!");
  }
  if (material->Get(AI_MATKEY_SHININESS, shininess) != AI_SUCCESS) {
    std::println("Failed to get shininess for material!");
  } else {
    assert(shininess >= 0.0F);
    shininess = std::max(shininess, 1.0F);
  }

  return Material{glm::vec3(ambient.r, ambient.g, ambient.b),
                  glm::vec3(diffuse.r, diffuse.g, diffuse.b),
                  glm::vec3(specular.r, specular.g, specular.b), shininess};
}

void ObjectLoader::_normalize(std::vector<std::shared_ptr<Mesh>> &meshes) {
  if (meshes.empty()) {
    return;
  }

  glm::vec3 global_min(meshes[0]->get_bounding_box().min);
  glm::vec3 global_max(meshes[0]->get_bounding_box().max);
  for (uint32 i = 1; i < meshes.size(); ++i) {
    auto box = meshes[i]->get_bounding_box();
    global_min = glm::min(global_min, box.min);
    global_max = glm::max(global_max, box.max);
  }

  glm::vec3 center = (global_min + global_max) * 0.5F;
  glm::vec3 range = global_max - global_min;
  float M = std::max({range.x, range.y, range.z});

  auto matrix = eng::TransformGenerator::scale_3D(glm::vec3(2.0f / M)) *
                eng::TransformGenerator::translate_3D(-center);

  for (auto &mesh : meshes) {
    mesh->apply_transform(matrix);
  }
}
}  // namespace eng
