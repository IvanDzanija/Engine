#version 330 core

in vec3 frag_color;
in vec3 v_normal;
in vec3 v_world_pos;
in vec3 v_eye_pos;

struct Light {
  vec3 position;
  vec3 intensity;
  vec3 ambient;
};
in Light v_light;

struct Material {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
in Material v_material;

// Output
out vec4 FragColor;

void main() {
  vec3 normal = normalize(v_normal);
  vec3 L = normalize(v_light.position - v_world_pos);
  vec3 V = normalize(v_eye_pos - v_world_pos);
  vec3 R = normalize(reflect(-L, normal));

  // Ambient
  vec3 ambient = v_light.ambient * v_material.ambient;

  // Diffuse
  float diff = max(dot(normal, L), 0.0);
  vec3 diffuse = diff * v_light.intensity * v_material.diffuse;

  // Specular
  float spec = pow(max(dot(V, R), 0.0), 32.0);
  vec3 specular = spec * v_light.intensity * v_material.specular;

  vec3 light = ambient + diffuse + specular;
  FragColor = vec4(light, 1.0);
}
