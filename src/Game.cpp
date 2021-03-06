/*
 * Game.cpp
 *
 *  Created on: 4 Sep 2018
 *      Author: harthoric
 */

#include "Game.h"
#include <iostream>
#include <list>
#include <map>

// Thickness of paddle
const int thickness = 15;
// Height of paddle
const float paddleHeight = 100.0f;
// Velocity the ball should increment each time
const float incrementVelocity = 1.05f;
// General velocity of player 2 paddle
float paddle2Velocity = 1.00f;
// Score of player 1
int scorePlayer1 = 0;
int scorePlayer2 = 0;

Game::Game() :
		mWindow(nullptr), mRenderer(nullptr), mTicksCount(0), mIsGameRunning(
				true), mPaddle1Direction(0), mPaddleDirection2(0) {

}

bool Game::Initialize() {
	// Initialise SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0) {
		SDL_Log("Unable to initialise SDL: %s", SDL_GetError());
		return false;
	}

	// Create a SDL Window
	mWindow = SDL_CreateWindow("Pong!", 100, 100, 1024, 768, 0);

	// If there is no window
	if (!mWindow) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// Create SDL renderer
	mRenderer = SDL_CreateRenderer(mWindow, -1,
			SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	// If there is no renderer
	if (!mRenderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	// Initialise paddle 1 position (x, y)
	mPaddlePosition1.x = 10.0f;
	mPaddlePosition1.y = 768.0f / 2.0f;
	// Initialise paddle 2 position (x, y)
	mPaddlePosition2.x = 1024.0f - 30.0f;
	mPaddlePosition2.y = 768.0f / 2.0f;
	// Initialise ball position (x, y)
	mBallPosition.x = 1024.0f / 2.0f;
	mBallPosition.y = 768.0f / 2.0f;
	// Initialise ball velocity (x, y)
	mBallVelocity.x = -200.0f * 1.22;
	mBallVelocity.y = 235.0f * 1.22;
	// If all is good, default true
	return true;
}

void Game::RunLoop() {
	// While the game is running
	while (mIsGameRunning) {
		// Process user input (keys)
		ProcessInput();
		// Update the game continuously
		UpdateGame();
		// Draw to display / show result output
		GenerateOutput();
	}
}

void Game::ProcessInput() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mIsGameRunning = false;
			break;
		}
	}

	// Get keyboard state
	const Uint8* state = SDL_GetKeyboardState(NULL);
	// If 'escape' key is pressed
	if (state[SDL_SCANCODE_ESCAPE])
		// End game loop
		mIsGameRunning = false;

	mPaddle1Direction = 0;
	// If 'w' key is pressed
	if (state[SDL_SCANCODE_W])
		// decrement player 1 paddle direction
		mPaddle1Direction--;

	// If 's' key is pressed
	if (state[SDL_SCANCODE_S])
		// Increment player 1 paddle direction
		mPaddle1Direction++;

	mPaddleDirection2 = 0;

	// If the ball if closer to player 2's paddle
	if (mBallPosition.x > 1024.0f - (1024.0f / 2.0f) * paddle2Velocity) {
		// If the paddle's y position is greater than that of the ball's, and the ball is closer to the right
		if (mPaddlePosition2.y > mBallPosition.y)
			// Decrement player 2 paddle direction
			mPaddleDirection2 -= paddle2Velocity;
		// If the paddle's y position is less than that of the ball's, and the ball if closer to the right
		if (mPaddlePosition2.y < mBallPosition.y)
			// Increment player 2 paddle direction
			mPaddleDirection2 += paddle2Velocity;
	}
}

void Game::UpdateGame() {
	// Wait until 16ms has elapsed since last frame
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
		;

	// The difference in ticks from last frame converted to seconds (1000 ticks in a second in this case)
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;

	// Clamp maximum delta time value (restrict its value)
	if (deltaTime > 0.05f) {
		deltaTime = 0.05f;
	}

	// Update tick counts (for the next frame)
	mTicksCount = SDL_GetTicks();

	// Update paddle 1 position based on direction
	if (mPaddle1Direction != 0) {
		mPaddlePosition1.y += mPaddle1Direction * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePosition1.y < (paddleHeight / 2.0f + thickness)) {
			mPaddlePosition1.y = paddleHeight / 2.0f + thickness;
		} else if (mPaddlePosition1.y
				> (768.0f - paddleHeight / 2.0f - thickness)) {
			mPaddlePosition1.y = 768.0f - paddleHeight / 2.0f - thickness;
		}
	}

	// Update paddle 2 position based on direction
	if (mPaddleDirection2 != 0) {
		mPaddlePosition2.y += mPaddleDirection2 * 300.0f * deltaTime;
		// Make sure paddle doesn't move off screen!
		if (mPaddlePosition2.y < (paddleHeight / 2.0f + thickness)) {
			mPaddlePosition2.y = paddleHeight / 2.0f + thickness;
		} else if (mPaddlePosition2.y
				> (768.0f - paddleHeight / 2.0f - thickness)) {
			mPaddlePosition2.y = 768.0f - paddleHeight / 2.0f - thickness;
		}
	}

	// Update ball position based on ball velocity
	mBallPosition.x += mBallVelocity.x * deltaTime;
	mBallPosition.y += mBallVelocity.y * deltaTime;

	// Bounce if needed
	// Did we intersect with the paddle?
	float diff1 = mPaddlePosition1.y - mBallPosition.y;
	float diff2 = mPaddlePosition2.y - mBallPosition.y;
	// Take absolute value of difference
	diff1 = (diff1 > 0.0f) ? diff1 : -diff1;
	diff2 = (diff2 > 0.0f) ? diff2 : -diff2;
	if ((diff1 <= paddleHeight / 2.0f && mBallPosition.x <= 30.0f
			&& mBallPosition.x >= 20.0f && mBallVelocity.x < 0.0f)
			|| (diff2 <= paddleHeight / 2.0f
					&& mBallPosition.x >= 1024.0f - 45.0f
					&& mBallPosition.x <= 1024.0f - 40.0f
					&& mBallVelocity.x > 0.0f)) {
		if (mBallVelocity.y != 500) {
			std::cout << mBallVelocity.y << std::endl;
			mBallVelocity.x *= -incrementVelocity;
			mBallVelocity.y *= incrementVelocity;
			paddle2Velocity *= incrementVelocity;
		} else {
			mBallVelocity.x = -incrementVelocity;
		}
	}

	// Did player 2 hit the ball?
	else if (diff2 <= paddleHeight / 2.0f && mBallPosition.x >= 1024.0f - 20.0f
			&& mBallPosition.x <= 1024.0f && mBallVelocity.x > 0.0f) {
		mBallVelocity.x *= -incrementVelocity;
		mBallVelocity.y *= incrementVelocity;
	}

	// Did the ball go off the screen? (if so, end game)
	else if (mBallPosition.x <= 0.0f && scorePlayer1 >= 1) {
//		mBallVelocity.x /= 2.0f;
//		mBallVelocity.y /= 2.0f;
//		paddle2Velocity /= 2.0f;
		mBallVelocity.x = -200.0f * 1.22;
		mBallVelocity.y = 235.0f * 1.22;
		paddle2Velocity = 1.00f;
		scorePlayer2++;
		ResetBall();
	} else if (mBallPosition.x >= (1024.0f - thickness)
			&& mBallVelocity.x > 0.0f && scorePlayer1 <= 9) {
		scorePlayer1++;
		ResetBall();
	} else if ((scorePlayer1 < 1 && mBallPosition.x <= 0.0f)
			|| scorePlayer1 > 9) {
		ResetBall();
	}

	// Did the ball collide with the top wall?
	if (mBallPosition.y <= thickness && mBallVelocity.y < 0.0f) {
		mBallVelocity.y *= -1;
	}
	// Did the ball collide with the bottom wall?
	else if (mBallPosition.y >= (768 - thickness) && mBallVelocity.y > 0.0f) {
		mBallVelocity.y *= -1;
	}

}

// Reset the ball position to centre
void Game::ResetBall() {
	mBallPosition.x = 1024 / 2;
	mBallPosition.y = 1024 / 2;
}

void Game::GenerateOutput() {
// Set draw color to black - R (red) G (Green) B (Blue) A (Alpha)
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);

// Clear back buffer
	SDL_RenderClear(mRenderer);

	// Draw walls
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

	// Draw top wall
	SDL_Rect wall { 0, 0, 1024, thickness };
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw bottom wall
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// Draw player1 paddle
	SDL_Rect paddle1 { static_cast<int>(mPaddlePosition1.x),
			static_cast<int>(mPaddlePosition1.y - paddleHeight / 2), thickness,
			static_cast<int>(paddleHeight) };
	SDL_RenderFillRect(mRenderer, &paddle1);

	// Draw player2 paddle
	SDL_Rect paddle2 { static_cast<int>(mPaddlePosition2.x),
			static_cast<int>(mPaddlePosition2.y - paddleHeight / 2), thickness,
			static_cast<int>(paddleHeight) };
	SDL_RenderFillRect(mRenderer, &paddle2);

	// Draw ball
	SDL_Rect ball { static_cast<int>(mBallPosition.x - thickness / 2),
			static_cast<int>(mBallPosition.y - thickness / 2), thickness,
			thickness };
	SDL_RenderFillRect(mRenderer, &ball);

	// Draw line
	for (int i = 1; i < 17; i++) {
		SDL_Rect line { static_cast<int>(1024 / 2),
				static_cast<int>(40 * i + 30), thickness / 2, 30 };
		SDL_RenderFillRect(mRenderer, &line);
	}

	for (int i = 0; i < 2; i++) {
		// Since I can't manage to get ttf working, I'm resorting to using eight rectangles to map the digits
		SDL_Rect leftTop {
				static_cast<int>(i < 1 ? 245 + i * 10 : 645 + i * 120),
				static_cast<int>(55), thickness / 3, thickness };
		SDL_Rect top { static_cast<int>(i < 1 ? 250 + i * 10 : 650 + i * 120),
				static_cast<int>(50), thickness, thickness / 3 };
		SDL_Rect rightTop { static_cast<int>(
				i < 1 ? 265 + i * 10 : 665 + i * 120), static_cast<int>(55),
				thickness / 3, thickness };
		SDL_Rect middle {
				static_cast<int>(i < 1 ? 250 + i * 10 : 650 + i * 120),
				static_cast<int>(70), thickness, thickness / 3 };
		SDL_Rect leftBottom { static_cast<int>(
				i < 1 ? 245 + i * 10 : 645 + i * 120), static_cast<int>(75),
				thickness / 3, thickness };
		SDL_Rect bottom {
				static_cast<int>(i < 1 ? 250 + i * 10 : 650 + i * 120),
				static_cast<int>(90), thickness, thickness / 3 };
		SDL_Rect rightBottom { static_cast<int>(
				i < 1 ? 265 + i * 10 : 665 + i * 120), static_cast<int>(75),
				thickness / 3, thickness };

		std::map<int, std::list<SDL_Rect>> dig;

		dig[0] = {leftTop, top, rightTop, leftBottom, bottom,
			rightBottom};
		dig[1] = {rightTop, rightBottom};
		dig[2] = {top, rightTop, middle, leftBottom, bottom};
		dig[3] = {top, rightTop, middle, bottom, rightBottom};
		dig[4] = {leftTop, rightTop, middle, rightBottom};
		dig[5] = {leftTop, top, middle, bottom, rightBottom};
		dig[6] = {leftTop, top, middle, leftBottom, bottom,
			rightBottom};
		dig[7] = {top, rightTop, rightBottom};
		dig[8] = {leftTop, top, rightTop, middle, leftBottom,
			bottom, rightBottom};
		dig[9] = {leftTop, top, rightTop, middle, bottom,
			rightBottom};

		for (unsigned j = 0; j < dig.size(); j++) {
			std::list<SDL_Rect>::iterator it = dig[
					i < 1 ? scorePlayer1 : scorePlayer2].begin();
			std::advance(it, j);
			SDL_RenderFillRect(mRenderer, &*it);
		}
	}

// Swap front buffer and back buffer
	SDL_RenderPresent(mRenderer);
}

void Game::Shutdown() {
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}
