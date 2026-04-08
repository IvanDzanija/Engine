#include "infra/ObjectLoader.h"

#include <filesystem>

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
  // data to fill
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

  // Materials
  // aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
  // // we assume a convention for sampler names in the shaders. Each diffuse texture
  // // should be named as 'texture_diffuseN' where N is a sequential number ranging
  // from 1
  // // to MAX_SAMPLER_NUMBER. Same applies to other texture as the following list
  // // summarizes: diffuse: texture_diffuseN specular: texture_specularN normal:
  // // texture_normalN

  // // 1. diffuse maps
  // std::vector<Texture> diffuseMaps =
  //     loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
  // textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
  // // 2. specular maps
  // vector<Texture> specularMaps =
  //     loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
  // textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  // // 3. normal maps
  // std::vector<Texture> normalMaps =
  //     loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
  // textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
  // // 4. height maps
  // std::vector<Texture> heightMaps =
  //     loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
  // textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
  return {vertices, indices, textures};
}
}  // namespace eng
