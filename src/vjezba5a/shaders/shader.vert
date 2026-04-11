#version 330 core
// Input
// Position
layout (location = 0) in vec3 aPos;
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

// Color
uniform vec3 u_color;
//layout (location = 1) in vec3 aCol;

// Output
out vec3 color;

void main()
{
	color = u_color;
  gl_Position = u_projection * u_view * u_model * vec4(aPos, 1.0);
}
