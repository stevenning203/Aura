#region vertex
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 mvp;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = mvp * vec4(aPos, 1.0);
}

#region fragment
#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
	FragColor = texture(texture_diffuse1, TexCoords);
	//FragColor = texture(texture_specular1, TexCoords);
}