#pragma once
#include "gloom.hpp"
#include "auracogs/const.hpp"

namespace aura
{
	int aura_imgui_static_window = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	int object_name_increment = 0;
	int object_loading_increment = 0;

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
		Object(gloom::Model &model_load)
		{
			object_name_increment++;
			this->model = model_load;
		}
		Object()
		{
			object_name_increment++;
			this->model = gloom::Model();
		}
		void Draw(gloom::ModMat &matrix, std::vector<gloom::Light> &lights)
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

namespace ap
{
	std::unordered_map<int, gloom::Model> model_hash;
	std::unordered_map<std::string, aura::Object> object_hash;
	void Throw(const char* text)
	{
		throw std::invalid_argument(text);
	}
	bool OverwriteString(std::string path, std::string& cont)
	{
		std::ifstream c(path);
		std::ofstream o("temp.cpp");
		if (std::remove(path.c_str()) != 0)
		{
			return false;
		}
		o << cont;
		if (std::rename("temp.cpp", path.c_str()) != 0)
		{
			return false;
		}
		return true;
	}
	void ObjectParse(std::string line)
	{
		if (line.substr(0, 13) == ">>NewObject()")
		{
			int len = stoi(line.substr(14, 3));
			int model_id = stoi(line.substr(18, 4));
			std::string id = line.substr(23, len);
			object_hash[id] = aura::Object(model_hash[model_id]);
		}
		else if (line.substr(0, 10) == "#endregion")
		{
			return;
		}
		else
		{
			Throw("Parsing error: >>NewObject() or #endregion text missing for objects region");
		}
	}
	void ModelParse(std::string line)
	{
		if (line.substr(0, 12) == ">>NewModel()")
		{
			int len = stoi(line.substr(13, 3));
			int id = stoi(line.substr(17, 4));
			std::string path = line.substr(22, len);
			model_hash[id] = gloom::Model(path.c_str());
		}
		else if (line.substr(0, 10) == "#endregion")
		{
			return;
		}
		else
		{
			Throw("Parsing error: >>NewModel() or #endregion text missing for models region");
		}
	}
	void Compile()
	{

	}
}