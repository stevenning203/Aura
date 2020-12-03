#include "scriptspace.hpp"

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
	bool context_editor_open = false;
	bool lights_menu_open = true;
	bool scripts_menu_open = false;
	bool compile_menu_open = false;
	bool theme_menu_open = false;
	bool text_editor_open = true;
	bool about_menu_open = false;
	bool load_script_menu_open = false;
	bool load_script_error_menu_open = false;
	bool load_script_help_menu_open = false;
	bool save_success_popup_open = false;
	bool save_failure_popup_open = false;
	bool new_script_menu_open = false;
	bool compile_success_menu_open = false;
	bool compile_success_failure_open = false;
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
	char object_name[k_char_input_max];
	char script_location[k_char_input_max];

	float shader_ambient_strength = 0.1f;
	float shader_specular_strength = 0.5f;

	bool flip_uvs = true;

	void Clear(char* buffer)
	{
		memset(buffer, NULL, k_char_input_max);
	}

	void Log(const char* output)
	{
		strcpy_s(console_output, output);
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
				buffers::Log("Successfully opened lights menu");
			}
			if (str_input.substr(7, 7) == "objects")
			{
				state::objects_menu_open = true;
				buffers::Log("Successfully opened objects menu");
			}
		}
		else if (str_input.substr(0, 4) == "quit" || str_input.substr(0, 4) == "exit")
		{
			gloom::ForceExit();
			buffers::Log("Force closing unsuccesful");
		}
	}
}

int main()
{
	gloom::Init(1920, 1080, "Aura");

	gloom::Sprite2D stewie("models/2d/ryanbarian.png", true);

	std::vector<gloom::Model> models;
	std::vector<std::string> model_names;
	std::vector<aura::Scene> scenes;
	std::vector<gloom::Particle> particles;
	
	aura::Scene main_scene("Main");
	gloom::Camera main_camera;
	gloom::SetCurrentCamera(&main_camera);

	aura::SetActiveScene(&main_scene);

	gloom::SetClearColor(0.65f, 0.5f, 0.65f);

	strcpy_s(buffers::model_location, "models/backpack/backpack.obj");
	strcpy_s(buffers::model_name, "backpack");

	gloom::CameraBegin();
	gloom::CameraEnd();
	while (!gloom::QueueExit())
	{
		gloom::ClearBuffer();
		//render
		if (gloom::mouse_button_right_held)
		{
			gloom::CameraBegin();
		}
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
						state::save_file_menu_open = true;
					}
					if (ImGui::MenuItem("Compile"))
					{
						state::compile_menu_open = true;
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
						if (ImGui::MenuItem("Text Editor"))
						{
							state::text_editor_open = true;
						}
						if (ImGui::MenuItem("Scripts"))
						{
							state::scripts_menu_open = true;
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
						state::theme_menu_open = true;
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Tools"))
				{
					if (ImGui::MenuItem("Options"))
					{
						state::options_menu_open = true;
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Help"))
				{
					if (ImGui::MenuItem("About"))
					{
						state::about_menu_open = true;
					}
					if (ImGui::MenuItem("Documentation"))
					{
						ShellExecute(0, 0, L"http://stevenning203.github.io/Aura-Documentation/", 0, 0, SW_SHOW);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
			if (state::compile_menu_open)
			{
				ImGui::Begin("Compiler");
				if (ImGui::Button("Compile"))
				{
					system("g++");
				}
				if (ImGui::Button("Close"))
				{
					state::compile_menu_open = false;
				}
				ImGui::End();
			}
			if (state::save_success_popup_open)
			{
				ImGui::Begin("Saved");
				ImGui::Text("File has succesfully been saved");
				if (ImGui::Button("OK"))
				{
					state::save_success_popup_open = false;
				}
				ImGui::End();
			}
			if (state::save_failure_popup_open)
			{
				ImGui::Begin("Saved failure");
				ImGui::Text("Error in saving file");
				if (ImGui::Button("OK"))
				{
					state::save_failure_popup_open = false;
				}
				ImGui::End();
			}
			if (state::new_script_menu_open)
			{
				ImGui::Begin("New Script");

				if (ImGui::Button("Close"))
				{
					state::new_script_menu_open = false;
				}
				ImGui::End();
			}
			if (state::text_editor_open)
			{
				ImGui::Begin("Text Editor", nullptr, aura::aura_imgui_static_window | ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("File"))
					{
						if (ImGui::MenuItem("New"))
						{
							state::new_script_menu_open = true;
						}
						if (ImGui::MenuItem("Save"))
						{
							std::string buffer = gloom::editor.GetText();
							if (!ap::OverwriteString(buffers::script_location, buffer))
							{
								state::save_success_popup_open = true;
							}
							else
							{
								state::save_failure_popup_open = true;
							}
						}
						if (ImGui::MenuItem("Open"))
						{
							state::load_script_menu_open = true;
						}
						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("Edit"))
					{
						bool ro = gloom::editor.IsReadOnly();
						if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
							gloom::editor.SetReadOnly(ro);
						ImGui::Separator();

						if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && gloom::editor.CanUndo()))
							gloom::editor.Undo();
						if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && gloom::editor.CanRedo()))
							gloom::editor.Redo();

						ImGui::Separator();

						if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, gloom::editor.HasSelection()))
							gloom::editor.Copy();
						if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && gloom::editor.HasSelection()))
							gloom::editor.Cut();
						if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && gloom::editor.HasSelection()))
							gloom::editor.Delete();
						if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
							gloom::editor.Paste();

						ImGui::Separator();

						if (ImGui::MenuItem("Select all", nullptr, nullptr))
							gloom::editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(gloom::editor.GetTotalLines(), 0));

						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}
				auto cpos = gloom::editor.GetCursorPosition();
				if (buffers::script_location[0] != 0)
				{
					ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, gloom::editor.GetTotalLines(), gloom::editor.IsOverwrite() ? "Ovr" : "Ins", gloom::editor.CanUndo() ? "*" : " ", gloom::editor.GetLanguageDefinition().mName.c_str(), buffers::script_location);
				}
				gloom::editor.Render("TE");
				ImGui::End();
			}
			if (state::load_script_menu_open)
			{
				ImGui::Begin("Load Scripts");
				ImGui::InputText("Script Location", buffers::script_location, sizeof(buffers::script_location));
				ImGui::SameLine();
				if (ImGui::Button("?"))
				{
					state::load_script_help_menu_open = true;
				}
				if (ImGui::Button("Load"))
				{
					gloom::editor_stream = std::ifstream(buffers::script_location);
					if (gloom::editor_stream.good())
					{
						std::string str((std::istreambuf_iterator<char>(gloom::editor_stream)), std::istreambuf_iterator<char>());
						gloom::editor.SetText(str);
					}
					else
					{
						state::load_script_error_menu_open = true;
					}
				}
				if (ImGui::Button("Close"))
				{
					state::load_script_menu_open = false;
				}
				ImGui::End();
			}
			if (state::load_script_help_menu_open)
			{
				ImGui::Begin("Help for loading scripts");
				ImGui::Text("Default location is the place where Aura.exe is placed.");
				if (ImGui::Button("Close"))
				{
					state::load_script_help_menu_open = false;
				}
				ImGui::End();
			}
			if (state::options_menu_open)
			{
				ImGui::Begin("Options");
				ImGui::BeginTabBar("0");
				ImGui::PushItemWidth(100.f);
				if (ImGui::BeginTabItem("Camera"))
				{
					ImGui::SliderFloat("Camera Sensitivity", &gloom::camera_sensitivity, 0.1f, 5.f);
					ImGui::SameLine();
					if (ImGui::Button("Reset Sens"))
					{
						gloom::camera_sensitivity = 0.2f;
					}
					ImGui::SliderFloat("Camera Move Speed", &gloom::camera_speed, 0.1f, 100.f);
					ImGui::SameLine();
					if (ImGui::Button("Reset Speed"))
					{
						gloom::camera_speed = 5.f;
					}
					if (ImGui::SliderFloat("Camera FOV (deg)", &gloom::field_of_view, 10.f, 130.f))
					{
						gloom::SetFieldOfView(gloom::field_of_view);
					}
					ImGui::SameLine();
					if (ImGui::Button("Reset FOV"))
					{
						gloom::SetFieldOfView(90.f);
					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Video"))
				{
					ImGui::PushItemWidth(200.f);
					if (ImGui::Checkbox("V-Sync", &gloom::vertical_sync))
					{
						glfwSwapInterval(gloom::vertical_sync);
					}
					ImGui::Text("!!!: V-Sync off is unstable");
					if (ImGui::SliderInt("Target FPS", &gloom::target_fps, 24, 240))
					{

					}
					ImGui::EndTabItem();
				}
				if (ImGui::BeginTabItem("Shader"))
				{
					if (ImGui::InputFloat("Ambient constant", &buffers::shader_ambient_strength))
					{
						gloom::WriteToShader(gloom::float_ambient_strength, buffers::shader_ambient_strength);
					}
					ImGui::SameLine();
					if (ImGui::Button("Reset AC"))
					{
						buffers::shader_ambient_strength = 0.1f;
						gloom::WriteToShader(gloom::float_ambient_strength, buffers::shader_ambient_strength);
					}
					if (ImGui::InputFloat("Specular constant", &buffers::shader_specular_strength))
					{
						gloom::WriteToShader(gloom::float_specular_strength, buffers::shader_specular_strength);
					}
					ImGui::SameLine();
					if (ImGui::Button("Reset SC"))
					{
						buffers::shader_specular_strength = 0.5f;
						gloom::WriteToShader(gloom::float_specular_strength, buffers::shader_specular_strength);
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
				if (ImGui::Button("Close"))
				{
					state::options_menu_open = false;
				}
				ImGui::End();
			}
			if (state::about_menu_open)
			{
				ImGui::Begin("About");
				ImGui::Text("Aura was developed by Steven Ning with C++ and OpenGL.");
				ImGui::Text("Visit the documentation page for help!");
				if (ImGui::Button("Close"))
				{
					state::about_menu_open = false;
				}
				ImGui::End();
			}
			if (state::scripts_menu_open)
			{
				ImGui::Begin("C++ files & scripts");
				ImGui::InputText(".cpp file location", buffers::script_location, sizeof(buffers::script_location));
				if (ImGui::Button("Load"))
				{

				}
				ImGui::SameLine();
				if (ImGui::Button("New"))
				{

				}
				if (ImGui::Button("Close"))
				{
					state::scripts_menu_open = false;
				}
				ImGui::End();
			}
			if (state::new_file_menu_open)
			{
				ImGui::Begin("New Project");
				ImGui::InputText("Project Name", buffers::project_name, buffers::k_char_input_max);
				ImGui::InputText("Path", buffers::project_location, buffers::k_char_input_max);
				if (ImGui::Button("Create"))
				{
					std::string temp_begin = std::string(buffers::project_location);
					gloom::NewPath(temp_begin);
					gloom::NewFile(temp_begin + std::string("/main.cpp"));
					gloom::NewFile(temp_begin + std::string("/mainscript.cpp"));
					gloom::NewFile(temp_begin + std::string("/project.aura"));
					gloom::NewPath(temp_begin + std::string("/models"));
					state::new_file_menu_open = false;
				}
				if (ImGui::Button("Close"))
				{
					state::new_file_menu_open = false;
				}
				ImGui::End();
			}
			if (state::load_script_error_menu_open)
			{
				ImGui::Begin("Error");
				ImGui::Text("Error: file does not exist or ifstream C++ error");
				if (ImGui::Button("OK"))
				{
					state::load_script_error_menu_open = false;
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
					aura::AuraParse mode = aura::AuraParse::k_null;
					int scene_number = 0;
					while (std::getline(file, line))
					{
						if (mode == aura::AuraParse::k_null)
						{
							if (ln == 1)
							{
								if (!(line.substr(0, 18) == "#filetype auraproj"))
								{
									break;
								}
							}
							if (line.substr(0, 16) == "#region : models")
							{
								mode = aura::AuraParse::k_model;
							}
							if (line.substr(0, 17) == "#region : objects")
							{
								mode = aura::AuraParse::k_object;
							}
							if (line.find("[region:lights") != std::string::npos)
							{
								mode = aura::AuraParse::k_light;
							}
							if (line.find("[region:scripts") != std::string::npos)
							{
								mode = aura::AuraParse::k_script;
							}
						}
						else if (mode == aura::AuraParse::k_model)
						{
							ap::ModelParse(line);
							if (line.substr(0, 10) == "#endregion")
							{
								mode = aura::AuraParse::k_null;
							}
						}
						else if (mode == aura::AuraParse::k_object)
						{
							if (line.substr(0, 10) == "#endregion")
							{
								mode = aura::AuraParse::k_null;
							}
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

				if (ImGui::Button("Close"))
				{
					state::save_file_menu_open = false;
				}
				ImGui::End();
			}
			if (state::theme_menu_open)
			{
				ImGui::Begin("Theme");
				if (ImGui::Button("Close"))
				{
					state::theme_menu_open = false;
				}
				ImGui::End();
			}
			if (state::models_menu_open)
			{
				ImGui::Begin("Models", nullptr, aura::aura_imgui_static_window);
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
				ImGui::Begin("Lights", nullptr, aura::aura_imgui_static_window);
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
						if (ImGui::Button("Delete Light"))
						{
							aura::active_scene->lights.erase(aura::active_scene->lights.begin() + i);
						}
						ImGui::TreePop();
					}
				}
				ImGui::End();
			}
			if (state::objects_menu_open)
			{
				ImGui::Begin("Objects", nullptr, aura::aura_imgui_static_window);
				if (ImGui::Button("Close"))
				{
					state::objects_menu_open = false;
				}
				for (int i = 0; i < aura::active_scene->objects.size(); i++)
				{
					if (ImGui::TreeNode((aura::active_scene->objects[i].name.c_str())))
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
						ImGui::InputText("Object Name", buffers::object_name, sizeof(buffers::object_name));
						if (ImGui::Button("Set Name"))
						{
							aura::active_scene->objects[i].name = std::string(buffers::object_name);
							buffers::Clear(buffers::object_name);
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
				ImGui::Begin("Context Editor", nullptr, aura::aura_imgui_static_window);
				ImGui::End();
			}
			if (state::console_open)
			{
				ImGui::Begin("Console", nullptr, aura::aura_imgui_static_window);
				ImGui::Text((std::string("OUT >> |: ") + std::string(buffers::console_output)).c_str());
				ImGui::InputText(">> console in", buffers::console_input, sizeof(buffers::console_input));
				if (ImGui::Button("Send"))
				{
					buffers::Clear(buffers::console_output);
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
		if (gloom::mouse_button_right_held)
		{
			gloom::CameraEnd();
		}
		gloom::FlipDisplay();
		gloom::Poll();
	}
	gloom::Terminate();
	return 0;
}