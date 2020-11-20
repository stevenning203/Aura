#include "gloom.hpp"

namespace ap
{
	std::unordered_map<int, gloom::Model> model_hash;
	void Throw(const char* text)
	{
		throw std::invalid_argument(text);
	}
	void ModelParse(std::string line)
	{
		if (line.substr(0, 12) == ">>NewModel()")
		{
			int len = stoi(line.substr(13, 2));
			int id = stoi(line.substr(16, 4));
			std::string path = line.substr(21, len);
			model_hash[id] = gloom::Model(path.c_str());
		}
		else if (line.substr(0, 10) == "#endregion")
		{
			return;
		}
		else
		{
			Throw("Parsing error: >>NewModel() or #endregion text missing for Objects region");
		}
	}
	void Compile()
	{

	}
}

namespace aura
{
	int aura_imgui_static_window = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	int object_name_increment = 0;
	int object_loading_increment = 0;
	enum class AuraParse
	{
		k_object, k_light, k_camera, k_script, k_null, k_model,
	};
	class Script
	{
	private:
		std::string path = "";
		std::string label = "Unnamed Script";
		std::string buffer;
	public:
		Script(std::string &path)
		{
			this->path = path;
		}
		void SetLabel(const char* label)
		{
			this->label = label;
		}
		std::string GetLabel()
		{
			return label;
		}
		std::string GetPath()
		{
			return path;
		}
	};
	class Player
	{
		
	};
	class Object
	{
	public:
		gloom::ModMat translation;
		gloom::ModMat rotation;
		gloom::ModMat scaling;
		gloom::ModMat modmat;
		glv3 position = glv3(0.f);
		glv3 scale = glv3(1.f);
		glv3 rot = glv3(0.f);
		std::string name = std::string("Unnamed Object ") + std::to_string(object_name_increment);
		gloom::Model model;
		Object(gloom::Model model_load)
		{
			object_name_increment++;
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

	class SnapShot
	{
		Scene* scene_pointer;
		Scene snapshot;
	public:
		SnapShot(Scene* set)
		{
			this->scene_pointer = set;
			this->snapshot = *set;
		}
		void Restore();
		void Set(Scene snapshot);
	};
}

void aura::SnapShot::Set(Scene snapshot)
{
	this->snapshot = snapshot;
}

void aura::SnapShot::Restore()
{
	*(this->scene_pointer) = snapshot;
}