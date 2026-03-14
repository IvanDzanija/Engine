#pragma once

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>

#include "Global.hpp"

namespace eng {
class Shader {
 public:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  uint32 ID;

  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  Shader(const char *vertexPath, const char *fragmentPath);
  Shader(const char *vertexPath, const char *geometryPath, const char *fragmentPath);
  ~Shader();

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void use();
  void setUniform(const std::string &name, bool value) const;
  void setUniform(const std::string &name, int value) const;
  void setUniform(const std::string &name, float value) const;

 private:
  static void _check_compiler_errors(unsigned int shader, const std::string &type);
};
}  // namespace eng
