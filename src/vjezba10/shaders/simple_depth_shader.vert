#version 330 core
layout(location = 0) in vec3 a_pos;

// Matrix
uniform mat4 u_model;
uniform mat4 u_light_view;
uniform mat4 u_light_projection;

void main() {
  gl_Position = u_light_projection * u_light_view * u_model * vec4(a_pos, 1.0);
}
