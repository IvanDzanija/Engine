#pragma once
#include "Graphics.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <print>
#include <string>

void (*eng::Graphics::_mouse_callback_user)(int32, int32, int32) = nullptr;
void (*eng::Graphics::_framebuffer_resize_callback_user)(GLFWwindow *, int32,
                                                         int32) = nullptr;
void (*eng::Graphics::_cursor_position_callback_user)(GLFWwindow *, double,
                                                      double) = nullptr;
glm::vec2 eng::Graphics::cursor_position(0, 0);
GLFWwindow *eng::Graphics::_window = nullptr;

namespace eng {
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Graphics::Graphics(int width, int height, glm::vec3 clear_color, char *path)
    : _width(width), _height(height), _clear_color(clear_color) {
  // Initialize _raster with clear color
  _raster = Raster(width, height, clear_color);

  // Initialize OpenGL context and glad
  _load_glfw();
  gladLoadGL();

  // Print OpenGL version
  const GLubyte *version = glGetString(GL_VERSION);
  if (version != nullptr) {
    std::println("OpenGL {}.", reinterpret_cast<const char *>(version));
  } else {
    std::println("Failed to get OpenGL version (no valid context?)");
  }

  // Set up OpenGL state
  apply_clear_color();
  _load_raster_shader(path);

  // Connect shader sampler to texture unit 0
  glUniform1i(glGetUniformLocation(_raster_shader->ID, "texture1"), 0);

  // Create & bind texture for raster
  glGenTextures(1, &_rasterID);
  glBindTexture(GL_TEXTURE_2D, _rasterID);

  // Upload raster data to GPU
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT,
               (const void *)_raster.data.data());

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // Set up vertex data and buffers
  // VAO
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // VBO
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(_raster_vertices), _raster_vertices.data(),
               GL_STATIC_DRAW);

  // Vertex attributes
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);
}

Graphics::~Graphics() {
  _width = 0;
  _height = 0;
  delete _raster_shader;
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
}

void Graphics::resize_framebuffer(int32 width, int32 height) {
  _width = width;
  _height = height;
  _raster.resize(width, height);
  glViewport(0, 0, width, height);
}

/*Viewport coordinates: (0, 0) is top left.
 * y-axis is inverted and this method handles the inversion internally.
 */

int32 Graphics::shade_fragment(int32 x, int32 y, glm::vec3 color) {
  y = _height - 1 - y;  // Invert y-axis
  if (x >= 0 && x < _width && y >= 0 && y < _height) {
    _raster(x, y) = color;
    // _raster[(y * _width * 3) + (x * 3)] = color.x;
    // _raster[(y * _width * 3) + (x * 3) + 1] = color.y;
    // _raster[(y * _width * 3) + (x * 3) + 2] = color.z;
    return 0;
  }
  std::cerr << "ERROR: fragment out of bounds: (" << x << ", " << y << ")" << std::endl;
  return -1;
}

// ----------------------------------
// OPENGL ABSTRACTION
// ----------------------------------
void Graphics::clear_window() {
  _raster.clear(_clear_color);
  glClear(GL_COLOR_BUFFER_BIT);
}

void Graphics::draw_raster() {
  if (glfwGetKey(_window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(_window, true);
  }

  // glBindTexture(GL_TEXTURE_2D, _rasterID);
  glUseProgram(_raster_shader->ID);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, _rasterID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, _width, _height, 0, GL_RGB, GL_FLOAT,
               (const void *)_raster.data.data());

  glBindVertexArray(VAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);

  glfwSwapBuffers(_window);
  glfwPollEvents();
}

bool Graphics::should_close() { return glfwWindowShouldClose(_window) == 0; }

void Graphics::apply_clear_color() const {
  glClearColor(_clear_color.x, _clear_color.y, _clear_color.z, 1.0F);
}

int32 Graphics::register_mouse_click_method(void (*mouse_callback_user)(int, int,
                                                                        int)) {
  glfwSetCursorPosCallback(_window, _cursor_position_callback);
  glfwSetMouseButtonCallback(_window, _mouse_button_callback);
  _mouse_callback_user = mouse_callback_user;
  return 0;
}

int32 Graphics::register_cursor_position_method(
    void (*cursor_position_callback_user)(GLFWwindow *, double, double)) {
  glfwSetCursorPosCallback(_window, _cursor_position_callback);
  _cursor_position_callback_user = cursor_position_callback_user;
  return 0;
}

int32 Graphics::register_framebuffer_resize_method(
    void (*Framebuffer_resize_callback_user)(GLFWwindow *, int, int)) {
  glfwSetFramebufferSizeCallback(_window, _framebuffer_size_callback);
  _framebuffer_resize_callback_user = Framebuffer_resize_callback_user;
  return 0;
}

// ----------------------------------
// CALLBACKS
// ----------------------------------
void Graphics::_mouse_button_callback(GLFWwindow *window, int32 button, int32 action,
                                      int32 mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    (*_mouse_callback_user)(cursor_position.x / 10, cursor_position.y / 10, 0);
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    (*_mouse_callback_user)(cursor_position.x / 10, cursor_position.y / 10, 1);
  }
}

void Graphics::_cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  cursor_position = glm::vec2(xpos, ypos);
  if (_cursor_position_callback_user != nullptr) {
    (*_cursor_position_callback_user)(window, xpos / 10, ypos / 10);
  }
}

void Graphics::_framebuffer_size_callback(GLFWwindow *window, int32 width,
                                          int32 height) {
  glViewport(0, 0, width, height);
}

// ----------------------------------
// OPENGL SETUP
// ----------------------------------
void Graphics::_load_glfw() {
#if defined(__APPLE__)
  glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  _window = glfwCreateWindow(_width * 10, _height * 10, "Engine", nullptr, nullptr);

  // Check for Valid Context
  if (_window == nullptr) {
    std::cerr << "Failed to Create OpenGL Context" << std::endl;
    exit(EXIT_FAILURE);
  }
  glfwSetWindowUserPointer(_window, this);
  glfwMakeContextCurrent(_window);
}

Shader *Graphics::_load_raster_shader(char *path) {
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
    std::cerr << "ERROR: invalid path to executable: " << path << std::endl;
    exit(1);
  }
  _raster_shader = new Shader(pathVert.c_str(), pathFrag.c_str());
  return _raster_shader;
}

}  // namespace eng
