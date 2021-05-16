#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gloomcogs/matrix.hpp"
#include "GLFW/glfw3.h"

namespace gloom
{
	class Window
	{
	public:
		GLFWwindow* local_window;
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

	Window window;

	float field_of_view = 90.f;

	ProjMat perspective_matrix, orthographic_matrix;

	void FrameBufferSizeCallback(GLFWwindow* window_ptr, int width, int height)
	{
		window.width = width;
		window.height = height;
		window.UpdateHalfPoint();
		window.UpdateAspectRatio();
		glViewport(0, 0, width, height);
		perspective_matrix.Set(glm::perspective(glm::radians(field_of_view), window.aspect_ratio, 0.1f, 100.f));
	}
}