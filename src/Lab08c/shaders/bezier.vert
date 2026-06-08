#version 330 core
// Input
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;

// Output
out vec3 color;

void main() {
  color = aCol;
  gl_Position = vec4(aPos, 1.0);
}
