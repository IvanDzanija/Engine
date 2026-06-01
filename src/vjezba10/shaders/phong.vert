#version 330 core
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_norm;
layout(location = 2) in vec2 a_tex_coords;

// Matrix
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform mat4 u_light_view;
uniform mat4 u_light_projection;

out vec3 v_normal;
out vec3 v_world_pos;
out vec2 v_tex_coords;
out vec4 v_frag_pos_light_space;

void main() {
  v_normal = transpose(inverse(mat3(u_model))) * a_norm;
  v_world_pos = vec3(u_model * vec4(a_pos, 1.0));
  v_tex_coords = a_tex_coords;
  v_frag_pos_light_space = u_light_projection * u_light_view * vec4(v_world_pos, 1.0);
  gl_Position = u_projection * u_view * vec4(v_world_pos, 1.0);
}
