#include "mainscript.cpp"

int main()
{
	gloom::Init(1920, 1080, "Game");
	while (!gloom::QueueExit)
	{
		gloom::ClearBuffer();
		
		gloom::FlipDisplay();
		gloom::Poll();
	}
}