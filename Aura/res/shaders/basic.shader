#region vertex
#version 410 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 vertex_color;
layout(location = 2) in vec2 tc;
out vec3 fragment_color;
out vec2 texture_coords;

uniform mat4 mvp;

void main() {
	gl_Position = mvp * vec4(pos, 1.0f);
	fragment_color = vertex_color;
	texture_coords = tc;
}


#region fragment
#version 410 core

in vec2 texture_coords;
in vec3 fragment_color;
out vec4 fragment;

uniform sampler2D texture_diffuse1;

void main()
{
	fragment = texture2D(texture_diffuse1, texture_coords);
	//fragment = vec4(texture_coords[0], texture_coords[1], 0.9f, 1.f);
};