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

void framebuffer_size_callback(GLFWwindow *window, int Width, int Height) {
  app_state.set_framebuffer_size(Width, Height);
  glViewport(0, 0, width, height);
}

void keyboard_press_callback() {}

int main(int argc, char *argv[]) {
  // Handles OpenGL context and window
  eng::Graphics screen(app_state, glm::vec3(0.15f, 0.1f, 0.1f));

  std::string dir = std::filesystem::current_path().string() + shaders_dir;
  eng::Shader shader(dir, "shader");
  eng::Renderer renderer(shader);

  glEnable(GL_DEPTH_TEST);

  glDepthFunc(GL_LESS);

  glfwSwapInterval(0);  // ne cekaj nakon iscrtavanja (vsync)

  FPSManager FPSManagerObject(eng::Graphics::get_window(), 60, 1.0, "Zadatak X");
  eng::Graphics::register_framebuffer_resize_method(framebuffer_size_callback);

  while (!eng::Graphics::should_close()) {
    eng::Graphics::start_frame();
    auto deltaTime = (float)FPSManagerObject.enforceFPS(false);

    renderer.render(app_state);

    eng::Graphics::end_frame();
  }

  return EXIT_SUCCESS;
}
