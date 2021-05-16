#pragma once
#include "glad.h"
#include "GLFW/glfw3.h"
#include "glad.c"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace gloom
{
	struct Sprite2D
	{
		unsigned int id = 0;
		unsigned int tid = 0;
		int width = 0, height = 0;
		float angle = 0;
		glm::vec2 scale = glm::vec2(1.f);
		glm::vec2 position = glm::vec2(0.f, 0.f);
		glm::vec2 point_of_rotation = glm::vec2(0, 0);
		Sprite2D(const char* path, bool transparency = false)
		{
			unsigned int texture_id;
			glGenTextures(1, &texture_id);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			int width, height, n;
			unsigned char* data = stbi_load(path, &width, &height, &n, 0);
			if (data)
			{
				if (transparency)
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				else
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				glGenerateMipmap(GL_TEXTURE_2D);
			}
			else
			{
				std::cout << "Failed to load texture" << std::endl;
			}
			stbi_image_free(data);

			float vertices[12] =
			{
				0.f, 0.f, 0.f,
				1.f, 0.f, 0.f,
				1.f, 1.f, 0.f,
				0.f, 1.f, 0.f
			};

			float texture_coordinates[12] =
			{
				0.f, 0.f,
				1.f, 0.f,
				1.f, 1.f,
				0.f, 1.f,
			};

			unsigned int element_indices[6] =
			{
				0, 2, 3,
				0, 1, 2,
			};

			unsigned int VAO, VBO, TCBO, EBO;
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBindVertexArray(VAO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(element_indices), &element_indices[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glGenBuffers(1, &TCBO);
			glBindBuffer(GL_ARRAY_BUFFER, TCBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coordinates), &texture_coordinates[0], GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
			this->id = VAO;
			this->width = width;
			this->height = height;
			this->tid = texture_id;
		}

		void Draw(ProjMat orthographic_matrix, glm::vec2 xy = glm::vec2(0, 0))
		{
			glm::mat4 model(1.f);
			model = glm::translate(model, glm::vec3(xy[0], xy[1], 0.f));
			if (this->angle != 0)
			{
				model = glm::translate(model, glm::vec3(this->point_of_rotation[0], this->point_of_rotation[1], 0.f));
				model = glm::rotate(model, glm::radians(this->angle), glm::vec3(0.f, 0.f, 1.f));
				model = glm::translate(model, glm::vec3(-this->point_of_rotation[0], -this->point_of_rotation[1], 0.f));
			}
			model = glm::scale(model, glm::vec3(this->width * this->scale[0], this->height * this->scale[1], 0.f));
			glm::mat4 identity_matrix(1.0f);
			WriteToShader(matrix_projection_location, orthographic_matrix.GetPointer());
			WriteToShader(matrix_model_location, &model);
			WriteToShader(matrix_view_location, &identity_matrix);
			WriteToShader(int_n_lights_location, -1);
			glBindVertexArray(this->id);
			glDrawElements(GL_TRIANGLES, 4, GL_FLOAT, 0);
		}

		glm::vec2 GetCenter()
		{
			return glm::vec2(scale[0] * width / 2, scale[1] * height / 2);
		}
	};
}