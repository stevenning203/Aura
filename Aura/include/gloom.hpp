#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "stb_image.h"

#include "gloomcogs/sys.hpp"
#include "gloomcogs/debug.hpp"
#include "gloomcogs/shader.hpp"
#include "gloomcogs/const.hpp"
#include "gloomcogs/input.hpp"
#include "gloomcogs/camera.hpp"
#include "gloomcogs/model.hpp"
#include "gloomcogs/window.hpp"
#include "gloomcogs/2d.hpp"

#include "TextEditor.h"
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
	unsigned int TextureFromFile(const char * path, const std::string &directory, bool gamma = false);

	bool force_exit = false;

	int target_fps = 60;

	unsigned int shader;

	glm::mat4 identity_matrix(1.f);

	std::chrono::high_resolution_clock::time_point time_point0;
	std::chrono::high_resolution_clock::time_point time_point1;

	bool vertical_sync = true;

	unsigned int default_shader;

	TextEditor editor;
	std::ifstream editor_stream;
	
	void SetCurrentCamera(Camera* camera_set);

	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	glm::vec2 GetMousePos();

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

void gloom::Model::Enable()
{
	enabled = true;
}

void gloom::Model::Disable()
{
	enabled = false;
}

bool gloom::Model::IsEnabled()
{
	return enabled;
}

void gloom::Model::Draw(ModMat mod, std::vector<Light> &light_sources)
{
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(mod, light_sources);
}

void gloom::Model::LoadModel(std::string path, bool flip_uvs)
{
	Assimp::Importer importer;
	const aiScene* scene = flip_uvs ? importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs) : importer.ReadFile(path, aiProcess_Triangulate);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		if (path == "IRP")
			return;
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	dir = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

std::vector<gloom::Texture> gloom::Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_string)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true; 
				break;
			}
		}
		if (!skip)
		{   
			Texture texture;
			texture.id = TextureFromFile(str.C_Str(), this->dir);
			texture.type = type_string;
			if (type_string == "texture_diffuse")
			{
				texture.enum_type = Gloonum::k_gloom_texture_diffuse;
			}
			else if (type_string == "texture_specular")
			{
				texture.enum_type = Gloonum::k_gloom_texture_specular;
			}
			else if (type_string == "texture_normal")
			{
				texture.enum_type = Gloonum::k_gloom_texture_normal;
			}
			else
			{
				texture.enum_type = Gloonum::k_gloom_texture_missing;
			}
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);  
		}
	}
	return textures;
}

void gloom::Model::ProcessNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(ProcessMesh(mesh, scene));
	}
	
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

gloom::Mesh gloom::Model::ProcessMesh(aiMesh *mesh, const aiScene * scene)
{
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; 
			
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.pos = vector;
			
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.norm = vector;
			}
			
			if (mesh->mTextureCoords[0]) 
			{
				glm::vec2 vec;
				
				
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.tc = vec;
			}
			else
				vertex.tc = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		
		return Mesh(vertices, indices, textures);
	}
}

gloom::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void gloom::Mesh::SetupMesh()
{
	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, norm));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tc));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));
	
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, bitangent));

	glBindVertexArray(0);
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
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

bool gloom::Model::Valid()
{
	return !meshes.empty();
}

unsigned int gloom::TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGBA;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

void gloom::ForceExit()
{
	force_exit = true;
}

GLFWwindow* gloom::Init(int window_width, int window_height, const char* window_name, bool fullscreen)
{
	stbi_set_flip_vertically_on_load(1);
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
	orthographic_matrix.Set(glm::ortho(0.f, (float)window.width, (float)window.height, 0.f, 0.f, 1.f));
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
	(void)io;
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
	shader = ShaderInit("res/shaders/basic.shader");
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

void gloom::SetFieldOfView(float deg)
{
	field_of_view = deg;
	perspective_matrix.Set(glm::perspective(glm::radians(field_of_view), window.aspect_ratio, 0.1f, 100.f));
}

gloom::Camera* gloom::GetCurrentCamera()
{
	return current_camera;
}