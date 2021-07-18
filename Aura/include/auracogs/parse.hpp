#pragma once
#include "gloom.hpp"
#include "auracogs/object.hpp"

namespace ap
{
	std::unordered_map<int, gloom::Model> model_hash;
	std::unordered_map<std::string, aura::Object> object_hash;
	void Throw(const char* text)
	{
		throw std::invalid_argument(text);
	}

	bool CheckFileExistsByDeletion(const char* path)
	{
		if (std::remove(path) == 0)
		{
			return true;
		}
		return false;
	}

	bool RenameFile(const char* file, const char* new_name)
	{
		if (std::rename(file, new_name) != 0)
		{
			return false;
		}
		return false;
	}

	void FillFileContents(const char* file_name, std::string& contents)
	{
		std::ofstream file_stream(file_name);
		file_stream << contents;
	}

	bool SaveNewFileContents(std::string path, std::string& contents)
	{
		if (CheckFileExistsByDeletion(path.c_str()))
		{
			return false;
		}
		FillFileContents("temp.cpp", contents);
		return RenameFile("temp.cpp", path.c_str());
	}

	void ObjectParse(std::string line)
	{
		if (line.substr(0, 13) == ">>NewObject()")
		{
			int len = stoi(line.substr(14, 3));
			int model_id = stoi(line.substr(18, 4));
			std::string id = line.substr(23, len);
			object_hash[id] = aura::Object(model_hash[model_id]);
		}
		else if (line.substr(0, 10) == "#endregion")
		{
			return;
		}
		else
		{
			Throw("Parsing error: >>NewObject() or #endregion text missing for objects region");
		}
	}
	void ModelParse(std::string line)
	{
		if (line.substr(0, 12) == ">>NewModel()")
		{
			int len = stoi(line.substr(13, 3));
			int id = stoi(line.substr(17, 4));
			std::string path = line.substr(22, len);
			model_hash[id] = gloom::Model(path.c_str());
		}
		else if (line.substr(0, 10) == "#endregion")
		{
			return;
		}
		else
		{
			Throw("Parsing error: >>NewModel() or #endregion text missing for models region");
		}
	}
	void Compile()
	{

	}
}