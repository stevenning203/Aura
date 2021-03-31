#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "glad.c"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gloomcogs/light.hpp"
#include "gloomcogs/const.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace gloom
{
	class UniformLocation
	{
	public:
		int val;
	};

	UniformLocation vector_camera_location;

	UniformLocation matrix_ortographic_location, matrix_model_location, matrix_projection_location, matrix_view_location;

	UniformLocation int_n_lights_location;

	UniformLocation struct_light_location[k_max_n_lights], struct_light_attenuation[k_max_n_lights], struct_light_color[k_max_n_lights], struct_light_theta[k_max_n_lights], struct_light_direction[k_max_n_lights];

	UniformLocation sampler_diffuse_location[k_max_n_diffuse], sampler_specular_location[k_max_n_specular];

	UniformLocation float_ambient_strength, float_specular_strength;

	void WriteToShader(UniformLocation shader_location, glm::vec3* vector)
	{
		glUniform3fv(shader_location.val, 1, &((*vector)[0]));
	}

	void WriteToShader(UniformLocation shader_location, int integer)
	{
		glUniform1i(shader_location.val, integer);
	}

	void WriteToShader(UniformLocation shader_location, float floating_point)
	{
		glUniform1f(shader_location.val, floating_point);
	}

	void WriteToShader(UniformLocation shader_location, glm::mat4* matrix)
	{
		glUniformMatrix4fv(shader_location.val, 1, GL_FALSE, &((*matrix)[0][0]));
	}

	void WriteToShader(std::vector<Light>& light_sources)
	{
		int index = 0;
		for (auto& i : light_sources)
		{
			WriteToShader(struct_light_attenuation[index], &i.attenuation);
			WriteToShader(struct_light_location[index], &i.position);
			WriteToShader(struct_light_color[index], &i.color);
			WriteToShader(struct_light_theta[index], i.theta);
			WriteToShader(struct_light_direction[index], &i.direction);
			index++;
		}
	}

	void ParseShader(std::string path, std::string* vertex_shader_src_ptr, std::string* fragment_shader_src_ptr)
	{
		std::string* ptvss = vertex_shader_src_ptr;
		std::string* ptfss = fragment_shader_src_ptr;
		std::string line;
		std::ifstream stream;
		std::stringstream string_stream[2];
		int vOF = 0;
		stream.open(path);
		while (getline(stream, line))
		{
			if (line.find("#region") != std::string::npos)
			{
				if (line.find("vertex") != std::string::npos)
				{
					vOF = 0;
				}
				else if (line.find("fragment") != std::string::npos)
				{
					vOF = 1;
				}
			}
			else
			{
				string_stream[vOF] << line << '\n';
			}
		}
		*ptvss = string_stream[0].str();
		*ptfss = string_stream[1].str();
	}

	unsigned int ShaderInit(const char* path) {
		unsigned int vSID = glCreateShader(GL_VERTEX_SHADER);
		unsigned int fSID = glCreateShader(GL_FRAGMENT_SHADER);
		std::string vSSrc;
		std::string fSSrc;
		ParseShader(path, &vSSrc, &fSSrc);
		GLint success = GL_FALSE;
		int InfoLogLength;
		char const* VertexSourcePointer = vSSrc.c_str();
		glShaderSource(vSID, 1, &VertexSourcePointer, NULL);
		glCompileShader(vSID);
		glGetShaderiv(vSID, GL_COMPILE_STATUS, &success);
		glGetShaderiv(vSID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage((size_t)InfoLogLength + (size_t)1);
			glGetShaderInfoLog(vSID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			std::cout << &VertexShaderErrorMessage[0];
		}
		char const* FragmentSourcePointer = fSSrc.c_str();
		glShaderSource(fSID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(fSID);
		glGetShaderiv(fSID, GL_COMPILE_STATUS, &success);
		glGetShaderiv(fSID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage((size_t)InfoLogLength + (size_t)1);
			glGetShaderInfoLog(fSID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			std::cout << &FragmentShaderErrorMessage[0];
		}
		unsigned int ProgramID = glCreateProgram();
		glAttachShader(ProgramID, vSID);
		glAttachShader(ProgramID, fSID);
		glLinkProgram(ProgramID);
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage((size_t)InfoLogLength + (size_t)1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			std::cout << &ProgramErrorMessage[0];
		}

		glDetachShader(ProgramID, vSID);
		glDetachShader(ProgramID, fSID);

		glDeleteShader(vSID);
		glDeleteShader(fSID);

		return ProgramID;
	}
}