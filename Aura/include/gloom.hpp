#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "gloomcogs/sys.hpp"
#include "gloomcogs/debug.hpp"
#include "gloomcogs/shader.hpp"
#include "gloomcogs/const.hpp"
#include "gloomcogs/input.hpp"
#include "gloomcogs/camera.hpp"
#include "gloomcogs/model.hpp"
#include "gloomcogs/window.hpp"
#include "gloomcogs/2d.hpp"

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <shellapi.h>
#include <cmath>
#include <thread>

namespace gloom
{
	bool force_exit = false;

	int target_fps = 60;

	glm::mat4 identity_matrix(1.f);

	std::chrono::high_resolution_clock::time_point time_point0;
	std::chrono::high_resolution_clock::time_point time_point1;

	bool vertical_sync = true;

	unsigned int default_shader;
	
	void SetCurrentCamera(Camera* camera_set);

	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	void ParseShader(std::string path, std::string* vertex_shader_src_ptr, std::string* fragment_shader_src_ptr);

	unsigned int ShaderInit(const char* path);

	GLFWwindow* Init(int window_width, int window_height, const char* window_name, bool fullscreen = NULL);

	void SetFieldOfView(float deg);

	void FlipDisplay();

	void StateChangeShader(unsigned int shader_id);

	void SetDefaultShader(unsigned int shader_id);

	void ResetShaderDefault();

	bool QueueExit();

	void SetClearColor(float x, float y, float z, float a = 1.0f);

	void Terminate();

	void ClearBuffer();

	void ForceExit();

	void CameraBegin();

	void CameraEnd();

	Camera * GetCurrentCamera();
}

void gloom::StateChangeShader(unsigned int shader_id)
{
	glUseProgram(shader_id);
}

void gloom::SetDefaultShader(unsigned int shader_id)
{
	default_shader = shader_id;
}

void gloom::ResetShaderDefault()
{
	glUseProgram(default_shader);
}

void gloom::ClearBuffer()
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

void gloom::Terminate()
{
	glfwTerminate();
}

void gloom::SetClearColor(float x, float y, float z, float a)
{
	glClearColor(x, y, z, a);
}

bool gloom::QueueExit()
{
	return glfwWindowShouldClose(window.local_window) || gloom::force_exit;
}

void gloom::FlipDisplay()
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

void gloom::SetCurrentCamera(gloom::Camera* camera_set)
{
	current_camera = camera_set;
}

void gloom::Mesh::Draw(ModMat mod, std::vector<Light> &light_sources)
{
	int diffuse_index = 1;
	int specular_index = 1;
	int normal_index = 1;
	int height_index = 1;
	for (int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		if (textures[i].enum_type == Gloonum::k_gloom_texture_diffuse)
		{
			WriteToShader(sampler_diffuse_location[diffuse_index++], i);
		}
		else if (textures[i].enum_type == Gloonum::k_gloom_texture_specular)
		{
			WriteToShader(sampler_diffuse_location[specular_index++], i);
		}
		else if (textures[i].enum_type == Gloonum::k_gloom_texture_normal)
		{
			WriteToShader(sampler_diffuse_location[normal_index++], i);
		}

		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	WriteToShader(matrix_projection_location, perspective_matrix.GetPointer());
	WriteToShader(matrix_model_location, mod.GetPointer());
	WriteToShader(matrix_view_location, current_camera->GetMatrixPointer());
	WriteToShader(light_sources);
	WriteToShader(int_n_lights_location, (int)light_sources.size());
	WriteToShader(vector_camera_location, current_camera->GetPosPointer());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void gloom::ForceExit()
{
	force_exit = true;
}

void gloom::SetFieldOfView(float deg)
{
	field_of_view = deg;
	perspective_matrix.Set(glm::perspective(glm::radians(field_of_view), window.aspect_ratio, 0.1f, 100.f));
}

gloom::Camera* gloom::GetCurrentCamera()
{
	return current_camera;
}