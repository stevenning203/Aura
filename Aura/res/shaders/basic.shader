#region vertex
#version 410 core
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 texture_coordinate_in;

out vec2 texture_coordinate;
out vec3 fragment_position;
out vec3 normal;

uniform mat4 matrix_view;
uniform mat4 matrix_projection;
uniform mat4 matrix_orthographic;
uniform mat4 matrix_model;

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

out vec4 fragment_color;

in vec2 texture_coordinate;
in vec3 fragment_position;
in vec3 normal;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform Light lights[100];
uniform int n_lights;
uniform int object_specularity;
uniform vec3 camera_position;
uniform float ambient_strength;
uniform float specular_strength;

vec3 CalculateAmbient(vec3 strength, vec3 color)
{
	return strength * color;
}

vec3 CalculateDiffuse(vec3 light_direction, vec3 color)
{
	float diffuse_constant = max(dot(normal, light_direction), ambient_strength);
	vec3 diffuse = diffuse_constant * color;
	return diffuse;
}

vec3 CalculateSpecular(vec3 view_direction, vec3 reflection_direction, vec3 color)
{
	float specularity = pow(max(dot(view_direction, reflection_direction), 0.f), 32);
	vec3 specular = specular_strength * specularity * color;
	return specular;
}

vec3 CalculateLighting(Light light_source, vec3 fragment_position)
{
	vec3 view_direction = normalize(camera_position - fragment_position);
	vec3 normalized = normalize(normal);
	vec3 light_direction = normalize(light_source.position - fragment_position);
	vec3 reflection_direction = reflect(-light_direction, normalized);

	vec3 result = CalculateDiffuse(light_direction, light_source.color) + CalculateSpecular(view_direction, reflection_direction, light_source.color);
	return result;
}

void main()
{
	vec4 texture_fragment = vec4(texture(texture_diffuse1, texture_coordinate));
	vec4 light_multiplier = vec4(vec3(ambient_strength), 1.f);
	for (int i = 0; i < n_lights; i++)
	{
		light_multiplier += vec4(CalculateLighting(lights[i], fragment_position), 0.f);
	}
	if (n_lights == -1)
	{
		light_multiplier = vec4(1.f);
	}
	fragment_color = texture_fragment * light_multiplier;
}