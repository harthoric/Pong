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
			mIsRunning = false;
			break;
		}
	}

	// get keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// if escape is pressed
	if (state[SDL_SCANCODE_ESCAPE])
		// end game loop
		mIsRunning = false;
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
