#include <glm/gtc/epsilon.hpp>

#include "FPSManager.h"
#include "Global.h"
#include "Graphics.h"
#include "infra/InputManager.h"
#include "infra/ResourceManager.h"
#include "math/BezierBuilder.h"
#include "render/Curve.h"
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
  eng::Graphics::register_mouse_click_method(eng::input::mouse_button_callback);
  eng::Graphics::register_polling_method(eng::input::poll_events);

  eng::Renderer renderer;
  FPSManager FPSManagerObject(eng::Graphics::get_window(), 60, 1.0, "Zadatak X");

  // One shader to rule them all
  auto default_shader = eng::ResourceManager::get_shader("shader");
  auto bezier_shader =
      eng::ResourceManager::get_shader("bezier", "bezier", "bezier", "bezier");
  auto light_shader = eng::ResourceManager::get_shader("light_shader", "light_shader",
                                                       "light_shader", "light_shader");
  auto gouraud_shader = eng::ResourceManager::get_shader("gouraud");
  auto phong_shader = eng::ResourceManager::get_shader("phong");

  // Global axes
  // auto global_axes = std::make_shared<eng::Object>(default_shader);
  // global_axes->set_scale({5.0F, 5.0F, 5.0F});
  // renderer.register_object(global_axes);

  // Camera
  auto camera = std::make_shared<eng::Camera>();
  camera->set_position({1.0F, 1.0F, 6.0F});
  camera->change_orientation({0.0F, 0.0F, 0.0F});
  renderer.link_camera(camera);
  eng::input::register_movable(camera);

  // Light
  auto light = std::make_shared<eng::Light>();
  light->set_position({-1.5F, 0.0F, 1.5F});
  renderer.register_light_source(light);
  auto light_model = eng::ResourceManager::get_model("bird", "bird.obj");
  auto light_object = std::make_shared<eng::Object>(light_model, light_shader);
  light_object->set_position(light->get_position());
  light_object->set_scale({0.2F, 0.2F, 0.2F});
  renderer.register_object(light_object);
  // eng::input::register_movable(light);

  // Kocka
  std::shared_ptr<eng::Model> model =
      eng::ResourceManager::get_model("glava", "glava.obj");

  // Kocka 1
  auto obj1 = std::make_shared<eng::Object>(model, phong_shader);
  obj1->set_position({-1.5F, 0.0F, 0.0F});
  renderer.register_object(obj1);
  // eng::input::register_movable(obj1);

  // Kocka 2
  auto obj2 = std::make_shared<eng::Object>(model, light_shader);
  obj2->set_position({1.5F, 0.0F, 0.0F});
  obj2->set_scale({0.5F, 0.5F, 0.5F});
  renderer.register_object(obj2);

  // Kocka 3
  auto obj3 = std::make_shared<eng::Object>(model, gouraud_shader);
  obj3->set_scale({0.5F, 0.5F, 0.5F});
  obj3->set_position({0.0F, 0.0F, 0.0F});
  renderer.register_object(obj3);

  // Bezier builder
  auto bezier_builder = std::make_shared<eng::BezierBuilder>();
  eng::input::register_bezier_builder(bezier_builder);
  auto bezier_object1 = std::make_shared<eng::Object>(default_shader);
  auto bezier_object2 = std::make_shared<eng::Object>(bezier_shader);

  auto control_curve = std::make_shared<eng::Curve>(bezier_builder->build_control());
  // auto approx_curve =
  // std::make_shared<eng::Curve>(bezier_builder->build_approximate());
  auto approx_curve = std::make_shared<eng::Curve>(bezier_builder->forward_to_gpu());
  approx_curve->set_draw_mode(GL_LINES_ADJACENCY);
  auto interp_curve =
      std::make_shared<eng::Curve>(bezier_builder->build_full_interpolate());

  bezier_object1->add_renderable(control_curve);
  bezier_object1->add_renderable(interp_curve);
  bezier_object1->use_uniform_color(false);

  bezier_object2->add_renderable(approx_curve);
  bezier_object2->use_uniform_color(false);

  renderer.register_object(bezier_object1);
  renderer.register_object(bezier_object2);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glfwSwapInterval(0);
  while (!eng::Graphics::should_close()) {
    auto deltaTime = (float)FPSManagerObject.enforceFPS(false);
    eng::Graphics::start_frame(deltaTime);
    if (eng::input::bezier_control_point_added()) {
      std::cout << "Control point added" << std::endl;
      control_curve->update_vertices(bezier_builder->build_control());
      // approx_curve->update_vertices(bezier_builder->build_approximate());
      approx_curve->update_vertices(bezier_builder->forward_to_gpu());
      interp_curve->update_vertices(bezier_builder->build_full_interpolate());
    }
    renderer.render();

    eng::Graphics::end_frame();
  }

  return EXIT_SUCCESS;
}
