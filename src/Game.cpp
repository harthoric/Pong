/*
 * Game.cpp
 *
 *  Created on: 4 Sep 2018
 *      Author: harthoric
 */

#include "Game.h"
#include <iostream>

// thickness of paddle
const int thickness = 15;
// height of paddle
const float paddleHeight = 100.0f;
// velocity the ball should increment each time
const float incrementVelocity = 1.05f;
// general velocity of player 2 paddle
float paddle2Velocity = 1.00f;
// score of player 1
int scorePlayer1 = 0;
int scorePlayer2 = 0;

Game::Game() :
		mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsGameRunning(true), mPaddleDirection1(
				0), mPaddleDirection2(0) {

}

bool Game::Initialize() {
	// initialise SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
		return false;
	}

	// create a SDL Window
	mWindow = SDL_CreateWindow("Pong!", 100, 100, 1024, 768, 0);

	// if there is no window
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// create SDL renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// if there is no renderer
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

  // initialise paddle 1 position (x, y)
	mPaddlePosition1.x = 10.0f;
	mPaddlePosition1.y = 768.0f / 2.0f;
	// initialise paddle 2 position (x, y)
	mPaddlePosition2.x = 1024.0f - 30.0f;
	mPaddlePosition2.y = 768.0f / 2.0f;
	// initialise ball position (x, y)
	mBallPosition.x = 1024.0f / 2.0f;
	mBallPosition.y = 768.0f / 2.0f;
	// initialise ball velocity (x, y)
	mBallVelocity.x = -200.0f;
	mBallVelocity.y = 235.0f;
	// if all is good, default true
	return true;
}

void Game::RunLoop() {
	// while the game is running
	while (mIsGameRunning) {
		// process user input (keys)
		ProcessInput();
		// update the game continuously
		UpdateGame();
		// draw to display / show result output
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		// if we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mIsGameRunning = false;
			break;
		}
	}

	// get keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// if escape is pressed
	if (state[SDL_SCANCODE_ESCAPE])
		// end game loop
		mIsGameRunning = false;

    mPaddleDirection1 = 0;
	// if 'w' is pressed
	if (state[SDL_SCANCODE_W])
		// decrement player 1 paddle direction
		mPaddleDirection1--;

	// if 's' is pressed
	if (state[SDL_SCANCODE_S])
		// increment player 1 paddle direction
		mPaddleDirection1++;

	mPaddleDirection2 = 0;

	// if the ball if closer to player 2's paddle
	if (mBallPosition.x > 1024.0f - (1024.0f / 2.0f) * paddle2Velocity) {
		// if the paddle's y position is greater than that of the ball's, and the ball is closer to the right
		if (mPaddlePosition2.y > mBallPosition.y)
			// decrement player 2 paddle direction
			mPaddleDirection2 -= paddle2Velocity;
		// if the paddle's y position is less than that of the ball's, and the ball if closer to the right
		if (mPaddlePosition2.y < mBallPosition.y)
			// increment player 2 paddle direction
			mPaddleDirection2 += paddle2Velocity;
	}
}

void Game::UpdateGame() {
	// wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

    // the difference in ticks from last frame converted to seconds (1000 ticks in a second in this case)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// clamp maximum delta time value (restrict its value)
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// update tick counts (for the next frame)
	mTicksCount = SDL_GetTicks();
}

void Game::GenerateOutput() {
// set draw color to black - R (red) G (Green) B (Blue) A (Alpha)
	SDL_SetRenderDrawColor(mRenderer, 0, 0,	0, 255);

// clear back buffer
	SDL_RenderClear(mRenderer);

// swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
