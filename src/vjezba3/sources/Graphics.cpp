#pragma once
#include "Graphics.h"

#include <cmath>
#include <filesystem>
#include <iostream>
#include <print>
#include <string>

namespace eng {
void (*Graphics::_mouse_callback_user)(int32, int32, int32) = nullptr;
void (*Graphics::_framebuffer_resize_callback_user)(GLFWwindow *, int32,
                                                    int32) = nullptr;
void (*Graphics::_cursor_position_callback_user)(GLFWwindow *, double,
                                                 double) = nullptr;
GLFWwindow *Graphics::_window = nullptr;
// ----------------------------------
// CONSTRUCTORS
// ----------------------------------
Graphics::Graphics(AppState &state, glm::vec3 clear_color) : _clear_color(clear_color) {
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
}

Graphics::~Graphics() {
  _width = 0;
  _height = 0;
  delete _raster_shader;
  glDeleteBuffers(1, &VBO);
  glDeleteVertexArrays(1, &VAO);

  glfwTerminate();
}

// ----------------------------------
// GETTERS & SETTERS
// ----------------------------------
// Clear color
[[nodiscard]] glm::vec3 Graphics::get_clear_color() const noexcept {
  return _clear_color;
}
void Graphics::set_clear_color(const glm::vec3 &clear_color) noexcept {
  _clear_color = clear_color;
  apply_clear_color();
}

// ----------------------------------
// OPENGL ABSTRACTION
// ----------------------------------

void Graphics::end_frame() {
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
    (*_mouse_callback_user)(0);
  }
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    (*_mouse_callback_user)(1);
  }
}

void Graphics::_cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  if (_cursor_position_callback_user != nullptr) {
    (*_cursor_position_callback_user)(window, xpos, ypos);
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

}  // namespace eng
