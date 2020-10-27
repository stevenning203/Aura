#region vertex
#version 330 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coordinate_in;

out vec2 texture_coordinate;
out vec3 fragment_position;
out vec3 normal;

uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform mat4 matrix_model;
uniform vec3 light_color;
uniform vec3 light_position;

void main()
{
	normal = vertex_normal;
	texture_coordinate = texture_coordinate_in;
	fragment_position = vec3(matrix_model * vec4(vertex_position, 1.f));

	gl_Position = matrix_projection * matrix_view * matrix_model * vec4(vertex_position, 1.f);
}

#region fragment
#version 330 core
precision mediump float;

struct Light
{
	vec3 color;
	vec3 direction;
	vec3 attenuation;
	vec3 position;
	float theta;
};

out vec3 fragment_color;

in vec2 texture_coordinate;
in vec3 fragment_position;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform Light lights[10];

void Calculate()
{

}

void main()
{
	float ambient_strength = 0.1f;
	vec3 ambient = ambient_strength * light_color;
	vec3 normalized = normalize(normal);
	vec3 light_direction = normalize(light_position - fragment_position);
	float diffuse_constant = max(dot(normal, light_direction), 0.f);
	vec3 diffuse = diffuse_constant * light_color;
	fragment_color = (ambient + diffuse) * vec3(texture(texture_diffuse1, texture_coordinate));
}