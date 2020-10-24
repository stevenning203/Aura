#include "aura.hpp"

namespace state
{
	bool saved = false;
	bool new_file_menu_open = false;
	bool open_file_menu_open = false;
	bool save_file_menu_open = false;
	bool options_menu_open = false;
}

namespace buffers
{
	const int k_char_input_max = 150;
	char* project_name = new char[k_char_input_max];
	char* project_location = new char[k_char_input_max];
	void Set();
}

void buffers::Set()
{
	memset(project_name, NULL, k_char_input_max);
	memset(project_location, NULL, k_char_input_max);
}

int main()
{
	buffers::Set();
	gloom::Init(1280, 720, "Aura");
	gloom::Camera cameras[50];
	gloom::SetCurrentCamera(&cameras[0]);
	gloom::Model backpack("models/backpack.obj");
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
					if (ImGui::MenuItem("Menus"))
					{

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
				ImGui::InputText(".auraproj File Path", buffers::project_location, buffers::k_char_input_max);
				if (ImGui::Button("Load"))
				{
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
		}
		//endrender
		gloom::FlipDisplay();
		gloom::Poll();
	}
	gloom::Terminate();
	return 0;
}