#pragma once
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <Windows.h>
#include <direct.h>
#include <fstream>
#include <string>
#include <vector>

namespace gloom
{
	void NewFile(const char* path)
	{
		std::ofstream t(path);
		t.close();
	}

	void NewFile(std::string path)
	{
		std::ofstream t(path);
		t.close();
	}

	void NewPath(const char* path)
	{
		int nil = _mkdir(path);
	}

	void NewPath(std::string path)
	{
		int nil = _mkdir(path.c_str());
	}

	std::vector<float> Split(std::string str, char split, bool fc)
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

	std::vector<std::string> SplitString(std::string str, char split)
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
}