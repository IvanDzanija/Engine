#version 330 core
// Shader for face culling in scene space

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

in vec3 color[];      // Input color from vertex shader
out vec3 frag_color;  // Output color to fragment shader

uniform vec3 u_eye_pos;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
  vec3 p0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
  vec3 p1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
  vec3 p2 = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;

  vec3 normal = normalize(cross(p1 - p0, p2 - p0));
  vec3 c = (p0 + p1 + p2) / 3.0;
  vec3 e = normalize(u_eye_pos - c);

  if (dot(normal, e) > 0.0) {
    // Emit the triangle edges
    gl_Position = u_projection * u_view * gl_in[0].gl_Position;
    frag_color = color[0];
    EmitVertex();
    gl_Position = u_projection * u_view * gl_in[1].gl_Position;
    frag_color = color[1];
    EmitVertex();
    gl_Position = u_projection * u_view * gl_in[2].gl_Position;
    frag_color = color[2];
    EmitVertex();
    // Close the loop
    gl_Position = u_projection * u_view * gl_in[0].gl_Position;
    frag_color = color[0];
    EmitVertex();
    EndPrimitive();
  }
}
