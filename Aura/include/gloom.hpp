#ifndef GLOOM_IMPORTED
#define GLOOM_IMPORTED
#define STB_IMAGE_IMPLEMENTATION
#define k_max_n_lights 100

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "stb_image.h"
#include "glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glad.c"

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <unordered_map>
#include <stdio.h>
#include <Windows.h>
#include <shellapi.h>
#include <math.h>
#include <thread>

typedef glm::vec3 glv3;
typedef glm::mat4 glm4;

namespace gloom
{
	unsigned int TextureFromFile(const char * path, const std::string &directory, bool gamma = false);
	enum class Gloonum
	{
		GLO_CAMERA_MODE_FREECAM, GLO_CAMERA_MODE_DISABLED, GLO_CAMERA_MODE_LIMITED, GLO_MOUSE_MODE_SHOW, GLO_MOUSE_MODE_HIDE, GLO_TEXTURE_DIFFUSE, GLO_TEXTURE_SPECULAR,
	};

	class Camera
	{
	private:
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 trg = glm::vec3(0.f, 0.f, 0.f);
		glm::mat4 view_matrix = glm::mat4(1.0f);
		Gloonum mode = Gloonum::GLO_CAMERA_MODE_FREECAM;
	public:
		void SetPos(glm::vec3 pos)
		{
			this->pos = pos;
			this->view_matrix = glm::lookAt(this->pos, this->trg, glm::vec3(0.f, 1.f, 0.f));
		}
		void SetTrg(glm::vec3 target)
		{
			this->trg = target;
			this->view_matrix = glm::lookAt(this->pos, this->trg, glm::vec3(0.f, 1.f, 0.f));
		}
		void SetMode(Gloonum mode)
		{
			this->mode = mode;
		}
		glm::mat4 GetMatrix()
		{
			return view_matrix;
		}
	};

	class ModMat
	{
		glm::mat4 internal = glm::mat4(1.0f);
	public:
		void Set(glm::mat4 matrix)
		{
			this->internal = matrix;
		}
		void Reset()
		{
			this->internal = glm::mat4(1.f);
		}
		void XYZ(glm::vec3 offset)
		{
			this->internal = glm::translate(this->internal, offset);
		}
		void Rotation(float degrees, glm::vec3 axes)
		{
			this->internal = glm::rotate(this->internal, glm::radians(degrees), axes);
		}
		void Scale(glm::vec3 axes)
		{
			this->internal = glm::scale(this->internal, axes);
		}
		glm::mat4 Get()
		{
			return this->internal;
		}
	};

	class ProjMat
	{
		glm::mat4 internal = glm::mat4(1.0f);
	public:
		void Set(glm::mat4 matrix)
		{
			this->internal = matrix;
		}
		glm::mat4 Get()
		{
			return this->internal;
		}
	};

	class UniformLocation
	{
	public:
		int val;
	};

	struct Light
	{
		glv3 color;
		glv3 direction;
		glv3 attenuation;
		glv3 position;
		float theta;
		Light(glv3 color = glv3(1.f, 1.f, 0.f), glv3 direction = glv3(0.f, 0.f, -1.f), glv3 attenuation = glv3(1.f), glv3 position = glv3(50.f), float theta = 1.f)
		{
			this->color = color;
			this->direction = direction;
			this->attenuation = attenuation;
			this->position = position;
			this->theta = theta;
		}
	};

	struct Sprite2D
	{
		unsigned int id = 0;
		unsigned int tid = 0;
		float r = 0.f, g = 0.f, b = 0.f;
		float width = 0, height = 0;
		float angle = 0;
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
		glm::vec2 pos = glm::vec2(0.f, 0.f);
		glm::vec2 point_of_rotation = glm::vec2(0, 0);
		Sprite2D(unsigned int buffer_id, glm::vec3 rgb, float width, float height, unsigned int texture_id = NULL)
		{
			this->id = buffer_id;
			this->tid = texture_id;
			this->r = rgb[0];
			this->g = rgb[1];
			this->b = rgb[2];
			this->width = width;
			this->height = height;
		}
		glm::vec2 GetCenter()
		{
			return glm::vec2(scale[0] * width / 2, scale[1] * height / 2);
		}
	};

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 norm;
		glm::vec2 tc;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	struct Texture
	{
		unsigned int id = 0;
		std::string type;
		std::string path;
	};

	struct Mesh
	{
	public:
		std::vector<Vertex>       vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture>      textures;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		void Draw(ModMat mod);
		void Draw(ModMat mod, Light* light_sources, int n_light_sources);
	private:
		unsigned int VAO, VBO, EBO;
		void SetupMesh();
	};

	class Model
	{
	public:
		ModMat matrix;
		Model(const char* path)
		{
			LoadModel(path);
		}
		void Draw(ModMat mod, Light* light_sources, int n_light_sources);
		void Draw(ModMat mod);
		void Draw();
		bool Valid();
		void Enable();
		void Disable();
		bool IsEnabled();
	private:
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string dir;
		void LoadModel(std::string path);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		bool enabled = true;
	};

	class Window
	{
	public:
		int width = 0;
		int height = 0;
	};

	bool force_exit = false;

	unsigned int shader;

	Camera* current_camera;

	Window window;

	int mouse_x, mouse_y;

	GLFWwindow* local_window;

	ProjMat perspective_matrix, orthographic_matrix;

	ModMat translationM, rotationM, scalingM;

	UniformLocation matrix_ortographic_location, matrix_model_location, matrix_projection_location, matrix_view_location;

	UniformLocation int_n_lights_location;

	UniformLocation struct_light_location[k_max_n_lights], struct_light_attenuation[k_max_n_lights], struct_light_color[k_max_n_lights], struct_light_theta[k_max_n_lights], struct_light_direction[k_max_n_lights];

	void SetCurrentCamera(Camera* camera_set);

	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	glm::vec2 GetMousePos();

	auto Split(std::string str, char split, bool fc = true);

	auto SplitString(std::string str, char split);

	void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);

	void ParseShader(std::string path, std::string* ptrToVertexShaderSrc, std::string* ptrToFragmentShaderSrc);

	unsigned int ShaderInit(const char* path);

	void Init(int window_width, int window_height, const char* window_name, bool fullscreen = NULL);

	void SetFieldOfView(float deg);

	void Poll();

	void FlipDisplay();

	bool QueueExit();

	bool GetKey(unsigned int glKeycode);

	void SetClearColor(float x, float y, float z, float a = 1.0f);

	void SetMouseMode(Gloonum mouse_mode);

	void SetMousePos(int x, int y);

	void Terminate();

	void ForceLowInput();

	void ClearBuffer();

	double GetTime();

	void Poll();

	void ForceExit();

	void WriteToShader(UniformLocation shader_location, glv3 vector);

	void WriteToShader(UniformLocation shader_location, int integer);

	void WriteToShader(UniformLocation shader_location, float floating_point);

	void WriteToShader(UniformLocation shader_location, glm4 matrix);

	void WriteToShader(UniformLocation shader_location, Light * light_sources, int n);

	Camera * GetCurrentCamera();
}

void gloom::WriteToShader(UniformLocation shader_location, Light* light_sources, int n)
{
	if (n > k_max_n_lights)
	{
		std::exit(0);
	}
	for (int i = 0; i < n; i++)
	{
		WriteToShader(struct_light_attenuation[i], light_sources[i].attenuation);
		WriteToShader(struct_light_location[i], light_sources[i].position);
		WriteToShader(struct_light_color[i], light_sources[i].color);
		WriteToShader(struct_light_theta[i], light_sources[i].theta);
		WriteToShader(struct_light_direction[i], light_sources[i].direction);
	}
}

void gloom::WriteToShader(UniformLocation shader_location, glv3 vector)
{
	glUniform3fv(shader_location.val, 1, &vector[0]);
}

void gloom::WriteToShader(UniformLocation shader_location, int integer)
{
	glUniform1i(shader_location.val, integer);
}

void gloom::WriteToShader(UniformLocation shader_location, float floating_point)
{
	glUniform1f(shader_location.val, floating_point);
}

void gloom::WriteToShader(UniformLocation shader_location, glm4 matrix)
{
	glUniformMatrix4fv(shader_location.val, 1, GL_FALSE, &matrix[0][0]);
}

void gloom::Poll()
{
	glfwPollEvents();
}

double gloom::GetTime()
{
	return glfwGetTime();
}

void gloom::ClearBuffer()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gloom::ForceLowInput()
{
	glFinish();
}

void gloom::Terminate()
{
	glfwTerminate();
}

void gloom::SetMousePos(int x, int y)
{
	glfwSetCursorPos(local_window, x, y);
}

void gloom::SetMouseMode(Gloonum mouse_mode)
{
	if (mouse_mode == gloom::Gloonum::GLO_MOUSE_MODE_HIDE)
	{
		glfwSetInputMode(local_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else if (mouse_mode == gloom::Gloonum::GLO_MOUSE_MODE_SHOW)
	{
		glfwSetInputMode(local_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void gloom::SetClearColor(float x, float y, float z, float a)
{
	glClearColor(x, y, z, a);
}

bool gloom::GetKey(unsigned int glKeycode)
{
	return glfwGetKey(local_window, glKeycode);
}

bool gloom::QueueExit()
{
	return glfwWindowShouldClose(local_window);
}

void gloom::FlipDisplay()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	glfwSwapBuffers(local_window);
}

glm::vec2 gloom::GetMousePos()
{
	return glm::vec2(mouse_x, mouse_y);
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

void gloom::Model::Draw(ModMat mod)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(mod);
}

void gloom::Model::Draw()
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(this->matrix);
}

void gloom::Model::Draw(ModMat mod, Light* light_sources, int n_light_sources)
{
	for (int i = 0; i < meshes.size(); i++)
		meshes[i].Draw(this->matrix, light_sources, n_light_sources);
}

void gloom::Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	dir = path.substr(0, path.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);
}

std::vector<gloom::Texture> gloom::Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
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
			texture.type = typeName;
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

void gloom::Mesh::Draw(ModMat mod, Light* light_sources, int n_light_sources)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		else if (name == "texture_normal")
			number = std::to_string(normalNr++);
		else if (name == "texture_height")
			number = std::to_string(heightNr++);
		glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	WriteToShader(matrix_projection_location, perspective_matrix.Get());
	WriteToShader(matrix_model_location, mod.Get());
	WriteToShader(matrix_view_location, current_camera->GetMatrix());
	WriteToShader(matrix_view_location, light_sources, n_light_sources);
	WriteToShader(int_n_lights_location, n_light_sources);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void gloom::Mesh::Draw(ModMat mod)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++); 
		else if (name == "texture_normal")
			number = std::to_string(normalNr++); 
		else if (name == "texture_height")
			number = std::to_string(heightNr++); 


		glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	WriteToShader(matrix_projection_location, perspective_matrix.Get());
	WriteToShader(matrix_model_location, mod.Get());
	WriteToShader(matrix_view_location, current_camera->GetMatrix());
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

auto gloom::Split(std::string str, char split, bool fc)
{
	std::vector<float> tv;
	int current = 1;
	std::string word = "";
	for (auto x : str)
	{
		if (x == split)
		{
			if (fc)
				tv.push_back(stof(word));
			word = "";
		}
		else
		{
			word = word + x;
		}
	}
	if (fc)
		tv.push_back(stof(word));
	return tv;
}

auto gloom::SplitString(std::string str, char split)
{
	std::vector<std::string> tv;
	int current = 1;
	std::string word = "";
	for (auto x : str)
	{
		if (x == split)
		{
			tv.push_back(word);
			word = "";
		}
		else
		{
			word = word + x;
		}
	}
	tv.push_back(word);
	return tv;
}

void gloom::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouse_x = (int)xpos;
	mouse_y = (int)ypos;
}

void gloom::ForceExit()
{
	gloom::force_exit = true;
}

void gloom::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void gloom::ParseShader(std::string path, std::string* ptr_vs_src, std::string* ptr_fs_src)
{
	std::string* ptvss = ptr_vs_src;
	std::string* ptfss = ptr_fs_src;
	std::string line;
	std::ifstream stream;
	std::stringstream string_stream[2];
	int vOF = 0;
	stream.open(path);
	while (getline(stream, line))
	{
		if (line.find("#region") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				vOF = 0;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				vOF = 1;
			}
		}
		else
		{
			string_stream[vOF] << line << '\n';
		}
	}
	*ptvss = string_stream[0].str();
	*ptfss = string_stream[1].str();
}

unsigned int gloom::ShaderInit(const char* path) {
	unsigned int vSID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fSID = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vSSrc;
	std::string fSSrc;
	ParseShader(path, &vSSrc, &fSSrc);
	GLint success = GL_FALSE;
	int InfoLogLength;
	char const* VertexSourcePointer = vSSrc.c_str();
	glShaderSource(vSID, 1, &VertexSourcePointer, NULL);
	glCompileShader(vSID);
	glGetShaderiv(vSID, GL_COMPILE_STATUS, &success);
	glGetShaderiv(vSID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(vSID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0];
	}
	char const* FragmentSourcePointer = fSSrc.c_str();
	glShaderSource(fSID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(fSID);
	glGetShaderiv(fSID, GL_COMPILE_STATUS, &success);
	glGetShaderiv(fSID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(fSID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0];
	}
	unsigned int ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vSID);
	glAttachShader(ProgramID, fSID);
	glLinkProgram(ProgramID);
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &success);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0];
	}

	glDetachShader(ProgramID, vSID);
	glDetachShader(ProgramID, fSID);

	glDeleteShader(vSID);
	glDeleteShader(fSID);

	return ProgramID;
}

void gloom::Init(int window_width, int window_height, const char* window_name, bool fullscreen)
{
	stbi_set_flip_vertically_on_load(1);
	std::srand((unsigned)time(0));
	int width = window_width;
	int height = window_height;
	window.width = window_width;
	window.height = window_height;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (fullscreen == true)
		local_window = glfwCreateWindow(width, height, window_name, glfwGetPrimaryMonitor(), NULL);
	else
		local_window = glfwCreateWindow(width, height, window_name, NULL, NULL);
	perspective_matrix.Set(glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f));
	orthographic_matrix.Set(glm::ortho(0.f, (float)width, (float)height, 0.f, -1.f, 1.f));
	glfwMakeContextCurrent(local_window);
	if (!local_window)
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
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(local_window, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(local_window, FrameBufferSizeCallback);
	shader = ShaderInit("res/shaders/basic.shader");
	glUseProgram(shader);
	matrix_model_location.val = glGetUniformLocation(shader, "matrix_model");
	matrix_projection_location.val = glGetUniformLocation(shader, "matrix_projection");
	matrix_view_location.val = glGetUniformLocation(shader, "matrix_view");
	matrix_ortographic_location.val = glGetUniformLocation(shader, "matrix_orthographic");
	int_n_lights_location.val = glGetUniformLocation(shader, "n_lights");
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glfwSetCursorPosCallback(local_window, CursorPosCallback);
}

void gloom::SetFieldOfView(float deg)
{
	perspective_matrix.Set(glm::perspective(glm::radians(deg), (float)window.width / (float)window.height, 0.1f, 150.f));
}

gloom::Camera * gloom::GetCurrentCamera()
{
	return current_camera;
}

#endif