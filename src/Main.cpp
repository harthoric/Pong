//============================================================================
// Name        : Game.cpp
// Author      : Harthoric
// Version     :
// Copyright   : Free to use
// Description : Pong in C++, Ansi-style
//============================================================================

#include "Game.h"

int main(int argc, char** argv) {
	Game game;
	bool success = game.Initialize();
	if (success) {
		game.RunLoop();
	}
	game.Shutdown();
	return 0;
}
