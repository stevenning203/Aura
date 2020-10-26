#include "gloom.hpp"

namespace aura
{
	class Object
	{
		Object(const char* path)
		{
			gloom::Model model(path);
		}
	};
}