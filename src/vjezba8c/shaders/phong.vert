#version 330 core
// Input
layout(location = 0) in vec3 a_pos;
layout(location = 1) in vec3 a_norm;

// Matrix
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

struct Light {
  vec3 position;
  vec3 intensity;
  vec3 ambient;
};
uniform Light u_light;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material u_material;

uniform vec3 u_eye_pos;

// Output
out Material v_material;
out Light v_light;
out vec3 v_normal;
out vec3 v_world_pos;
out vec3 v_eye_pos;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
  v_material = u_material;
  v_light = u_light;
  v_normal = transpose(inverse(mat3(u_model))) * a_norm;
  v_world_pos = vec3(u_model * vec4(a_pos, 1.0));
  v_eye_pos = u_eye_pos;
}
