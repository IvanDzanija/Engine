#pragma once

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <fstream>
#include <glm/glm.hpp>
#include <sstream>
#include <string>
#include <vector>

#include "Global.hpp"
#include "Shader.h"

namespace eng {
class Graphics {
 public:
  // ----------------------------------
  // FIELDS
  // ----------------------------------
  static glm::vec2 cursor_position;

  // ----------------------------------
  // CONSTRUCTORS
  // ----------------------------------
  explicit Graphics(int width, int height, glm::vec3 clearColor, char *path);
  ~Graphics();

  int32 light_fragment(int32 x, int32 y, glm::vec3 color);
  int osvijetliFragment(int32 x, int32 y, glm::vec3 color);
  int osvijetliFragment(int32 x, int32 y);

  void pobrisiProzor();

  // pozovi funkcije u OpenGL-u koje iscrtaju raster
  void iscrtajRaster();

  static bool trebaZatvoriti();

  static int registrirajFunkcijuZaKlikMisa(void (*mouse_callback_user)(int, int, int));

 private:
  // ----------------------------------
  // CONSTANTS
  // ----------------------------------
  static constexpr uint32 _CHANNELS = 3;
  static constexpr uint32 _RASTER_VERTICES_COUNT = 20;

  // ----------------------------------
  // FIELDS
  // ----------------------------------
  int32 _width;
  int32 _height;
  glm::vec3 _clear_color;
  float *_raster;
  std::array<float, _RASTER_VERTICES_COUNT> _raster_vertices = {
      //  verticesCoord	textureCoord
      -1, -1, 0, 0, 0, 1, -1, 0, 1, 0, -1, 1, 0, 0, 1, 1, 1, 0, 1, 1};

  static GLFWwindow *window;

  unsigned int _rasterID;
  unsigned int VAO;
  unsigned int VBO;

  Shader *_raster_shader;

  // ----------------------------------
  // METHODS
  // ----------------------------------
  static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                    int mods);
  static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
  static void (*mouse_callback_user)(int, int, int);

  void load_glfw();
  Shader *loadRasterShader(char *path);
};
}  // namespace eng
