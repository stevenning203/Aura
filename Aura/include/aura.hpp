#include "gloom.hpp"

namespace aura
{
	class Scene
	{
		std::vector<gloom::Model> models;
		std::vector<gloom::Light> lights;
		std::vector<gloom::Camera> cameras;
		void Draw(int camera_index)
		{
			gloom::SetCurrentCamera(&cameras[camera_index]);
			for (int i = 0; i < models.size(); i++)
			{
				if (models[i].IsEnabled())
				{
					models[i].Draw(models[i].matrix, lights.data(), lights.size());
				}
			}
		}
		void AddModel(gloom::Model model)
		{
			models.push_back(model);
		}
		void RemoveModel(int index)
		{

		}
		void AddLight(gloom::Light light)
		{
			lights.push_back(light);
		}
		void RemoveLight(int index)
		{

		}
	};

	class Object
	{
		int id = -1;
		Object(gloom::Model model_load)
		{
			gloom::Model model = model_load;
		}
	};
}