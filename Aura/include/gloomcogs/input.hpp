#pragma once
#include "GLFW/glfw3.h"

namespace gloom
{
	int mouse_x, mouse_y;

	bool mouse_button_left_down, mouse_button_right_down, mouse_button_middle_down;

	bool mouse_button_left_up, mouse_button_right_up, mouse_button_middle_up;

	bool mouse_button_left_held, mouse_button_right_held, mouse_button_middle_held;

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mode)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			mouse_button_left_down = 1;
			mouse_button_left_held = 1;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			mouse_button_right_down = 1;
			mouse_button_right_held = 1;
		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		{
			mouse_button_middle_down = 1;
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			mouse_button_left_up = 1;
			mouse_button_left_held = 0;
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		{
			mouse_button_right_up = 1;
			mouse_button_right_held = 0;
		}
	}
	
	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		mouse_x = (int)xpos;
		mouse_y = (int)ypos;
	}

	glm::vec2 GetMousePos()
	{
		return glm::vec2(mouse_x, mouse_y);
	}

	void SetMousePos(GLFWwindow* window, int x, int y)
	{
		glfwSetCursorPos(window, x, y);
	}

	void SetMouseMode(GLFWwindow* window, Gloonum mouse_mode)
	{
		if (mouse_mode == gloom::Gloonum::k_gloom_mouse_mode_hide)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		}
		else if (mouse_mode == gloom::Gloonum::k_gloom_mouse_mode_show)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}

	void ForceLowInput()
	{
		glFinish();
	}

	double GetTime()
	{
		return glfwGetTime();
	}

	void Poll()
	{
		glfwPollEvents();
	}

	bool GetKey(GLFWwindow* window, unsigned int glKeycode)
	{
		return glfwGetKey(window, glKeycode);
	}
}