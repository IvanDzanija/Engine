#include <glm/gtc/epsilon.hpp>

#include "FPSManager.h"
#include "Global.h"
#include "Graphics.h"
#include "infra/InputManager.h"
#include "infra/ResourceManager.h"
#include "render/Renderer.h"

static int32 width = 500;
static int32 height = 500;
eng::AppState app_state(width, height);

int main(int argc, char *argv[]) {
  // ----------------------------------
  // MANAGERS
  // ----------------------------------
  // Graphics
  eng::Graphics screen(app_state, glm::vec3(0.15F, 0.1F, 0.1F));
  eng::Graphics::register_framebuffer_resize_method(
      eng::input::framebuffer_size_callback);
  eng::Graphics::register_keyboard_press_method(eng::input::keyboard_press_callback);
  eng::Graphics::register_cursor_position_method(eng::input::cursor_position_callback);
  eng::Graphics::register_polling_method(eng::input::poll_events);

  eng::Renderer renderer;
  FPSManager FPSManagerObject(eng::Graphics::get_window(), 60, 1.0, "Zadatak X");

  // One shader to rule them all
  auto default_shader = eng::ResourceManager::get_shader("shader");
  auto kocka1_shader = eng::ResourceManager::get_shader(
      "kocka1", "projection_culling", "projection_culling", "projection_culling");

  auto kocka2_shader = eng::ResourceManager::get_shader(
      "kocka2", "scene_culling", "scene_culling", "scene_culling");

  // Global axes
  // auto global_axes = std::make_shared<eng::Object>(default_shader);
  // global_axes->set_scale({5.0F, 5.0F, 5.0F});
  // renderer.register_object(global_axes);

  // Camera
  auto camera = std::make_shared<eng::Camera>();
  camera->set_position({3.0F, 4.0F, 1.0F});
  camera->change_orientation({0.0F, 0.0F, 0.0F});
  renderer.link_camera(camera);
  eng::input::register_movable(camera);

  // Kocka
  auto model = eng::ResourceManager::get_model("kocka.obj");

  // Kocka 1
  auto obj1 = std::make_shared<eng::Object>(model, kocka1_shader);
  obj1->set_position({-1.5F, 0.0F, 0.0F});
  renderer.register_object(obj1);
  // eng::input::register_movable(obj1);

  // Kocka 2
  auto obj2 = std::make_shared<eng::Object>(model, kocka2_shader);
  obj2->set_position({1.5F, 0.0F, 0.0F});
  obj2->set_scale({0.5F, 0.5F, 0.5F});
  renderer.register_object(obj2);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glfwSwapInterval(0);

  while (!eng::Graphics::should_close()) {
    auto deltaTime = (float)FPSManagerObject.enforceFPS(false);
    eng::Graphics::start_frame(deltaTime);
    renderer.render();

    eng::Graphics::end_frame();
  }

  return EXIT_SUCCESS;
}
