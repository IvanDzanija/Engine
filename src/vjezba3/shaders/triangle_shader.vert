#version 330 core
// Input
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
//uniform mat4 transform_matrix;

// Output
out vec3 color;

void main()
{
	color = aCol;
  gl_Position = vec4(aPos, 1.0);
  //gl_Position = transform_matrix * vec4(aPos, 1.0);
}
