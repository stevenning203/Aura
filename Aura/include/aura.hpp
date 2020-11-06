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
		gloom::ModMat translation;
		gloom::ModMat rotation;
		gloom::ModMat scaling;
		gloom::ModMat modmat;
		glv3 position = glv3(0.f);
		glv3 scale = glv3(1.f);
		std::string name = "Unnamed Object";
		gloom::Model model;
		Object(gloom::Model model_load)
		{
			this->model = model_load;
		}
		void Draw(gloom::ModMat matrix, std::vector<gloom::Light> &lights)
		{
			this->model.Draw(matrix, lights);
		}
		void MergeMat()
		{
			modmat.Set(translation.Get() * rotation.Get() * scaling.Get());
		}
	};
	class Scene
	{
		std::vector<gloom::Model> models_vector;
		std::vector<gloom::Light> lights_vector;
		std::vector<gloom::Camera> cameras_vector;
	public:
		std::vector<Object> objects;
		std::vector<gloom::Light> lights;
		std::vector<gloom::Camera> cameras;
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