#version 330 core
layout(lines_adjacency) in;
layout(line_strip, max_vertices = 100) out;

in vec3 color[];
out vec3 frag_color;

uniform mat4 u_view;
uniform mat4 u_projection;
uniform int u_segments = 100;

void main() {
  vec3 p0 = gl_in[0].gl_Position.xyz;
  vec3 p1 = gl_in[1].gl_Position.xyz;
  vec3 p2 = gl_in[2].gl_Position.xyz;
  vec3 p3 = gl_in[3].gl_Position.xyz;

  frag_color = color[0];

  float delta = 1.0 / float(u_segments);

  for (int i = 0; i <= u_segments; ++i) {
    float t = float(i) * delta;
    float t_inv = 1.0 - t;
    vec3 pos = (t_inv * t_inv * t_inv) * p0 + (3.0 * t * t_inv * t_inv) * p1 +
               (3.0 * t * t * t_inv) * p2 + (t * t * t) * p3;

    gl_Position = u_projection * u_view * vec4(pos, 1.0);
    EmitVertex();
  }
  EndPrimitive();
}
