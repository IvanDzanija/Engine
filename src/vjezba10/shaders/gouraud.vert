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
out vec3 frag_color;

void main() {
  gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
  vec3 model_normal = normalize(transpose(inverse(mat3(u_model))) * a_norm);
  vec3 world_pos = vec3(u_model * vec4(a_pos, 1.0));

  vec3 L = normalize(u_light.position - world_pos);
  vec3 V = normalize(u_eye_pos - world_pos);
  vec3 R = normalize(reflect(-L, model_normal));

  // Ambient
  vec3 ambient = u_light.ambient * u_material.ambient;

  // Diffuse
  float diff = max(dot(model_normal, L), 0.0);
  vec3 diffuse = diff * u_light.intensity * u_material.diffuse;

  // Specular
  float spec = pow(max(dot(V, R), 0.0), 32.0);
  vec3 specular = spec * u_light.intensity * u_material.specular;

  vec3 light = ambient + diffuse + specular;
  frag_color = light;
}
