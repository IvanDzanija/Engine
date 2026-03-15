#include "Shader.h"

#include <fstream>
#include <iostream>
#include <print>
#include <sstream>

namespace eng {

// ----------------------------------
// HELPER METHODS
// ----------------------------------
namespace detail {
std::string read_file(const char *path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::runtime_error("Could not open shader file: " + std::string(path));
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  file.close();
  return buffer.str();
}

uint32 compile_shader(const std::string &code, uint32 type) {
  const char *code_cstr = code.c_str();
  uint32 shader = glCreateShader(type);
  glShaderSource(shader, 1, &code_cstr, nullptr);
  glCompileShader(shader);
  return shader;
}
}  // namespace detail

// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  uint32 vertex;
  uint32 fragment;

  // Load & compile the vertex shader
  try {
    std::string vertex_code = detail::read_file(vertexPath);
    vertex = detail::compile_shader(vertex_code, GL_VERTEX_SHADER);
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
    std::string fragment_code = detail::read_file(fragmentPath);
    fragment = detail::compile_shader(fragment_code, GL_FRAGMENT_SHADER);
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

Shader::Shader(const char *vertexPath, const char *geometryPath,
               const char *fragmentPath) {
  uint32 vertex;
  uint32 geometry;
  uint32 fragment;

  // Load & compile the vertex shader
  try {
    std::string vertex_code = detail::read_file(vertexPath);
    vertex = detail::compile_shader(vertex_code, GL_VERTEX_SHADER);
    _check_compiler_errors(vertex, _ShaderType::VERTEX);
  } catch (const std::ifstream::failure &e) {
    std::cerr << "ERROR: LOADING VERTEX SHADER" << e.what() << '\n';
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "UNKNOWN ERROR!" << std::endl;
  }

  // Load & compile the geometry shader
  try {
    std::string geometry_code = detail::read_file(geometryPath);
    geometry = detail::compile_shader(geometry_code, GL_GEOMETRY_SHADER);
    _check_compiler_errors(geometry, _ShaderType::GEOMETRY);
  } catch (const std::ifstream::failure &e) {
    std::cerr << "ERROR: LOADING GEOMETRY SHADER" << e.what() << '\n';
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "UNKNOWN ERROR!" << std::endl;
  }

  // Load & compile the fragment shader
  try {
    std::string fragment_code = detail::read_file(fragmentPath);
    fragment = detail::compile_shader(fragment_code, GL_FRAGMENT_SHADER);
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
  glAttachShader(ID, geometry);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  _check_compiler_errors(ID, _ShaderType::PROGRAM);

  glDeleteShader(vertex);
  glDeleteShader(geometry);
  glDeleteShader(fragment);
}

Shader::~Shader() { glDeleteProgram(ID); }

// ----------------------------------
// PUBLIC METHODS
// ----------------------------------
void Shader::use() const { glUseProgram(ID); }

void Shader::setUniform(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setUniform(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
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
}  // namespace eng
