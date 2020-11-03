#include "gloom.hpp"

namespace aura
{
	class Player
	{
		
	};
	class Object
	{
		int id = -1;
	public:
		glv3 position = glv3(0.f);
		glv3 scale = glv3(1.f);
		std::string name = "Unnamed Object";
		gloom::Model model;
		Object(gloom::Model model_load)
		{
			this->model = model_load;
		}
		void Draw(gloom::ModMat matrix, std::unordered_map<std::string, gloom::Light> lights)
		{
			this->model.Draw(matrix, lights);
		}
	};
	class Scene
	{
		std::vector<gloom::Model> models_vector;
		std::vector<gloom::Light> lights_vector;
		std::vector<gloom::Camera> cameras_vector;
	public:
		std::vector<Object> objects;
		std::unordered_map<std::string, gloom::Model> models;
		std::unordered_map<std::string, gloom::Light> lights;
		std::unordered_map<std::string, gloom::Camera> cameras;
		void Draw()
		{
			for (int i = 0; i < objects.size(); i++)
			{
				Object& ref = objects[i];
				if (ref.model.IsEnabled())
				{
					ref.model.Draw(ref.model.matrix, lights);
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

	Scene* active_scene;

	void SetActiveScene(Scene* scene)
	{
		active_scene = scene;
	}
}