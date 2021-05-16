#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gloomcogs/shader.hpp"
#include "gloomcogs/matrix.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <iostream>

namespace gloom
{

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
		Gloonum enum_type = Gloonum::k_gloom_texture_null;
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
		void Draw(ModMat mod, std::vector<Light>& light_sources);
	private:
		unsigned int VAO, VBO, EBO;
		void SetupMesh();
	};

	class Particle
	{
	private:
		glm::vec3 position = glm::vec3(0.f);
		glm::vec3 acceleration = glm::vec3(0.f);
		glm::vec3 velocity = glm::vec3(0.f);
		float life = 100.f;
		float decay = 1.f;
	public:
		Particle(glm::vec3 accel, glm::vec3 vel, float life, float decay)
		{
			this->acceleration = accel;
			this->velocity = vel;
			this->life = life;
			this->decay = decay;
		}
		bool Update()
		{
			if (this->life <= 0)
			{
				return false;
			}
			this->velocity = this->velocity + this->acceleration;
			this->position = this->position + this->velocity;
			this->life -= this->decay;
			return true;
		}
		void SetAcceleration(glm::vec3 v3)
		{
			this->acceleration = v3;
		}
		glm::vec3 GetAcceleration()
		{
			return this->acceleration;
		}
		void SetVelocity(glm::vec3 v3)
		{
			this->velocity = velocity;
		}
		glm::vec3 GetVelocity()
		{
			return this->velocity;
		}
		void SetLife(float f)
		{
			this->life = f;
		}
		float GetLife()
		{
			return this->life;
		}
		void SetDecay(float d)
		{
			this->decay = decay;
		}
		float GetDecay()
		{
			return this->decay;
		}
		void Draw()
		{

		}
	};

	class Model
	{
	public:
		Model(const char* path = "IRP", bool flip_uvs = true)
		{
			LoadModel(path, flip_uvs);
		}
		void Draw(ModMat mod, std::vector<Light>& light_sources);
		bool Valid();
		void Enable();
		void Disable();
		bool IsEnabled();
	private:
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string dir;
		void LoadModel(std::string path, bool flip_uvs);
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_string);
		bool enabled = true;
	};
}