#region vertex
#version 330 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coordinate_in;

out vec2 texture_coordinate;
out vec3 fragment_position;

uniform mat4 matrix_model;
uniform mat4 matrix_view;
uniform mat4 matrix_projection;

void main()
{
	texture_coordinate = texture_coordinate_in;
	gl_Position = matrix_model * matrix_projection * matrix_view * vec4(vertex_position, 1.0);
}

#region fragment
#version 330 core
out vec4 fragment_color;

in vec2 texture_coordinate;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	fragment_color = texture(texture_diffuse1, texture_coordinate);
}