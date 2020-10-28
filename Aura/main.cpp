#include "aura.hpp"

namespace console
{
	void Parse(char* input)
	{

	}
}

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
	void Set();
	void Clear(char* buffer)
	{
		memset(buffer, NULL, k_char_input_max);
	}
}

void buffers::Set()
{
	memset(project_name, NULL, k_char_input_max);
	memset(project_location, NULL, k_char_input_max);
	memset(console_output, NULL, k_char_input_max);
	memset(console_input, NULL, k_char_input_max);
}

int main()
{
	buffers::Set();
	gloom::Init(1280, 720, "Aura");
	gloom::Camera cameras[50];
	gloom::SetCurrentCamera(&cameras[0]);
	gloom::Model backpack("models/backpack.obj");
	std::vector<gloom::Model> models;
	std::vector<std::string> model_names;
	gloom::SetClearColor(0.5f, 0.5f, 0.5f);
	while (!gloom::QueueExit())
	{
		gloom::ClearBuffer();
		//render

		cameras[0].SetPos(glm::vec3(-3.f, 0.f, 3.f));
		backpack.Draw();

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
						if (ImGui::MenuItem("Models"))
						{
							state::models_menu_open = true;
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
					while (std::getline(file, line))
					{
						break;
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

			}
			if (state::models_menu_open)
			{
				ImGui::Begin("Models");
				if (ImGui::Button("Load Models"))
				{
					state::load_models_menu_open = true;
				}
				ImGui::End();
			}
			if (state::load_models_menu_open)
			{
				ImGui::Begin("Load Model");
				ImGui::InputText("Model Location", buffers::model_location, sizeof(buffers::model_location));
				ImGui::InputText("Model Name", buffers::model_name, sizeof(buffers::model_name));
				if (ImGui::Button("Load"))
				{
					ImGui::Text("LOADING...");
					gloom::Model temp_model(buffers::model_location);
					std::cout << temp_model.Valid() << std::endl;
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