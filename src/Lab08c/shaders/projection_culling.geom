#version 330 core
// Shader for face culling in projection space

layout(triangles) in;
layout(line_strip, max_vertices = 4) out;

in vec3 color[];      // Input color from vertex shader
out vec3 frag_color;  // Output color to fragment shader

void main() {
  // Checking in projection space is done with just CCW and CW check
  vec3 p0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
  vec3 p1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
  vec3 p2 = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;

  // CCW check
  float ccw = cross(p1 - p0, p2 - p0).z;
  if (ccw > 0.0) {
    // Emit the triangle edges
    gl_Position = gl_in[0].gl_Position;
    frag_color = color[0];
    EmitVertex();
    gl_Position = gl_in[1].gl_Position;
    frag_color = color[1];
    EmitVertex();
    gl_Position = gl_in[2].gl_Position;
    frag_color = color[2];
    EmitVertex();
    // Close the loop
    gl_Position = gl_in[0].gl_Position;
    frag_color = color[0];
    EmitVertex();
    EndPrimitive();
  }
}
