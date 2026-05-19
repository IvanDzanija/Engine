#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 world_pos[];
in vec3 color[];
out vec3 frag_color;

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

void main() {
  vec3 v0 = world_pos[0];
  vec3 v1 = world_pos[1];
  vec3 v2 = world_pos[2];
  vec3 normal = normalize(cross(v1 - v0, v2 - v0));

  vec3 center = (v0 + v1 + v2) / 3.0;

  vec3 L = normalize(u_light.position - center);
  vec3 V = normalize(u_eye_pos - center);
  vec3 R = reflect(-L, normal);

  // Ambient
  vec3 ambient = u_light.ambient * u_material.ambient;

  // Diffuse
  float diff = max(dot(normal, L), 0.0);
  vec3 diffuse = diff * u_light.intensity * u_material.diffuse;

  // Specular
  float spec = pow(max(dot(V, R), 0.0), 32.0);
  vec3 specular = spec * u_light.intensity * u_material.specular;

  vec3 light = ambient + diffuse + specular;
  frag_color = light;

  for (int i = 0; i < 3; ++i) {
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();
}
