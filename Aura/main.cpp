#include "aura.hpp"

namespace state
{
	bool saved = false;
	bool new_file_menu_open = false;
	bool open_file_menu_open = false;
	bool save_file_menu_open = false;
	bool options_menu_open = false;
	bool models_menu_open = true;
	bool console_open = true;
	bool load_models_menu_open = false;
	bool load_models_error_popup_open = false;
	bool objects_menu_open = true;
	bool scenes_menu_open = false;
	bool context_editor_open = true;
	bool lights_menu_open = true;
}

namespace buffers
{
	const int k_char_input_max = 300;
	char project_name[k_char_input_max];
	char project_location[k_char_input_max];
	char console_output[k_char_input_max];
	char console_input[k_char_input_max];
	char model_name[k_char_input_max];
	char model_location[k_char_input_max];
	char scene_name[k_char_input_max];

	bool flip_uvs = true;

	void Clear(char* buffer)
	{
		memset(buffer, NULL, k_char_input_max);
	}
}

namespace console
{
	void Parse(char* input)
	{
		std::string str_input = std::string(input);
		if (str_input.substr(0, 7) == "menu : ")
		{
			if (str_input.substr(7, 6) == "lights")
			{
				state::lights_menu_open = true;
			}
			if (str_input.substr(7, 7) == "objects")
			{
				state::objects_menu_open = true;
			}
		}
	}
}

int main()
{
	gloom::Init(1920, 1080, "Aura");

	std::vector<gloom::Model> models;
	std::vector<std::string> model_names;
	std::vector<aura::Scene> scenes;
	
	aura::Scene main_scene("Main");
	gloom::Camera main_camera;
	main_camera.SetPos(glv3(3.f, 3.f, 3.f));
	gloom::SetCurrentCamera(&main_camera);

	main_scene.AddCamera(main_camera);

	aura::SetActiveScene(&main_scene);

	gloom::SetClearColor(0.65f, 0.5f, 0.65f);

	while (!gloom::QueueExit())
	{	

		gloom::ClearBuffer();
		//render
		aura::active_scene->Draw();
		{
			if (ImGui::BeginMainMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("New"))
					{
						state::new_file_menu_open = true;
					}
					if (ImGui::MenuItem("Open"))
					{
						state::open_file_menu_open = true;
					}
					if (ImGui::MenuItem("Save"))
					{

					}
					if (ImGui::MenuItem("Compile"))
					{

					}
					if (ImGui::MenuItem("Quit"))
					{
						return 0;
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("View"))
				{
					if (ImGui::BeginMenu("Menus"))
					{
						if (ImGui::MenuItem("Context Editor"))
						{
							state::context_editor_open = true;
						}
						if (ImGui::MenuItem("Models"))
						{
							state::models_menu_open = true;
						}
						if (ImGui::MenuItem("Lights"))
						{
							state::lights_menu_open = true;
						}
						if (ImGui::MenuItem("Objects"))
						{
							state::objects_menu_open = true;
						}
						if (ImGui::MenuItem("Scenes"))
						{
							state::scenes_menu_open = true;
						}
						if (ImGui::MenuItem("Console"))
						{
							state::console_open = true;
						}
						ImGui::EndMenu();
					}
					if (ImGui::MenuItem("Theme"))
					{

					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Tools"))
				{
					if (ImGui::MenuItem("Placeholder"))
					{

					}
					if (ImGui::MenuItem("Options"))
					{
						state::options_menu_open = true;
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("Documentation"))
					{
						ShellExecute(0, 0, L"http://stevenning203.github.io/Aura-Documentation/", 0, 0, SW_SHOW);
					}
					ImGui::EndMenu();
				}
				if (ImGui::Button("|> / ||"))
				{

				}
				ImGui::EndMainMenuBar();
			}
			if (state::new_file_menu_open)
			{
				ImGui::Begin("New Project");
				ImGui::InputText("Project Name", buffers::project_name, buffers::k_char_input_max);
				ImGui::InputText("Path", buffers::project_location, buffers::k_char_input_max);
				if (ImGui::Button("Create"))
				{
					state::new_file_menu_open = false;
				}
				if (ImGui::Button("Close"))
				{
					state::new_file_menu_open = false;
				}
				ImGui::End();
			}
			if (state::open_file_menu_open)
			{
				ImGui::Begin("Open Project");
				ImGui::InputText(".aura File Path", buffers::project_location, buffers::k_char_input_max);
				if (ImGui::Button("Load"))
				{
					std::string line;
					std::fstream file(buffers::project_location);
					int ln = 1;
					aura::AuraParse mode;
					while (std::getline(file, line))
					{
						if (line.find("[begin:a^f]"))
						{
							if (!ln)
							{
								break;
							}
						}
						if (line.find("[region:objects]"))
						{
							mode = aura::AuraParse::k_object;
						}
						if (line.find("[region:lights"))
						{
							mode = aura::AuraParse::k_light;
						}
						if (line.find("[region:scripts"))
						{
							mode = aura::AuraParse::k_script;
						}
						ln++;
					}
					state::open_file_menu_open = false;
				}
				if (ImGui::Button("Close"))
				{
					state::open_file_menu_open = false;
				}
				ImGui::End();
			}
			if (state::save_file_menu_open)
			{
				ImGui::Begin("Save");

				ImGui::End();
			}
			if (state::models_menu_open)
			{
				ImGui::Begin("Models");
				if (ImGui::Button("Load Models"))
				{
					state::load_models_menu_open = true;
				}
				for (int i = 0; i < models.size(); i++)
				{
					if (i)
						ImGui::SameLine();
					if (ImGui::Button(model_names[i].c_str()))
					{
						aura::active_scene->objects.push_back(aura::Object(models[i]));
					}
				}
				ImGui::End();
			}
			if (state::lights_menu_open)
			{
				ImGui::Begin("Lights");
				if (ImGui::Button("Close"))
				{
					state::lights_menu_open = false;
				}
				if (ImGui::Button("Add Light"))
				{
					aura::active_scene->lights.push_back(gloom::Light());
				}
				for (int i = 0; i < aura::active_scene->lights.size(); i++)
				{
					if (ImGui::TreeNode("Light " + i))
					{
						if (ImGui::InputFloat("Light Attenuation", &aura::active_scene->lights[i].attenuation[0]))
						{

						}
						ImGui::InputFloat("Light Colour Normalized R", &aura::active_scene->lights[i].color[0]);
						ImGui::InputFloat("Light Colour Normalized G", &aura::active_scene->lights[i].color[1]);
						ImGui::InputFloat("Light Colour Normalized B", &aura::active_scene->lights[i].color[2]);
						{
							
						}
						if (ImGui::InputFloat("Light Position X", &aura::active_scene->lights[i].position[0]) || ImGui::InputFloat("Light Position Y", &aura::active_scene->lights[i].position[1]) || ImGui::InputFloat("Light Position Z", &aura::active_scene->lights[i].position[2]))
						{

						}
						if (ImGui::InputFloat("Light Direction X", &aura::active_scene->lights[i].direction[0]) || ImGui::InputFloat("Light Direction Y", &aura::active_scene->lights[i].direction[1]) || ImGui::InputFloat("Light Direction Z", &aura::active_scene->lights[i].direction[2]))
						{

						}
						ImGui::TreePop();
					}
				}
				ImGui::End();
			}
			if (state::objects_menu_open)
			{
				ImGui::Begin("Objects");
				if (ImGui::Button("Close"))
				{
					state::objects_menu_open = false;
				}
				for (int i = 0; i < aura::active_scene->objects.size(); i++)
				{
					if (ImGui::TreeNode((aura::active_scene->objects[i].name + std::to_string(i)).c_str()))
					{
						if (ImGui::InputFloat("X Position", &aura::active_scene->objects[i].position[0], 1, 1) || ImGui::InputFloat("Y Position", &aura::active_scene->objects[i].position[1], 1, 1) || ImGui::InputFloat("Z Position", &aura::active_scene->objects[i].position[2], 1, 1))
						{
							aura::active_scene->objects[i].translation.Reset();
							aura::active_scene->objects[i].translation.XYZ(glv3(aura::active_scene->objects[i].position));
							aura::active_scene->objects[i].MergeMat();
						}
						if (ImGui::InputFloat("X Scale", &aura::active_scene->objects[i].scale.x, 1, 1) || ImGui::InputFloat("Y Scale", &aura::active_scene->objects[i].scale.y, 1, 1) || ImGui::InputFloat("Z Scale", &aura::active_scene->objects[i].scale.z, 1, 1))
						{
							aura::active_scene->objects[i].scaling.Reset();
							aura::active_scene->objects[i].scaling.Scale(glv3(aura::active_scene->objects[i].scale));
							aura::active_scene->objects[i].MergeMat();
						}
						if (ImGui::InputFloat("X rotation", &aura::active_scene->objects[i].rot[0], 1, 1))
						{

						}
						if (ImGui::Button("Delete Object"))
						{
							aura::active_scene->objects.erase(aura::active_scene->objects.begin() + i);
						}
						ImGui::TreePop();
					}
				}
				ImGui::End();
			}
			if (state::load_models_menu_open)
			{
				ImGui::Begin("Load Model");
				ImGui::InputText("Model Location", buffers::model_location, sizeof(buffers::model_location));
				ImGui::InputText("Model Name", buffers::model_name, sizeof(buffers::model_name));
				ImGui::Checkbox("Flip UVS", &buffers::flip_uvs);
				if (ImGui::Button("Load"))
				{
					ImGui::Text("LOADING...");
					gloom::Model temp_model(buffers::model_location, buffers::flip_uvs);
					if (!temp_model.Valid())
					{
						state::load_models_error_popup_open = true;
					}
					else
					{
						models.push_back(temp_model);
						model_names.push_back(buffers::model_name);
						buffers::Clear(buffers::model_location);
						buffers::Clear(buffers::model_name);
					}
				}
				if (ImGui::Button("Close"))
				{
					state::load_models_menu_open = false;
				}
				ImGui::End();
			}
			if (state::scenes_menu_open)
			{
				ImGui::Begin("Scenes");
				ImGui::InputText("Scene Name", buffers::scene_name, buffers::k_char_input_max);
				if (ImGui::Button("Add Scene"))
				{
					scenes.push_back(aura::Scene(buffers::scene_name));
				}
				for (int i = 0; i < scenes.size(); i++)
				{
					ImGui::TreeNode(scenes[i].name.c_str());
					if (ImGui::Button("Set As Active"))
					{
						aura::SetActiveScene(&scenes[i]);
					}
					ImGui::TreePop();
				}
				ImGui::End();
			}
			if (state::context_editor_open)
			{
				ImGui::Begin("Context Editor");
				ImGui::End();
			}
			if (state::console_open)
			{
				ImGui::Begin("Console");
				ImGui::Text((std::string("OUT >> |: ") + std::string(buffers::console_output)).c_str());
				ImGui::InputText(">> console in", buffers::console_input, sizeof(buffers::console_input));
				if (ImGui::Button("Send"))
				{
					console::Parse(buffers::console_input);
					buffers::Clear(buffers::console_input);
				}
				ImGui::End();
			}
			if (state::load_models_error_popup_open)
			{
				ImGui::Begin("Error");
				ImGui::Text("Invalid path to model or model has no valid meshes");
				if (ImGui::Button("OK"))
				{
					state::load_models_error_popup_open = false;
				}
				ImGui::End();
			}
		}
		//endrender
		gloom::FlipDisplay();
		gloom::Poll();
	}
	gloom::Terminate();
	return 0;
}