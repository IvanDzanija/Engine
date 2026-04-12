#include "infra/ObjectLoader.h"

#include <filesystem>

#include "math/TransformGenerator.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace eng {
// ----------------------------------
// METHODS
// ----------------------------------
std::vector<Mesh> ObjectLoader::load_model(const std::string &name) {
  static std::string path =
      std::filesystem::current_path().string() + RESOURCES_DIR + name;

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

  std::vector<Mesh> meshes;
  _process_node(scene->mRootNode, scene, meshes);
  _normalize(meshes);
  return meshes;
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
void ObjectLoader::_process_node(aiNode *node, const aiScene *scene,
                                 std::vector<Mesh> &meshes) {
  for (uint32 i = 0; i < node->mNumMeshes; ++i) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(_process_mesh(mesh, scene));
  }
  for (uint32 i = 0; i < node->mNumChildren; i++) {
    _process_node(node->mChildren[i], scene, meshes);
  }
}

Mesh ObjectLoader::_process_mesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<uint32> indices;
  std::vector<Texture> textures;

  // Vertices
  for (uint32 i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex;
    glm::vec3 vec;

    // Coords
    vec.x = mesh->mVertices[i].x;
    vec.y = mesh->mVertices[i].y;
    vec.z = mesh->mVertices[i].z;
    vertex.coords = vec;
    // Normals
    if (mesh->HasNormals()) {
      vec.x = mesh->mNormals[i].x;
      vec.y = mesh->mNormals[i].y;
      vec.z = mesh->mNormals[i].z;
      vertex.normal = vec;
    }
    // Texture coords
    if (mesh->mTextureCoords[0] != nullptr) {
      glm::vec2 vec;
      vec.x = mesh->mTextureCoords[0][i].x;
      vec.y = mesh->mTextureCoords[0][i].y;
      vertex.tex_coords = vec;
    } else {
      vertex.tex_coords = glm::vec2(0.0F, 0.0F);
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
  return {vertices, indices, textures};
}

void ObjectLoader::_normalize(std::vector<Mesh> &meshes) {
  if (meshes.empty()) {
    return;
  }

  glm::vec3 global_min(meshes[0].get_bounding_box().min);
  glm::vec3 global_max(meshes[0].get_bounding_box().max);
  for (uint32 i = 1; i < meshes.size(); ++i) {
    auto box = meshes[i].get_bounding_box();
    global_min = glm::min(global_min, box.min);
    global_max = glm::max(global_max, box.max);
  }

  glm::vec3 center = (global_min + global_max) * 0.5F;
  glm::vec3 range = global_max - global_min;
  float M = std::max({range.x, range.y, range.z});

  auto matrix = eng::TransformGenerator::scale_3D(glm::vec3(2.0f / M)) *
                eng::TransformGenerator::translate_3D(-center);

  for (auto &mesh : meshes) {
    mesh.apply_transform(matrix);
  }
}
}  // namespace eng
