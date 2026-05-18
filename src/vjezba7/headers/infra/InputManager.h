#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#pragma once
#include <memory>

#include "Graphics.h"
#include "math/BezierBuilder.h"
#include "math/Transform.h"

extern eng::AppState app_state;
namespace eng::input {
// Curve pathing controll
struct CurveControl {
  std::vector<CurveVertex> vertices;
  size_t index = 0;
  bool first_space = true;
  bool forward = true;
  bool added_control_point = false;
};

static GLFWwindow *window = nullptr;
static std::shared_ptr<Transform> _target = nullptr;
static std::array<bool, 1024> keys{};
static std::shared_ptr<BezierBuilder> _bezier_builder = nullptr;

static CurveControl curve_control;

void _camera_forward() {
  auto curr_pos = curve_control.vertices[curve_control.index].coords;
  _target->set_position(curr_pos);

  if (curve_control.index > 0) {
    auto next_pos = curve_control.vertices[curve_control.index - 1].coords;
    _target->change_orientation(next_pos, glm::vec3(0.0F, 1.0F, 0.0F));
  }

  --curve_control.index;
}
void _camera_backward() {
  auto curr_pos = curve_control.vertices[curve_control.index].coords;
  _target->set_position(curr_pos);

  if (curve_control.index < curve_control.vertices.size() - 1) {
    auto next_pos = curve_control.vertices[curve_control.index + 1].coords;
    _target->change_orientation(next_pos, glm::vec3(0.0F, 1.0F, 0.0F));
  }

  ++curve_control.index;
}

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
  // optional: zoom
}

void mouse_button_callback(GLFWwindow *, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
    _bezier_builder->add_control_point(_target->get_position());
    curve_control.added_control_point = true;
  }
}

void register_movable(std::shared_ptr<Transform> target) {
  _target = std::move(target);
}

void register_bezier_builder(std::shared_ptr<BezierBuilder> builder) {
  std::println("Registered BezierBuilder");
  _bezier_builder = std::move(builder);
}

bool bezier_control_point_added() {
  if (curve_control.added_control_point) {
    curve_control.added_control_point = false;
    curve_control.first_space = true;
    return true;
  }
  return false;
}

void poll_events(float deltaTime) {
  glfwPollEvents();

  if (_target == nullptr) {
    return;
  }
  float velocity = speed * deltaTime;

  if (keys[GLFW_KEY_A]) {
    // std::println("A pressed");
    _target->move_local_x(-velocity);
  }
  if (keys[GLFW_KEY_D]) {
    // std::println("D pressed");
    _target->move_local_x(velocity);
  }
  if (keys[GLFW_KEY_Q]) {
    // std::println("Q pressed");
    _target->move_local_y(velocity);
  }
  if (keys[GLFW_KEY_E]) {
    // std::println("E pressed");
    _target->move_local_y(-velocity);
  }
  if (keys[GLFW_KEY_W]) {
    // std::println("W pressed");
    _target->move_local_z(-velocity);
  }
  if (keys[GLFW_KEY_S]) {
    // std::println("S pressed");
    _target->move_local_z(velocity);
  }
  if (keys[GLFW_KEY_SPACE]) {
    // std::println("SPACE pressed");
    if (curve_control.first_space) {
      // On click move forward
      curve_control.vertices = _bezier_builder->build_interpolate(5000);
      if (curve_control.vertices.empty()) {
        std::println("No control points added yet.");
        return;
      }
      curve_control.first_space = false;
      curve_control.forward = true;
      curve_control.index = curve_control.vertices.size() - 1;
      _camera_forward();
    } else if (curve_control.forward && curve_control.index == 0) {
      // Move backward once end is reached
      curve_control.forward = false;
      _camera_backward();
    } else if (!curve_control.forward &&
               curve_control.index == curve_control.vertices.size() - 1) {
      // Move forward once start is reached
      curve_control.forward = true;
      _camera_forward();
    } else if (curve_control.forward) {
      _camera_forward();
    } else {
      _camera_backward();
    }
  }
}

}  // namespace eng::input

#endif
