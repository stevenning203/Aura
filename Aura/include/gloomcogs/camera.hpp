#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLFW/glfw3.h"
#include "gloomcogs/input.hpp"
#include "gloomcogs/window.hpp"

namespace gloom
{
	class Camera
	{
	private:
		float fov = 90.f;
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 trg = glm::vec3(0.f, 0.f, 0.f);
		glm::mat4 view_matrix = glm::mat4(1.0f);
		Gloonum mode = Gloonum::k_gloom_mode_freecam;
	public:
		void SetPos(glm::vec3 pos)
		{
			this->pos = pos;
			this->view_matrix = glm::lookAt(this->pos, this->trg, glm::vec3(0.f, 1.f, 0.f));
		}
		glm::vec3 GetPos()
		{
			return pos;
		}
		glm::vec3* GetPosPointer()
		{
			return &(this->pos);
		}
		void SetTrg(glm::vec3 target)
		{
			this->trg = target;
			this->view_matrix = glm::lookAt(this->pos, this->trg, glm::vec3(0.f, 1.f, 0.f));
		}
		void SetMode(Gloonum mode)
		{
			this->mode = mode;
		}
		glm::mat4 GetMatrix()
		{
			return view_matrix;
		}
		glm::mat4* GetMatrixPointer()
		{
			return &(this->view_matrix);
		}
	};

	Camera* current_camera;

	float camera_sensitivity = 0.2f;
	float camera_speed = 5.f;
	float camera_speed_multiplier = 1.f;

	float pitch = 0.f;
	float yaw = -90.f;

	void CameraBegin()
	{
		GetKey(window.local_window, GLFW_KEY_LEFT_SHIFT) ? camera_speed_multiplier = 2.5f : camera_speed_multiplier = 1.f;
		float delta_x = window.half_point.x - mouse_x;
		float delta_y = window.half_point.y - mouse_y;
		if (mouse_button_right_down)
		{
			delta_x = 0;
			delta_y = 0;
		}
		delta_x *= camera_sensitivity;
		delta_y *= camera_sensitivity;
		yaw += delta_x;
		pitch += delta_y;
		if (pitch > 89.9f)
		{
			pitch = 89.9f;
		}
		if (pitch < -89.9f)
		{
			pitch = -89.9f;
		}
		glm::vec3 direction;
		direction.x = std::cos(glm::radians(pitch)) * std::sin(glm::radians(yaw));
		direction.y = std::sin(glm::radians(pitch));
		direction.z = std::cos(glm::radians(pitch)) * std::cos(glm::radians(yaw));
		direction = glm::normalize(direction);
		glm::vec3 right = glm::vec3(std::sin(glm::radians(yaw) - k_pi / 2.0f), 0, std::cos(glm::radians(yaw) - k_pi / 2.0f));
		if (GetKey(window.local_window, GLFW_KEY_W))
		{
			current_camera->SetPos(current_camera->GetPos() + camera_speed_multiplier * camera_speed * direction * time2);
		}
		if (GetKey(window.local_window, GLFW_KEY_S))
		{
			current_camera->SetPos(current_camera->GetPos() - camera_speed_multiplier * camera_speed * direction * time2);
		}
		if (GetKey(window.local_window, GLFW_KEY_A))
		{
			current_camera->SetPos(current_camera->GetPos() - camera_speed_multiplier * camera_speed * right * time2);
		}
		if (GetKey(window.local_window, GLFW_KEY_D))
		{
			current_camera->SetPos(current_camera->GetPos() + camera_speed_multiplier * camera_speed * right * time2);
		}
		if (GetKey(window.local_window, GLFW_KEY_SPACE))
		{
			current_camera->SetPos(current_camera->GetPos() + glm::vec3(0.f, camera_speed_multiplier * camera_speed * time2, 0.f));
		}
		current_camera->SetTrg(current_camera->GetPos() + direction);
	}

	void CameraEnd()
	{
		SetMousePos(window.local_window, (int)window.half_point.x, (int)window.half_point.y);
	}

	void SetFieldOfView(float deg)
	{
		field_of_view = deg;
		perspective_matrix.Set(glm::perspective(glm::radians(field_of_view), window.aspect_ratio, 0.1f, 100.f));
	}

	Camera* GetCurrentCamera()
	{
		return current_camera;
	}

	void SetCurrentCamera(gloom::Camera* camera_set)
	{
		current_camera = camera_set;
	}
}