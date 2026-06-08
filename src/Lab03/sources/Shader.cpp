#include "Shader.h"

#include <fstream>
#include <iostream>
#include <print>
#include <sstream>

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Shader::Shader(const std::string &path, const std::string &name) {
  const std::string vertex_path = path + name + ".vert";
  const std::string fragment_path = path + name + ".frag";
  uint32 vertex;
  uint32 fragment;

  // Load & compile the vertex shader
  try {
    std::string vertex_code = _read_shader_source(vertex_path.c_str());
    vertex = _compile_shader(vertex_code, GL_VERTEX_SHADER);
    _check_compiler_errors(vertex, _ShaderType::VERTEX);
  } catch (const std::ifstream::failure &e) {
    std::cerr << "ERROR: LOADING VERTEX SHADER" << e.what() << '\n';
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "UNKNOWN ERROR!" << std::endl;
  }

  // Load & compile the fragment shader
  try {
    std::string fragment_code = _read_shader_source(fragment_path.c_str());
    fragment = _compile_shader(fragment_code, GL_FRAGMENT_SHADER);
    _check_compiler_errors(fragment, _ShaderType::FRAGMENT);
  } catch (const std::ifstream::failure &e) {
    std::cerr << "ERROR: LOADING FRAGMENT SHADER" << e.what() << '\n';
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "UNKNOWN ERROR!" << std::endl;
  }

  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  _check_compiler_errors(ID, _ShaderType::PROGRAM);

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::Shader(Shader &&other) noexcept : ID(other.ID) { other.ID = 0; }

Shader &Shader::operator=(Shader &&other) noexcept {
  if (this != &other) {
    if (ID != 0) {
      glDeleteProgram(ID);
    }

    ID = other.ID;
    other.ID = 0;
  }
  return *this;
}

Shader::~Shader() { glDeleteProgram(ID); }

// ----------------------------------
// PUBLIC METHODS
// ----------------------------------
void Shader::use() const { glUseProgram(ID); }
void Shader::set_uniform(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::set_uniform(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::set_uniform(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::set_uniform(const std::string &name, const glm::vec3 &value) const {
  glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::set_uniform(const std::string &name, const glm::mat4 &value) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

// ----------------------------------
// PRIVATE METHODS
// ----------------------------------
void Shader::_check_compiler_errors(uint32 shader, const _ShaderType type) {
  static constexpr size_t LOG_LENGTH = 1024;

  int success;
  std::string infolog(LOG_LENGTH, '\0');

  switch (type) {
    case _ShaderType::PROGRAM:
      glGetProgramiv(shader, GL_LINK_STATUS, &success);
      if (success == 0) {
        glGetProgramInfoLog(shader, LOG_LENGTH, nullptr, infolog.data());
        std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << Shader::to_string(type)
                  << std::endl
                  << infolog << std::endl;
      }
      break;
    default:
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
      if (success == 0) {
        glGetShaderInfoLog(shader, LOG_LENGTH, nullptr, infolog.data());
        std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: "
                  << Shader::to_string(type) << std::endl
                  << infolog << std::endl;
      }
      break;
  }
}

std::string Shader::_read_shader_source(const char *path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open shader file: " + std::string(path));
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  return buffer.str();
}

uint32 Shader::_compile_shader(const std::string &code, uint32 type) {
  const char *code_cstr = code.c_str();
  uint32 shader = glCreateShader(type);
  glShaderSource(shader, 1, &code_cstr, nullptr);
  glCompileShader(shader);
  return shader;
}
}  // namespace eng
