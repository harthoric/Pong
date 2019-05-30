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
	// Initialise game
	bool Initialize();
	// Where game is updated
	void RunLoop();
	// Shutdown game
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
	// Holds number of ticks since game's start
	Uint32 mTicksCount;
	// To check for game exit or crash
	bool mIsGameRunning;

	// Direction of paddle 1
	int mPaddle1Direction;
	// Position of paddle 1
	Vector2 mPaddlePosition1;

	// Direction of paddle 2
	int mPaddleDirection2;
	// Position of paddle 2
	Vector2 mPaddlePosition2;

	// Position of ball
	Vector2 mBallPosition;
	// Velocity of ball
	Vector2 mBallVelocity;
};
