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
