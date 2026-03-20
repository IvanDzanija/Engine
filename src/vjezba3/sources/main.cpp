// Local Headers
#include "AppState.h"
#include "FPSManager.h"
#include "Global.h"
#include "Graphics.h"
#include "Renderer.h"
#include "Shader.h"

// System Headers
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <filesystem>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>

static int32 width = 500;
static int32 height = 500;
static constexpr std::string shaders_dir = "/shaders/";
static eng::AppState app_state(width, height);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  app_state.set_framebuffer_size(width, height);
  std::cout << app_state.get_framebuffer_size().first << " "
            << app_state.get_framebuffer_size().second << std::endl;
  glViewport(0, 0, width, height);
}

void keyboard_press_callback(GLFWwindow *window, int key, int scancode, int action,
                             int mods) {
  if (action == GLFW_PRESS || action == GLFW_REPEAT) {
    switch (key) {
      case GLFW_KEY_R:
        app_state.set_active_component(eng::color_component::red);
        break;
      case GLFW_KEY_G:
        app_state.set_active_component(eng::color_component::green);
        break;
      case GLFW_KEY_B:
        app_state.set_active_component(eng::color_component::blue);
        break;
      case GLFW_KEY_UP:
        app_state.adjust_active_component(0.1f);
        break;
      case GLFW_KEY_DOWN:
        app_state.adjust_active_component(-0.1f);
        break;
    }
  }
}

void mouse_click_callback(int32 x, int32 y, int32 button) {
  if (button == 0) {  // left click
    app_state.add_point_screen(x, y);
    std::cout
  } else if (button == 1) {  // right click
    app_state.clear_geometry();
  }
}

int main(int argc, char *argv[]) {
  // Handles OpenGL context and window
  eng::Graphics screen(app_state, glm::vec3(0.15f, 0.1f, 0.1f));

  std::string dir = std::filesystem::current_path().string() + shaders_dir;
  eng::Shader indicator_shader(dir, "shader");
  eng::Shader triangles_shader(dir, "triangle_shader");

  eng::Renderer renderer(indicator_shader, triangles_shader);

  glEnable(GL_DEPTH_TEST);

  glDepthFunc(GL_LESS);

  glfwSwapInterval(0);

  FPSManager FPSManagerObject(eng::Graphics::get_window(), 60, 1.0, "Zadatak X");
  eng::Graphics::register_framebuffer_resize_method(framebuffer_size_callback);
  eng::Graphics::register_keyboard_press_method(keyboard_press_callback);
  eng::Graphics::register_mouse_click_method(mouse_click_callback);

  while (!eng::Graphics::should_close()) {
    eng::Graphics::start_frame();
    auto deltaTime = (float)FPSManagerObject.enforceFPS(false);

    renderer.render(app_state);

    eng::Graphics::end_frame();
  }

  return EXIT_SUCCESS;
}
