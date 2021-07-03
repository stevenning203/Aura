#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gloomcogs/matrix.hpp"
#include "GLFW/glfw3.h"
#include "gloomcogs/shader.hpp"
#include "TextEditor.h"
#include "stb_image.h"
#include "gloomcogs/camera.hpp"
#include <chrono>

namespace gloom
{
	class Window
	{
	public:
		GLFWwindow* local_window;
		int width = 0;
		int height = 0;
		glm::vec2 half_point = glm::vec2(0.f);
		float aspect_ratio = 1.f;
		void UpdateHalfPoint()
		{
			this->half_point[0] = (float)width / 2.f;
			this->half_point[1] = (float)height / 2.f;
		}
		void UpdateAspectRatio()
		{
			if (width && height)
				this->aspect_ratio = (float)width / (float)height;
			else
				this->aspect_ratio = 16.f / 9.f;
		}
	};

	Window window;

	float field_of_view = 90.f;

	ProjMat perspective_matrix, orthographic_matrix;

	TextEditor editor;
	std::ifstream editor_stream;

	bool force_exit = false;

	int target_fps = 60;

	bool vertical_sync = true;

	float time0 = 0, time1 = 0, time2 = 0;

	std::chrono::high_resolution_clock::time_point time_point0;
	std::chrono::high_resolution_clock::time_point time_point1;

	unsigned int default_shader;

	void FrameBufferSizeCallback(GLFWwindow* window_ptr, int width, int height)
	{
		window.width = width;
		window.height = height;
		window.UpdateHalfPoint();
		window.UpdateAspectRatio();
		glViewport(0, 0, width, height);
		perspective_matrix.Set(glm::perspective(glm::radians(field_of_view), window.aspect_ratio, 0.1f, 100.f));
		orthographic_matrix.Set(glm::ortho(0.f, (float)window.width, (float)window.height, 0.f, -1.f, 1.f));
	}

	void StateChangeShader(unsigned int shader_id)
	{
		glUseProgram(shader_id);
	}

	void SetDefaultShader(unsigned int shader_id)
	{
		default_shader = shader_id;
	}

	void ResetShaderDefault()
	{
		glUseProgram(default_shader);
	}

	void ClearBuffer()
	{
		time_point0 = std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		time0 = time1;
		time1 = (float)GetTime();
		time2 = time1 - time0;
	}

	void Terminate()
	{
		glfwTerminate();
	}

	void SetClearColor(float x, float y, float z, float a = 1.f)
	{
		glClearColor(x, y, z, a);
	}

	bool QueueExit()
	{
		return glfwWindowShouldClose(window.local_window) || gloom::force_exit;
	}

	void FlipDisplay()
	{
		mouse_button_left_down = 0;
		mouse_button_right_down = 0;
		mouse_button_middle_down = 0;
		mouse_button_left_up = 0;
		mouse_button_right_up = 0;
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window.local_window);
		time_point1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> delta = std::chrono::duration_cast<std::chrono::duration<double>>(time_point1 - time_point0);
		if (!vertical_sync)
		{
			double fps_inv = (double)1000 / (double)target_fps;
			double delta_count = (double)1000 * delta.count();
			double sleep_time = (int)(fps_inv - delta_count);
			if (sleep_time > k_max_fps_inv)
			{
				sleep_time = (int)k_max_fps_inv;
			}
			Sleep((DWORD)sleep_time);
		}
	}

	void ForceExit()
	{
		force_exit = true;
	}

	GLFWwindow* Init(int window_width, int window_height, const char* window_name, bool fullscreen = false)
	{
		stbi_set_flip_vertically_on_load(0);
		std::srand((unsigned)time(0));
		int width = window_width;
		int height = window_height;
		window.width = window_width;
		window.height = window_height;
		window.UpdateHalfPoint();
		window.UpdateAspectRatio();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		GLFWwindow* temp;
		if (!glfwInit())
		{
			glfwTerminate();
		}
		if (fullscreen == true)
			temp = glfwCreateWindow(width, height, window_name, glfwGetPrimaryMonitor(), NULL);
		else
			temp = glfwCreateWindow(width, height, window_name, NULL, NULL);
		gloom::window.local_window = temp;
		perspective_matrix.Set(glm::perspective(glm::radians(field_of_view), window.aspect_ratio, 0.1f, 100.0f));
		orthographic_matrix.Set(glm::ortho(0.f, (float)window.width, (float)window.height, 0.f, -1.f, 1.f));
		glfwMakeContextCurrent(window.local_window);
		if (!window.local_window)
		{
			std::cout << "Failed to init GLFW window" << std::endl;
			glfwTerminate();
		}
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		ImFont* google_sans = io.Fonts->AddFontFromFileTTF("res/fonts/google_sans.ttf", 18.f);
		ImGuiStyle& style = ImGui::GetStyle();

		style.WindowPadding = ImVec2(15, 15);
		style.WindowRounding = 1.0f;
		style.FramePadding = ImVec2(5, 5);
		style.FrameRounding = 1.0f;
		style.ItemSpacing = ImVec2(12, 8);
		style.ItemInnerSpacing = ImVec2(8, 6);
		style.IndentSpacing = 25.0f;
		style.ScrollbarSize = 15.0f;
		style.ScrollbarRounding = 9.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 3.0f;

		style.Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.16f, 0.15f, 0.17f, 1.00f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.80f, 0.80f, 0.83f, 0.88f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.15f, 0.15f, 0.15f, 0.65f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.f, 0.f, 0.f, 1.00f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.f, 0.f, 0.f, 1.00f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		ImGui_ImplGlfw_InitForOpenGL(window.local_window, true);
		ImGui_ImplOpenGL3_Init("#version 430 core");
		glViewport(0, 0, width, height);
		glfwSetFramebufferSizeCallback(window.local_window, FrameBufferSizeCallback);
		glfwSetMouseButtonCallback(window.local_window, MouseButtonCallback);
		unsigned int shader = ShaderInit("res/shaders/basic.shader");
		glUseProgram(shader);
		matrix_model_location.val = glGetUniformLocation(shader, "matrix_model");
		matrix_projection_location.val = glGetUniformLocation(shader, "matrix_projection");
		matrix_view_location.val = glGetUniformLocation(shader, "matrix_view");
		matrix_ortographic_location.val = glGetUniformLocation(shader, "matrix_orthographic");
		int_n_lights_location.val = glGetUniformLocation(shader, "n_lights");
		vector_camera_location.val = glGetUniformLocation(shader, "camera_position");
		float_ambient_strength.val = glGetUniformLocation(shader, "ambient_strength");
		float_specular_strength.val = glGetUniformLocation(shader, "specular_strength");
		WriteToShader(float_ambient_strength, 0.1f);
		WriteToShader(float_specular_strength, 0.5f);
		std::string temp_lights = "lights[";
		std::string temp_location = "].position";
		std::string temp_color = "].color";
		std::string temp_attenuation = "].attenuation";
		std::string temp_direction = "].direction";
		std::string temp_theta = "].theta";
		for (int i = 0; i < k_max_n_lights; i++)
		{
			struct_light_location[i].val = glGetUniformLocation(shader, (temp_lights + std::to_string(i) + temp_location).c_str());
			struct_light_color[i].val = glGetUniformLocation(shader, (temp_lights + std::to_string(i) + temp_color).c_str());
			struct_light_direction[i].val = glGetUniformLocation(shader, (temp_lights + std::to_string(i) + temp_direction).c_str());
			struct_light_attenuation[i].val = glGetUniformLocation(shader, (temp_lights + std::to_string(i) + temp_attenuation).c_str());
			struct_light_theta[i].val = glGetUniformLocation(shader, (temp_lights + std::to_string(i) + temp_theta).c_str());
		}
		for (int i = 0; i < k_max_n_diffuse; i++)
		{
			std::string temp_diffuse = "texture_diffuse";
			std::string temp_specular = "texture_specular";
			sampler_diffuse_location[i].val = glGetUniformLocation(shader, (temp_diffuse + std::to_string(i + 1)).c_str());
			sampler_specular_location[i].val = glGetUniformLocation(shader, (temp_specular + std::to_string(i + 1)).c_str());
		}
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glfwSetCursorPosCallback(window.local_window, CursorPosCallback);
		auto language = TextEditor::LanguageDefinition::CPlusPlus();
		editor.SetLanguageDefinition(language);
		return window.local_window;
	}
}