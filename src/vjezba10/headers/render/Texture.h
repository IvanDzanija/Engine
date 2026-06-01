#ifndef GPUTEXTURE_H
#define GPUTEXTURE_H

#pragma once
#include <glad/glad.h>

#include "Global.h"
#include "stb_image.h"

namespace eng {
struct Texture {
 public:
  explicit Texture(std::string type, std::string path)
      : _type(std::move(type)), _path(std::move(path)) {
    glGenTextures(1, &_id);

    int32 width;
    int32 height;
    int32 components;

    unsigned char *data = stbi_load(_path.c_str(), &width, &height, &components, 0);
    if (data != nullptr) {
      GLenum format;
      if (components == 1) {
        format = GL_RED;
      } else if (components == 3) {
        format = GL_RGB;
      } else if (components == 4) {
        format = GL_RGBA;
      } else {
        std::cerr << "Unsupported number of components (" << components
                  << ") in texture at path: " << path << std::endl;
        return;
      }

      glBindTexture(GL_TEXTURE_2D, _id);
      {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      }
      glBindTexture(GL_TEXTURE_2D, 0);
      stbi_image_free(data);
    } else {
      std::cerr << "Texture failed to load at path: " << _path << std::endl;
      stbi_image_free(data);
    }
  }

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] uint32 get_id() const { return _id; }
  [[nodiscard]] const std::string &get_type() const { return _type; }
  [[nodiscard]] const std::string &get_path() const { return _path; }

 private:
  GLuint _id;
  std::string _type;
  std::string _path;
};
}  // namespace eng

#endif
