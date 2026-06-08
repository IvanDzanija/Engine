#version 330 core

in vec3 v_normal;
in vec3 v_world_pos;
in vec2 v_tex_coords;

struct Light {
  vec3 position;
  vec3 intensity;
  vec3 ambient;
};
uniform Light u_light;

struct Material {
  sampler2D diffuse1;
  sampler2D specular1;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  float shininess;
};
uniform Material u_material;

uniform vec3 u_eye_pos;

out vec4 FragColor;

void main() {
  vec3 normal = normalize(v_normal);
  vec3 L = normalize(u_light.position - v_world_pos);
  vec3 V = normalize(u_eye_pos - v_world_pos);
  vec3 R = normalize(reflect(-L, normal));

  vec3 tex_diffuse = texture(u_material.diffuse1, v_tex_coords).rgb;
  // vec3 tex_specular = texture(u_material.specular1, v_tex_coords).rgb;
  vec3 tex_specular = vec3(1.0);

  vec3 final_ambient_color = tex_diffuse * u_material.ambient;
  vec3 final_diffuse_color = tex_diffuse * u_material.diffuse;
  vec3 final_specular_color = tex_specular * u_material.specular;

  vec3 ambient = u_light.ambient * final_ambient_color;

  float diff_factor = max(dot(normal, L), 0.0);
  vec3 diffuse = diff_factor * u_light.intensity * final_diffuse_color;

  float spec_factor = pow(max(dot(V, R), 0.0), u_material.shininess);
  vec3 specular = spec_factor * u_light.intensity * final_specular_color;

  vec3 final_lighting = ambient + diffuse + specular;

  FragColor = vec4(final_lighting, 1.0);
}
