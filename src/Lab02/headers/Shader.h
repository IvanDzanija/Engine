#pragma once

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <string>

#include "Global.h"

namespace eng {
class Shader {
 private:
  enum class _ShaderType : uint8 { VERTEX, GEOMETRY, FRAGMENT, PROGRAM };
  static void _check_compiler_errors(uint32 shader, _ShaderType type);

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
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  Shader(Shader &&other) noexcept;
  Shader &operator=(Shader &&other) noexcept;
  ~Shader();

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void use() const;
  void setUniform(const std::string &name, bool value) const;
  void setUniform(const std::string &name, int value) const;
  void setUniform(const std::string &name, float value) const;

  // ----------------------------------
  // HELPER METHODS
  // ----------------------------------
  static constexpr std::string_view to_string(_ShaderType stage) {
    switch (stage) {
      case _ShaderType::VERTEX:
        return "VERTEX";
      case _ShaderType::FRAGMENT:
        return "FRAGMENT";
      case _ShaderType::GEOMETRY:
        return "GEOMETRY";
      case _ShaderType::PROGRAM:
        return "PROGRAM";
    }
    return "UNKNOWN";
  }
};

}  // namespace eng
