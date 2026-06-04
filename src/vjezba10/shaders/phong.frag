#version 330 core

in vec3 v_normal;
in vec3 v_world_pos;
in vec2 v_tex_coords;
in vec4 v_frag_pos_light_space;

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

uniform sampler2D shadow_map;
uniform vec3 u_eye_pos;

out vec4 FragColor;

float calculate_shadow(vec4 light_space_pos) {
  // We are using ortho now so this is not needed, but just for completeness
  vec3 proj_coords = light_space_pos.xyz / light_space_pos.w;
  proj_coords = proj_coords * 0.5 + 0.5;

  if (proj_coords.z > 1.0) {
    return 0.0;
  }

  float closest_depth = texture(shadow_map, proj_coords.xy).r;
  float current_depth = proj_coords.z;

  float bias = 0.001;  // Adjust this
  float shadow = current_depth - bias > closest_depth ? 1.0 : 0.0;

  return shadow;
}

void main() {
  vec3 normal = normalize(v_normal);
  vec3 L = normalize(u_light.position - v_world_pos);
  vec3 V = normalize(u_eye_pos - v_world_pos);
  vec3 R = normalize(reflect(-L, normal));

  vec3 tex_diffuse = texture(u_material.diffuse1, v_tex_coords).rgb;
  vec3 tex_specular = texture(u_material.specular1, v_tex_coords).rgb;

  vec3 final_ambient_color = tex_diffuse * u_material.ambient;
  vec3 final_diffuse_color = tex_diffuse * u_material.diffuse;
  vec3 final_specular_color = tex_specular * u_material.specular;

  float shadow = calculate_shadow(v_frag_pos_light_space);

  vec3 ambient = u_light.ambient * final_ambient_color;

  float diff_factor = max(dot(normal, L), 0.0);
  vec3 diffuse = diff_factor * u_light.intensity * final_diffuse_color * (1.0 - shadow);

  float spec_factor = pow(max(dot(V, R), 0.0), u_material.shininess);
  vec3 specular =
      spec_factor * u_light.intensity * final_specular_color * (1.0 - shadow);

  vec3 final_lighting = ambient + diffuse + specular;

  FragColor = vec4(final_lighting, 1.0);
}
