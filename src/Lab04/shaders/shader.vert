#version 330 core
// Input
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aCol;
uniform vec3 u_color;
//uniform mat4 transform_matrix;

// Output
out vec3 color;

void main()
{
	color = u_color;
  gl_Position = vec4(aPos, 1.0);
}
