#pragma once
#include <cmath>
#include <filesystem>
#include <iostream>
#include <print>
#include <string>

#include "Graphics.h"

namespace eng {

Graphics::Graphics(int width, int height, glm::vec3 clearColor, char *path)
    : _width(width), _height(height), _clear_color(clearColor) {
  // Initialize _raster with clear color
  _raster = new float[_width * _height * 3];
  for (int i = 0; i < _height; i++) {
    for (int j = 0; j < _width; j++) {
      _raster[(i * _width * 3) + (j * 3)] = clearColor.x;
      _raster[(i * _width * 3) + (j * 3) + 1] = clearColor.y;
      _raster[(i * _width * 3) + (j * 3) + 2] = clearColor.z;
    }
  }

  load_glfw();

  gladLoadGL();

  const GLubyte *version = glGetString(GL_VERSION);
  if (version != nullptr) {
    std::println("OpenGL {}.", reinterpret_cast<const char *>(version));
  } else {
    std::println("Failed to get OpenGL version (no valid context?)");
  }

  glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);

  loadRasterShader(path);

  glUniform1i(glGetUniformLocation(_raster_shader->ID, "texture1"), 0);

  glGenTextures(1, &_rasterID);
  glBindTexture(GL_TEXTURE_2D, _rasterID);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT,
               (const void *)_raster);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  glGenBuffers(1, &VBO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_raster_vertices), _raster_vertices.data(),
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

Graphics::~Graphics() {
  _width = 0;
  _height = 0;
  delete[] _raster;
  delete _raster_shader;
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
}
void (*Graphics::mouse_callback_user)(int, int, int) = nullptr;
glm::vec2 Graphics::cursor_position(0, 0);
GLFWwindow *Graphics::window = nullptr;

void Graphics::mouse_button_callback(GLFWwindow *window, int button, int action,
                                     int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    (*mouse_callback_user)(cursor_position.x / 10, cursor_position.y / 10, 0);
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
    (*mouse_callback_user)(cursor_position.x / 10, cursor_position.y / 10, 1);
}

void Graphics::cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  cursor_position = glm::vec2(xpos, ypos);
}

// ----------------------------------
// OPENGL SETUP
// ----------------------------------
void Graphics::load_glfw() {
#if defined(__APPLE__)
  glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif
  std::println("{}", std::filesystem::absolute(".").string());
  glfwInit();
  std::println("{}", std::filesystem::absolute(".").string());

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window = glfwCreateWindow(_width * 10, _height * 10, "_raster", nullptr, nullptr);

  // Check for Valid Context
  if (window == nullptr) {
    fprintf(stderr, "Failed to Create OpenGL Context");
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);
}

Shader *Graphics::loadRasterShader(char *path) {
  std::string sPath(path);
  std::string pathVert;
  std::string pathFrag;

  pathVert.append(path, sPath.find_last_of("\\/") + 1);
  pathFrag.append(path, sPath.find_last_of("\\/") + 1);
  if (pathFrag[pathFrag.size() - 1] == '/') {
    pathVert.append("shaders/shader.vert");
    pathFrag.append("shaders/shader.frag");
  } else if (pathFrag[pathFrag.size() - 1] == '\\') {
    pathVert.append("shaders\\shader.vert");
    pathFrag.append("shaders\\shader.frag");
  } else {
    std::cerr << "nepoznat format pozicije shadera";
    exit(1);
  }

  _raster_shader = new Shader(pathVert.c_str(), pathFrag.c_str());
  return _raster_shader;
}

int Graphics::osvijetliFragment(int x, int y) {
  return osvijetliFragment(x, y, glm::vec3(1, 1, 1));
}

int Graphics::osvijetliFragment(int x, int y, glm::vec3 color) {
  if (x >= 0 && x < _width && y >= 0 && y < _height) {
    _raster[y * _width * 3 + x * 3] = color.x;
    _raster[y * _width * 3 + x * 3 + 1] = color.y;
    _raster[y * _width * 3 + x * 3 + 2] = color.z;
    return 0;
  }
  std::cerr << "ERROR: pokusaj osvjetljavanja izvan podrucja";
  return -1;
}

void Graphics::pobrisiProzor() {
  std::fill_n(_raster, _width * _height * 3, 0);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::iscrtajRaster() {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  // glBindTexture(GL_TEXTURE_2D, _rasterID);
  glUseProgram(_raster_shader->ID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _rasterID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT,
               (const void *)_raster);

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);

  glfwSwapBuffers(window);
  glfwPollEvents();
}

bool Graphics::trebaZatvoriti() { return glfwWindowShouldClose(window) == 0; }

// int Graphics::register_mouse_click

int Graphics::registrirajFunkcijuZaKlikMisa(void (*Mouse_callback_user)(int, int,
                                                                        int)) {
  glfwSetCursorPosCallback(window, cursor_position_callback);
  glfwSetMouseButtonCallback(window, mouse_button_callback);

  mouse_callback_user = Mouse_callback_user;

  return 0;
}
}  // namespace eng
