#ifndef GLOOM_IMPORTED
#define GLOOM_IMPORTED
#define STB_IMAGE_IMPLEMENTATION

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
#include "gladsource.h"

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <unordered_map>
#include <stdio.h>

typedef glm::vec3 glv3;
typedef bool label;

namespace gloom
{
	enum class Gloonum
	{
		GLO_CAMERA_MODE_FREECAM, GLO_CAMERA_MODE_DISABLED, GLO_CAMERA_MODE_LIMITED, GLO_MOUSE_MODE_SHOW, GLO_MOUSE_MODE_HIDE, GLO_TEXTURE_DIFFUSE, GLO_TEXTURE_SPECULAR,
	};

	class Camera
	{
	private:
		glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
		glm::vec3 trg = glm::vec3(0.f, 0.f, 0.f);
		glm::mat4 ViewMatrix = glm::mat4(1.0f);
		Gloonum mode = Gloonum::GLO_CAMERA_MODE_FREECAM;
	public:
		void setPos(glm::vec3 pos)
		{
			this->pos = pos;
			this->ViewMatrix = glm::lookAt(this->pos, this->trg, glm::vec3(0.f, 1.f, 0.f));
		}
		void setTrg(glm::vec3 target)
		{
			this->trg = target;
			this->ViewMatrix = glm::lookAt(this->pos, this->trg, glm::vec3(0.f, 1.f, 0.f));
		}
		void setMode(Gloonum mode)
		{
			this->mode = mode;
		}
		glm::mat4 getMatrix()
		{
			return ViewMatrix;
		}
	};

	class ModMat
	{
		glm::mat4 internal = glm::mat4(1.0f);
	public:
		void set(glm::mat4 matrix)
		{
			this->internal = matrix;
		}
		void reset()
		{
			this->internal = glm::mat4(1.f);
		}
		void xyz(glm::vec3 offset)
		{
			this->internal = glm::translate(this->internal, offset);
		}
		void rotation(float degrees, glm::vec3 axes)
		{
			this->internal = glm::rotate(this->internal, glm::radians(degrees), axes);
		}
		void scale(glm::vec3 axes)
		{
			this->internal = glm::scale(this->internal, axes);
		}
		glm::mat4 get()
		{
			return this->internal;
		}
	};

	class ProjMat
	{
		glm::mat4 internal = glm::mat4(1.0f);
	public:
		void set(glm::mat4 matrix)
		{
			this->internal = matrix;
		}
		glm::mat4 get()
		{
			return this->internal;
		}
	};

	class UniformLoc
	{
	public:
		int val;
	};

	struct Sprite2D
	{
		unsigned int ID = 0;
		unsigned int tID = 0;
		float r = 0.f, g = 0.f, b = 0.f;
		float width = 0, height = 0;
		float angle = 0;
		glm::vec3 scale = glm::vec3(1.f, 1.f, 1.f);
		glm::vec2 pos = glm::vec2(0.f, 0.f);
		glm::vec2 pointOfRot = glm::vec2(0, 0);
		Sprite2D(unsigned int bufferID, glm::vec3 rgb, float width, float height, unsigned int textureID = NULL)
		{
			this->ID = bufferID;
			this->tID = textureID;
			this->r = rgb[0];
			this->g = rgb[1];
			this->b = rgb[2];
			this->width = width;
			this->height = height;
		}
		glm::vec2 getCenter()
		{
			return glm::vec2(scale[0] * width / 2, scale[1] * height / 2);
		}
	};

	struct Vertex
	{
		glm::vec3 pos;
		glm::vec2 norm;
		glm::vec2 tc;
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
		void draw(ModMat mod);
	private:
		unsigned int VAO, VBO, EBO;
		void setupMesh();
	};

	class Model
	{
	public:
		Model(const char* path)
		{
			loadModel(path);
		}
		void draw(ModMat mod);
	private:
		std::vector<Mesh> meshes;
		std::string dir;
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	};

	class Window
	{
	public:
		int width = 0;
		int height = 0;
	};

	Camera* currentCamera;

	Window window;

	int mouseX, mouseY;

	GLFWwindow* localWindow;

	ProjMat perspectiveM, orthographicM;

	ModMat translationM, rotationM, scalingM;

	UniformLoc boolTextUL, vecRGBUL, matOrthoUL, matModelUL, boolDIMUL, matMVPUL;

	void setCurrentCamera(Camera* camera_set);

	void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	glm::vec2 getMousePos();

	auto split(std::string str, char split, bool fc = true);

	auto split_string(std::string str, char split);

	void framebufferSizeCallback(GLFWwindow* window, int width, int height);

	void parseShader(std::string path, std::string* ptrToVertexShaderSrc, std::string* ptrToFragmentShaderSrc);

	unsigned int shaderInit(const char* path);

	void init(int windowWidth, int windowHeight, const char* windowName, bool fullScreen = NULL);

	void setFOV(float deg);

	void poll();

	void flipDisplay();

	bool queueExit();

	bool getKey(unsigned int glKeycode);

	void setClearColor(float x, float y, float z, float a = 1.0f);

	void setMouseMode(Gloonum mouseMode);

	void setMousePos(int x, int y);

	void terminate();

	void forceLowInput();

	void clearBuffer();

	double getTime();

	void poll();

	void ImguiBegin();

	void ImguiEnd();

	void ImguiMessage(const char* message);

	void LoadFont(const char* path_to_ttf);
}

void gloom::poll()
{
	glfwPollEvents();
}

double gloom::getTime()
{
	return glfwGetTime();
}

void gloom::clearBuffer()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void gloom::forceLowInput()
{
	glFinish();
}

void gloom::terminate()
{
	glfwTerminate();
}

void gloom::setMousePos(int x, int y)
{
	glfwSetCursorPos(localWindow, x, y);
}

void gloom::setMouseMode(Gloonum mouseMode)
{
	if (mouseMode == gloom::Gloonum::GLO_MOUSE_MODE_HIDE)
	{
		glfwSetInputMode(localWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	}
	else if (mouseMode == gloom::Gloonum::GLO_MOUSE_MODE_SHOW)
	{
		glfwSetInputMode(localWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void gloom::setClearColor(float x, float y, float z, float a)
{
	glClearColor(x, y, z, a);
}

bool gloom::getKey(unsigned int glKeycode)
{
	return glfwGetKey(localWindow, glKeycode);
}

bool gloom::queueExit()
{
	return glfwWindowShouldClose(localWindow);
}

void gloom::flipDisplay()
{
	glfwSwapBuffers(localWindow);
}

glm::vec2 gloom::getMousePos()
{
	return glm::vec2(mouseX, mouseY);
}

void gloom::Model::draw(ModMat mod)
{
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(mod);
}

void gloom::Model::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	dir = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

std::vector<gloom::Texture> gloom::Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture texture;
		texture.id = 0;//texture.id = stbi_load_from_file(str.C_Str(), dir);
		texture.type = typeName;
		texture.path = str.C_Str();
		textures.push_back(texture);
	}
	return textures;
}

void gloom::Model::processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	// then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

gloom::Mesh gloom::Model::processMesh(aiMesh *mesh, const aiScene * scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// process vertex positions, normals and texture coordinates
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.norm = vector;
		if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
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
	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}
	// process material
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

gloom::Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void gloom::Mesh::setupMesh()
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
	glBindVertexArray(0);
}

void gloom::setCurrentCamera(gloom::Camera* camera_set)
{
	currentCamera = camera_set;
}

void gloom::Mesh::draw(ModMat mod)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].type;
		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);
		//glUniform1f();
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glm::mat4 temp_mat = perspectiveM.get() * currentCamera->getMatrix() * mod.get();
	glUniformMatrix4fv(matMVPUL.val, 1, GL_FALSE, &temp_mat[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

auto gloom::split(std::string str, char split, bool fc)
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

auto gloom::split_string(std::string str, char split)
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

void gloom::cursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	mouseX = (int)xpos;
	mouseY = (int)ypos;
}

void gloom::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void gloom::parseShader(std::string path, std::string* ptrToVertexShaderSrc, std::string* ptrToFragmentShaderSrc)
{
	std::string* ptvss = ptrToVertexShaderSrc;
	std::string* ptfss = ptrToFragmentShaderSrc;
	std::string line;
	std::ifstream stream;
	std::stringstream stringStream[2];
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
			stringStream[vOF] << line << '\n';
		}
	}
	*ptvss = stringStream[0].str();
	*ptfss = stringStream[1].str();
}

unsigned int gloom::shaderInit(const char* path) {
	unsigned int vSID = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fSID = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vSSrc;
	std::string fSSrc;
	parseShader(path, &vSSrc, &fSSrc);
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

void gloom::init(int windowWidth, int windowHeight, const char* windowName, bool fullScreen)
{
	std::srand((unsigned)time(0));
	int width = windowWidth;
	int height = windowHeight;
	window.width = windowWidth;
	window.height = windowHeight;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if (fullScreen == true)
		localWindow = glfwCreateWindow(width, height, windowName, glfwGetPrimaryMonitor(), NULL);
	else
		localWindow = glfwCreateWindow(width, height, windowName, NULL, NULL);
	perspectiveM.set(glm::perspective(glm::radians(90.0f), (float)width / (float)height, 0.1f, 100.0f));
	orthographicM.set(glm::ortho(0.f, (float)width, (float)height, 0.f, -1.f, 1.f));
	glfwMakeContextCurrent(localWindow);
	if (!localWindow)
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
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(localWindow, true);
	ImGui_ImplOpenGL3_Init("#version 430 core");
	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(localWindow, framebufferSizeCallback);
	unsigned int shaderProgram = shaderInit("res/shaders/basic.shader");
	matMVPUL.val = glGetUniformLocation(shaderProgram, "mvp");
	boolTextUL.val = glGetUniformLocation(shaderProgram, "textureExists");
	vecRGBUL.val = glGetUniformLocation(shaderProgram, "objColor");
	matModelUL.val = glGetUniformLocation(shaderProgram, "model");
	matOrthoUL.val = glGetUniformLocation(shaderProgram, "ortho");
	boolDIMUL.val = glGetUniformLocation(shaderProgram, "mode");
	glUseProgram(shaderProgram);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);
	glfwSetCursorPosCallback(localWindow, cursorPosCallback);
}

void gloom::LoadFont(const char* path_to_ttf)
{

}

void gloom::ImguiBegin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void gloom::ImguiEnd()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gloom::setFOV(float deg)
{
	perspectiveM.set(glm::perspective(glm::radians(deg), (float)window.width / (float)window.height, 0.1f, 150.f));
}

#endif