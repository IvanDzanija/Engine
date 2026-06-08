#version 330 core
// Input
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;

// Eye position
uniform vec3 u_eye_pos;

// Matrix
uniform mat4 u_model;

// Color
uniform vec3 u_color;

// Color switch
uniform bool u_use_vertex_color;

// Output
out vec3 color;

void main() {
  if (u_use_vertex_color) {
    color = aCol;
  } else {
    color = u_color;
  }

  gl_Position = u_model * vec4(aPos, 1.0);
}
