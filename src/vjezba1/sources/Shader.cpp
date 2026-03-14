#include "Shader.h"

#include <iostream>
#include <print>
namespace eng {

Shader::Shader(const char *vertexPath, const char *fragmentPath) {
  std::string vertex_code;
  std::string fragment_code;
  std::ifstream vertex_file;
  std::ifstream fragment_file;
  vertex_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fragment_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  // Load vertex shader
  try {
    vertex_file.open(vertexPath);
    while (std::getline(vertex_file, vertex_code)) {
    };
    vertex_file.close();
  } catch (const std::ifstream::failure &e) {
    std::cerr << "ERROR: LOADING VERTEX SHADER" << e.what() << '\n';
  } catch (const std::exception &e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "UNKNOWN ERROR!" << std::endl;
  }

  // Load fragment shader

  // try {
  //   // open files
  //   vShaderFile.open(vertexPath);
  //   fShaderFile.open(fragmentPath);
  //   std::stringstream vShaderStream, fShaderStream;
  //   // read file's buffer contents into streams
  //   vShaderStream << vShaderFile.rdbuf();
  //   fShaderStream << fShaderFile.rdbuf();
  //   // close file handlers
  //   vShaderFile.close();
  //   fShaderFile.close();
  //   // convert stream into string
  //   vertexCode = vShaderStream.str();
  //   fragmentCode = fShaderStream.str();
  // } catch (std::ifstream::failure e) {
  //   fprintf(stderr, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
  // }

  const char *vShaderCode = vertexCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
  // 2. compile shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  _check_compiler_errors(vertex, "VERTEX");

  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  _check_compiler_errors(fragment, "FRAGMENT");

  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  _check_compiler_errors(ID, "PROGRAM");

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::Shader(const char *vertexPath, const char *geometryPath,
               const char *fragmentPath) {
  // std::cout << vertexPath<<std::endl;
  std::string vertexCode;
  std::string geometryCode;
  std::string fragmentCode;
  std::ifstream vShaderFile;
  std::ifstream gShaderFile;
  std::ifstream fShaderFile;
  // ensure ifstream objects can throw exceptions:
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  // printf("%s\n\n", vertexPath);
  try {
    // open files
    // printf("%s\n\n", vertexPath);
    vShaderFile.open(vertexPath);
    gShaderFile.open(geometryPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, gShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    gShaderStream << gShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    gShaderFile.close();
    fShaderFile.close();
    // convert stream into string
    vertexCode = vShaderStream.str();
    geometryCode = gShaderStream.str();
    fragmentCode = fShaderStream.str();
  } catch (std::ifstream::failure e) {
    fprintf(stderr, "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ\n");
  }

  const char *vShaderCode = vertexCode.c_str();
  const char *gShaderCode = geometryCode.c_str();
  const char *fShaderCode = fragmentCode.c_str();
  // printf("%s\n\n\n %s\n\n\n%s", vShaderCode, gShaderCode, fShaderCode);

  // 2. compile shaders
  unsigned int vertex, geometry, fragment;
  int success;
  char infoLog[512];

  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  _check_compiler_errors(vertex, "VERTEX");

  // geometry shader
  geometry = glCreateShader(GL_GEOMETRY_SHADER);
  glShaderSource(geometry, 1, &gShaderCode, NULL);
  glCompileShader(geometry);
  _check_compiler_errors(geometry, "GEOMETRY");

  // fragment Shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  _check_compiler_errors(fragment, "FRAGMENT");

  // shader Program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, geometry);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  _check_compiler_errors(ID, "PROGRAM");

  // delete the shaders as they're linked into our program now and no longer necessary
  glDeleteShader(vertex);
  glDeleteShader(geometry);
  glDeleteShader(fragment);
}

Shader::~Shader() { glDeleteProgram(ID); }

void Shader::use() { glUseProgram(ID); }

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
void Shader::_check_compiler_errors(unsigned int shader, const std::string &type) {
  static constexpr size_t LOG_LENGTH = 1024;

  int success;
  std::string infolog(LOG_LENGTH, '\0');

  if (type != "PROGRAM") {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == 0) {
      glGetShaderInfoLog(shader, LOG_LENGTH, nullptr, infolog.data());
      std::println("ERROR::SHADER_COMPILATION_ERROR of type: {} \n{} ", type, infolog);
    }
  } else {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (success == 0) {
      glGetProgramInfoLog(shader, LOG_LENGTH, nullptr, infolog.data());
      std::println("ERROR::PROGRAM_LINKING_ERROR of type:{} \n{} ", type, infolog);
    }
  }
}
}  // namespace eng
