#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

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
}