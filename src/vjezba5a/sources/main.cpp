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
  eng::Graphics screen(app_state, glm::vec3(0.15F, 0.1F, 0.1F));
  eng::Renderer renderer;
  FPSManager FPSManagerObject(eng::Graphics::get_window(), 60, 1.0, "Zadatak X");

  auto camera = std::make_shared<eng::Camera>();
  camera->set_position({3.0F, 4.0F, 1.0F});
  camera->change_orientation({0.0F, 0.0F, 0.0F});

  renderer.link_camera(camera);

  auto shader = eng::ResourceManager::get_shader("shader");
  auto model = eng::ResourceManager::get_model("kocka.obj");

  auto obj1 = std::make_shared<eng::Object>(model, shader);
  // obj1->set_position({-1.5F, 0.0F, 0.0F});
  renderer.register_object(obj1);

  // auto obj2 = std::make_shared<eng::Object>(model, shader);
  // obj2->set_position({1.5F, 0.0F, 0.0F});
  // obj2->set_scale({0.5F, 0.5F, 0.5F});
  // renderer.register_object(obj2);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSwapInterval(0);

  while (!eng::Graphics::should_close()) {
    eng::Graphics::start_frame();
    auto deltaTime = (float)FPSManagerObject.enforceFPS(false);
    renderer.render();

    eng::Graphics::end_frame();
  }

  return EXIT_SUCCESS;
}
