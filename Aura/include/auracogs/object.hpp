#pragma once
#include "gloom.hpp"

namespace aura
{
	int object_name_increment = 0;

	class Object
	{
	public:
		gloom::ModMat translation;
		gloom::ModMat rotation;
		gloom::ModMat scaling;
		gloom::ModMat modmat;
		glm::vec3 position = glm::vec3(0.f);
		glm::vec3 scale = glm::vec3(1.f);
		glm::vec3 rot = glm::vec3(0.f);
		std::string name = std::string("Unnamed Object ") + std::to_string(object_name_increment);
		gloom::Model model;
		Object(gloom::Model& model_load)
		{
			object_name_increment++;
			this->model = model_load;
		}
		Object()
		{
			object_name_increment++;
			this->model = gloom::Model();
		}
		void Draw(gloom::ModMat& matrix, std::vector<gloom::Light>& lights)
		{
			this->model.Draw(matrix, lights);
		}
		void MergeMat()
		{
			modmat.Set(translation.Get() * rotation.Get() * scaling.Get());
		}
	};
}