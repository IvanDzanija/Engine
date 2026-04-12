#include <glm/gtc/epsilon.hpp>

#include "FPSManager.h"
#include "Global.h"
#include "Graphics.h"
#include "infra/InputManager.h"
#include "infra/ResourceManager.h"
#include "math/TransformGenerator.h"
#include "render/Renderer.h"
#include "render/Shader.h"

static int32 width = 500;
static int32 height = 500;
eng::AppState app_state(width, height);
bool nearlyEqual(const glm::mat4 &a, const glm::mat4 &b, float eps) {
  for (int i = 0; i < 4; ++i)
    if (!glm::all(glm::epsilonEqual(a[i], b[i], eps))) return false;
  return true;
}

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
  auto shader = eng::ResourceManager::get_shader("shader");

  // Global axes
  auto global_axes = std::make_shared<eng::Object>(shader);
  global_axes->set_scale({5.0F, 5.0F, 5.0F});
  renderer.register_object(global_axes);

  // Camera
  auto camera = std::make_shared<eng::Camera>();
  camera->set_position({3.0F, 4.0F, 1.0F});
  camera->change_orientation({0.0F, 0.0F, 0.0F});
  renderer.link_camera(camera);
  eng::input::register_movable(camera);

  // TESTING
  auto mat = glm::translate(glm::mat4(1.0F), glm::vec3(9.0F, 5.0F, -2.0F));
  eng::matrix_print(mat);
  auto mat1 = eng::TransformGenerator::translate_3D(glm::vec3(9.0F, 5.0F, -2.0F));
  eng::matrix_print(mat1);
  assert(mat == mat1);

  mat = glm::rotate(glm::mat4(1.F), glm::radians(31.0F), glm::vec3(0.5F, 1.0F, 1.0F));
  mat1 = eng::TransformGenerator::rotate_3D(glm::vec3(0.5F, 1.0F, 1.0F), 31.0F);
  eng::matrix_print(mat);
  eng::matrix_print(mat1);
  assert(nearlyEqual(mat, mat1, 0.0001F));

  mat = glm::scale(glm::mat4(1.F), glm::vec3(0.5F, 0.5F, 0.5F));
  mat1 = eng::TransformGenerator::scale_3D(glm::vec3(0.5F, 0.5F, 0.5F));
  eng::matrix_print(mat);
  eng::matrix_print(mat1);
  assert(nearlyEqual(mat, mat1, 0.0001F));

  mat = glm::lookAt(glm::vec3(1.0F, 2.0F, 3.0F), glm::vec3(0.0F, 0.0F, 0.0F),
                    glm::vec3(0.0F, 1.0F, 0.0F));
  mat1 = eng::TransformGenerator::look_at_matrix(glm::vec3(1.0F, 2.0F, 3.0F),
                                                 glm::vec3(0.0F, 0.0F, 0.0F),
                                                 glm::vec3(0.0F, 1.0F, 0.0F));
  eng::matrix_print(mat);
  eng::matrix_print(mat1);
  assert(nearlyEqual(mat, mat1, 0.0001F));

  // exit(1);

  // Kocka
  auto model = eng::ResourceManager::get_model("kocka.obj");

  // Kocka 1
  auto obj1 = std::make_shared<eng::Object>(model, shader);
  obj1->set_position({-1.5F, 0.0F, 0.0F});
  renderer.register_object(obj1);
  // eng::input::register_movable(obj1);

  // Kocka 2
  auto obj2 = std::make_shared<eng::Object>(model, shader);
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
