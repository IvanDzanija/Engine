#version 330 core
// Input
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;

// Matrix
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

// Color
uniform vec3 u_color;

// Color switch
uniform bool u_use_vertex_color;

// Output
out vec3 color;
out vec3 world_pos;

void main() {
  if (u_use_vertex_color) {
    color = aCol;
  } else {
    color = u_color;
  }

  world_pos = vec3(u_model * vec4(aPos, 1.0));
  gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}
