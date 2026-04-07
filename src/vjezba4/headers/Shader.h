#pragma once

#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <string>

#include "Global.h"

namespace eng {
class Shader {
 private:
  enum class _ShaderType : uint8 { VERTEX, GEOMETRY, FRAGMENT, PROGRAM };
  static void _check_compiler_errors(uint32 shader, _ShaderType type);
  static std::string _read_shader_source(const char *path);
  static uint32 _compile_shader(const std::string &code, uint32 type);

 public:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  uint32 ID;

  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  explicit Shader(const std::string &path, const std::string &name);
  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  Shader(Shader &&other) noexcept;
  Shader &operator=(Shader &&other) noexcept;
  ~Shader();

  // ----------------------------------
  // METHODS
  // ----------------------------------
  void use() const;
  void set_uniform(const std::string &name, bool value) const;
  void set_uniform(const std::string &name, int value) const;
  void set_uniform(const std::string &name, float value) const;
  void set_uniform(const std::string &name, const glm::vec3 &value) const;
  void set_uniform(const std::string &name, const glm::mat4 &value) const;

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
