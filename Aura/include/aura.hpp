#include "gloom.hpp"

namespace aura
{
	class Scene
	{
		std::unordered_map<std::string, gloom::Model> models;
		std::unordered_map<std::string, gloom::Light> lights;
		std::unordered_map<std::string, gloom::Camera> cameras;
		std::vector<gloom::Model> models_vector;
		std::vector<gloom::Light> lights_vector;
		std::vector<gloom::Camera> cameras_vector;
	public:
		void Draw(std::string camera_key)
		{
			gloom::SetCurrentCamera(&cameras[camera_key]);
			for (auto &i: models)
			{
				if (i.second.IsEnabled())
				{
					i.second.Draw(i.second.matrix, lights);
				}
			}
		}
		void AddModel(std::string key, gloom::Model model)
		{
			models[key] = model;
		}
		void RemoveModel(std::string key)
		{
			models.erase(key);
		}
		void AddLight(std::string key, gloom::Light light)
		{
			lights[key] = light;
		}
		void RemoveLight(std::string key)
		{
			lights.erase(key);
		}
		void AddCamera(std::string key, gloom::Camera camera)
		{
			cameras[key] = camera;
		}
		void RemoveCamera(std::string key)
		{
			cameras.erase(key);
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