#version 330 core
// Input
layout (location = 0) in vec3 aPos;
uniform vec3 u_color;
uniform mat4 transform_matrix;

// Output
out vec3 color;

void main()
{
	color = u_color;
  gl_Position = transform_matrix*vec4(aPos, 1.0);
}
