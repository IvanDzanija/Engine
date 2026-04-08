#include "FPSManager.h"
#include "Global.h"
#include "Graphics.h"
#include "infra/ResourceManager.h"
#include "render/Renderer.h"
#include "render/Shader.h"

static int32 width = 500;
static int32 height = 500;
static eng::AppState app_state(width, height);

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  app_state.set_framebuffer_size(width, height);
  glViewport(0, 0, width, height);
}

int main(int argc, char *argv[]) {
  // Managers
  eng::Graphics screen(app_state, glm::vec3(0.15f, 0.1f, 0.1f));
  eng::Renderer renderer;
  FPSManager FPSManagerObject(eng::Graphics::get_window(), 60, 1.0, "Zadatak X");

  auto shader = eng::ResourceManager::get_shader("shader");
  auto model = eng::ResourceManager::get_model("glava/glava.obj");

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSwapInterval(0);

  auto head = std::make_shared<eng::Object>(model, shader);
  renderer.register_object(head);

  while (!eng::Graphics::should_close()) {
    eng::Graphics::start_frame();
    auto deltaTime = (float)FPSManagerObject.enforceFPS(false);
    renderer.render();

    eng::Graphics::end_frame();
  }

  return EXIT_SUCCESS;
}
