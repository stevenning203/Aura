#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace gloom
{
	struct Light
	{
		glm::vec3 color;
		glm::vec3 direction;
		glm::vec3 attenuation;
		glm::vec3 position;
		float theta;
		Light(glm::vec3 color = glm::vec3(1.f, 1.f, 0.85f), glm::vec3 direction = glm::vec3(0.f, 0.f, -1.f), glm::vec3 attenuation = glm::vec3(1.f), glm::vec3 position = glm::vec3(0.f), float theta = 1.f)
		{
			this->color = color;
			this->direction = direction;
			this->attenuation = attenuation;
			this->position = position;
			this->theta = theta;
		}
	};
}