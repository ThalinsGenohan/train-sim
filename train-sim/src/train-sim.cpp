#include "pch.h"

#include "Game.h"

int main()
{
	auto game = new ott::Game();
	game->Run();

	delete game;

	return 0;
}
