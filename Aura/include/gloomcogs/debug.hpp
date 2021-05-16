#pragma once
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace debug
{
	void LogMatrix(glm::mat4* matrix)
	{
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::cout << (*matrix)[j][i] << " | ";
			}
			std::cout << std::endl;
		}
	}

	template <typename v>
	void LogVector(v* vector)
	{
		for (int i = 0; i < vector->length(); i++)
		{
			std::cout << (*vector)[i] << " | ";
		}
		std::cout << std::endl;
	}
}