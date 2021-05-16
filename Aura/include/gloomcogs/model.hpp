#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "gloomcogs/shader.hpp"
#include "gloomcogs/matrix.hpp"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "gloomcogs/texture.hpp"
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

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
		{
			this->vertices = vertices;
			this->indices = indices;
			this->textures = textures;

			SetupMesh();
		}
		void Draw(ModMat mod, std::vector<Light>& light_sources);
	private:
		unsigned int VAO, VBO, EBO;
		void SetupMesh()
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
		void Draw(ModMat mod, std::vector<Light>& light_sources)
		{
			for (int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(mod, light_sources);
		}
		bool Valid()
		{
			return !meshes.empty();
		}
		void Enable()
		{
			enabled = true;
		}
		void Disable()
		{
			enabled = false;
		}
		bool IsEnabled()
		{
			return enabled;
		}
	private:
		std::vector<Texture> textures_loaded;
		std::vector<Mesh> meshes;
		std::string dir;
		void LoadModel(std::string path, bool flip_uvs)
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
		void ProcessNode(aiNode* node, const aiScene* scene)
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
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene)
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
		std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string type_string)
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
		bool enabled = true;
	};
}