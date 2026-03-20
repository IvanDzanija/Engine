#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "AppState.h"
#include "Global.h"
#include "Shader.h"

namespace eng {
class Graphics {
 public:
  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  explicit Graphics(AppState &state,
                    glm::vec3 clear_color = glm::vec3(0.0f, 0.0f, 0.0f));

  ~Graphics();

  // ----------------------------------
  // GETTERS & SETTERS
  // ----------------------------------
  // Clear color
  [[nodiscard]] glm::vec3 get_clear_color() const noexcept;
  void set_clear_color(const glm::vec3 &clear_color) noexcept;
  // State
  [[nodiscard]] AppState &get_state() const noexcept;
  void set_state(AppState &state) noexcept;
  [[nodiscard]] static GLFWwindow *get_window() noexcept;

  // ----------------------------------
  // OPENGL ABSTRACTION
  // ----------------------------------
  void apply_clear_color() const;
  // static void clear_window();
  static void start_frame();
  static void end_frame();
  static bool should_close();
  static int32 register_mouse_click_method(void (*mouse_callback_user)(int, int, int));
  static int32 register_cursor_position_method(
      void (*cursor_position_callback_user)(GLFWwindow *, double, double));
  static int32 register_framebuffer_resize_method(
      void (*framebuffer_resize_callback_user)(GLFWwindow *, int, int));

 private:
  // ----------------------------------
  // STATIC FIELDS
  // ----------------------------------
  static GLFWwindow *_window;

  // ----------------------------------
  // FIELDS
  // ----------------------------------
  AppState &_state;
  glm::vec3 _clear_color;

  // ----------------------------------
  // CALLBACKS
  // ----------------------------------
  static void _mouse_button_callback(GLFWwindow *window, int32 button, int32 action,
                                     int32 mods);
  static void _cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
  static void _framebuffer_size_callback(GLFWwindow *window, int width, int height);
  static void (*_cursor_position_callback_user)(GLFWwindow *, double, double);
  static void (*_mouse_callback_user)(int32, int32, int32);
  static void (*_framebuffer_resize_callback_user)(GLFWwindow *, int32, int32);

  // ----------------------------------
  // OPENGL SETUP
  // ----------------------------------
  void _load_glfw();
};
}  // namespace eng
