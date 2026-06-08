#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#pragma once
#include <memory>

#include "Graphics.h"
#include "math/Transform.h"

extern eng::AppState app_state;

namespace eng::input {

static GLFWwindow *window = nullptr;
static std::shared_ptr<Transform> _target = nullptr;

static std::array<bool, 1024> keys{};

// Mouse settings
constexpr static float mouse_sensitivity = 100.F;
constexpr static float speed = 2.5F;

// Framebuffer
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  app_state.set_framebuffer_size(width, height);
  glViewport(0, 0, width, height);
}

// Keyboard
void keyboard_press_callback(GLFWwindow *window, int key, int scancode, int action,
                             int mods) {
  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      keys[key] = true;
    } else if (action == GLFW_RELEASE) {
      keys[key] = false;
    }
  }
}

// Cursor
void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  auto prev_currson_postion = app_state.get_cursor_position();
  app_state.set_cursor_position(xpos, ypos);  // Converts to NDC
  auto curr_cursor_position = app_state.get_cursor_position();
  xpos = curr_cursor_position.x;
  ypos = curr_cursor_position.y;

  static bool first_mouse = true;
  if (first_mouse) {
    first_mouse = false;
    return;
  }
  float xoffset = (xpos - prev_currson_postion.x) * mouse_sensitivity;
  float yoffset = (ypos - prev_currson_postion.y) * mouse_sensitivity;

  _target->rotateFPS(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *, double xoffset, double yoffset) {
  // Empty
}

void mouse_button_callback(GLFWwindow *, int button, int action, int mods) {
  // Empty
}

void register_movable(std::shared_ptr<Transform> target) {
  _target = std::move(target);
}

void poll_events(float deltaTime) {
  glfwPollEvents();

  if (_target == nullptr) {
    return;
  }
  float velocity = speed * deltaTime;

  if (keys[GLFW_KEY_A]) {
    std::cout << "A pressed" << std::endl;
    _target->move_local_x(-velocity);
  }
  if (keys[GLFW_KEY_D]) {
    std::cout << "D pressed" << std::endl;
    _target->move_local_x(velocity);
  }
  if (keys[GLFW_KEY_Q]) {
    std::cout << "Q pressed" << std::endl;
    _target->move_local_y(velocity);
  }
  if (keys[GLFW_KEY_E]) {
    std::cout << "E pressed" << std::endl;
    _target->move_local_y(-velocity);
  }
  if (keys[GLFW_KEY_W]) {
    std::cout << "W pressed" << std::endl;
    _target->move_local_z(-velocity);
  }
  if (keys[GLFW_KEY_S]) {
    std::cout << "S pressed" << std::endl;
    _target->move_local_z(velocity);
  }
}

// ---------------- CALLBACKS ----------------

}  // namespace eng::input

#endif
