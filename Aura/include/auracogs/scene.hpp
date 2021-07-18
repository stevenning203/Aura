#pragma once
#include "gloom.hpp"
#include "auracogs/object.hpp"

namespace aura
{
	class Scene
	{
		std::vector<gloom::Model> models_vector;
		std::vector<gloom::Light> lights_vector;
		std::vector<gloom::Camera> cameras_vector;
	public:
		std::string name;
		std::vector<Object> objects;
		std::vector<gloom::Light> lights;
		std::vector<gloom::Camera> cameras;
		Scene(const char* label = "Unnamed Scene")
		{
			this->name = label;
		}
		void Draw()
		{
			for (int i = 0; i < objects.size(); i++)
			{
				Object& ref = objects[i];
				if (ref.model.IsEnabled())
				{
					ref.model.Draw(objects[i].modmat, lights);
				}
			}
		}
		void AddLight(gloom::Light light)
		{
			lights.push_back(light);
		}
		void AddCamera(gloom::Camera camera)
		{
			cameras.push_back(camera);
		}
	};

	Scene* active_scene;

	void SetActiveScene(Scene* scene)
	{
		active_scene = scene;
	}
}