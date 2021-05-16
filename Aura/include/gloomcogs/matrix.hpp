#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace gloom
{
	class ModMat
	{
		glm::mat4 internal = glm::mat4(1.0f);
	public:
		void Set(glm::mat4 matrix)
		{
			this->internal = matrix;
		}
		void Reset()
		{
			this->internal = glm::mat4(1.f);
		}
		void XYZ(glm::vec3 offset)
		{
			this->internal = glm::translate(this->internal, offset);
		}
		void Rotation(float degrees, glm::vec3 axes)
		{
			this->internal = glm::rotate(this->internal, glm::radians(degrees), axes);
		}
		void Scale(glm::vec3 axes)
		{
			this->internal = glm::scale(this->internal, axes);
		}
		glm::mat4 Get()
		{
			return this->internal;
		}
		glm::mat4* GetPointer()
		{
			return &(this->internal);
		}
	};

	class ProjMat
	{
		glm::mat4 internal = glm::mat4(1.0f);
	public:
		void Set(glm::mat4 matrix)
		{
			this->internal = matrix;
		}
		glm::mat4 Get()
		{
			return this->internal;
		}
		glm::mat4* GetPointer()
		{
			return &(this->internal);
		}
	};
}