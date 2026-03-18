#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "Global.h"
#include "Raster.h"
#include "Shader.h"

namespace eng {
class Graphics {
 public:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  static glm::vec2 cursor_position;

  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  explicit Graphics(int width, int height, glm::vec3 clear_color, char *path);
  ~Graphics();

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  [[nodiscard]] int32 get_width() const noexcept { return _width; }
  [[nodiscard]] int32 get_height() const noexcept { return _height; }
  [[nodiscard]] std::pair<int32, int32> get_framebuffer_size() const noexcept {
    return {_width, _height};
  }
  void resize_framebuffer(int width, int height);
  int32 shade_fragment(int32 x, int32 y, glm::vec3 color = glm::vec3(1.0F));

  void register_user_state(void *data) { _user_state = data; }
  [[nodiscard]] void *get_user_state() { return _user_state; }
  [[nodiscard]] const void *get_user_state() const { return _user_state; }

  // ----------------------------------
  // OPENGL ABSTRACTION
  // ----------------------------------
  void clear_window();
  void draw_raster();  // pozovi funkcije u OpenGL-u koje iscrtaju raster
  void apply_clear_color() const;
  static bool should_close();
  static int32 register_mouse_click_method(void (*mouse_callback_user)(int, int, int));
  static int32 register_cursor_position_method(
      void (*cursor_position_callback_user)(GLFWwindow *, double, double));
  static int32 register_framebuffer_resize_method(
      void (*framebuffer_resize_callback_user)(GLFWwindow *, int, int));

 private:
  // ----------------------------------
  // CONSTANTS
  // ----------------------------------
  static constexpr uint32 _CHANNELS = 3;
  static constexpr uint32 _RASTER_VERTICES_COUNT = 20;

  // ----------------------------------
  // FIELDS
  // ----------------------------------
  int32 _width;
  int32 _height;
  glm::vec3 _clear_color;
  Raster _raster;
  uint32 _rasterID;
  void *_user_state = nullptr;
  std::array<float, _RASTER_VERTICES_COUNT> _raster_vertices = {
      //  verticesCoord	textureCoord
      -1, -1, 0, 0, 0, 1, -1, 0, 1, 0, -1, 1, 0, 0, 1, 1, 1, 0, 1, 1};

  static GLFWwindow *_window;

  unsigned int VAO;
  unsigned int VBO;

  Shader *_raster_shader;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  static void _mouse_button_callback(GLFWwindow *window, int32 button, int32 action,
                                     int32 mods);
  static void _cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
  static void _framebuffer_size_callback(GLFWwindow *window, int width, int height);
  static void (*_cursor_position_callback_user)(GLFWwindow *, double, double);
  static void (*_mouse_callback_user)(int32, int32, int32);
  static void (*_framebuffer_resize_callback_user)(GLFWwindow *, int32, int32);
  Shader *_load_raster_shader(char *path);

  // ----------------------------------
  // OPENGL SETUP
  // ----------------------------------
  void _load_glfw();
};
}  // namespace eng
