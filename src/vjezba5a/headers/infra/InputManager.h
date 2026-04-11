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
static float speed = 2.5F;

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

void mouse_callback(GLFWwindow *, double xpos, double ypos) {
  // optional: implement camera rotation
}

void scroll_callback(GLFWwindow *, double xoffset, double yoffset) {
  // optional: zoom
}

void mouse_button_callback(GLFWwindow *, int button, int action, int mods) {
  // optional
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
