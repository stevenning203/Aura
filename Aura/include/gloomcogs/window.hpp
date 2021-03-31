#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace gloom
{
	class Window
	{
	public:
		int width = 0;
		int height = 0;
		glm::vec2 half_point = glm::vec2(0.f);
		float aspect_ratio = 1.f;
		void UpdateHalfPoint()
		{
			this->half_point[0] = (int)((float)width / 2.f);
			this->half_point[1] = (int)((float)height / 2.f);
		}
		void UpdateAspectRatio()
		{
			if (width && height)
				this->aspect_ratio = (float)width / (float)height;
			else
				this->aspect_ratio = 16.f / 9.f;
		}
	};
}