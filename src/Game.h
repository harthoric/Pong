/*
 * Game.h
 *
 *  Created on: 4 Sep 2018
 *      Author: harthoric
 */

#pragma once
#include "SDL.h"

struct Vector2 {
	float x;
	float y;
};

class Game {
public:
	Game();
	// initialises game
	bool Initialize();
	// runs game loop until the game has finished
	void RunLoop();
	// shutdowns the game
	void Shutdown();
private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void resetBall();

	// SDL window
	SDL_Window* mWindow;
	// 2D renderer
	SDL_Renderer* mRenderer;
	// holds number of ticks since game's start
	Uint32 mTicksCount;
	// is the game running
	bool mIsGameRunning;

	// direction of paddle 1
	int mPaddle1Direction;
	// position of paddle 1
	Vector2 mPaddlePosition1;

	// direction of paddle 2
	int mPaddleDirection2;
	// position of paddle 2
	Vector2 mPaddlePosition2;

	// position of ball
	Vector2 mBallPosition;
	// velocity of ball
	Vector2 mBallVelocity;
};
