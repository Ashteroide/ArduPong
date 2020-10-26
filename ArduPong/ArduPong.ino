#include <Arduboy2.h>
#include <Tinyfont.h>
Arduboy2 arduboy;

#include "ball.h"
#include "court.h"
#include "playerPaddle.h"
#include "titleArduPong.h"

enum class GameState
{
  TitleScreen,
  GamePlay,
  GameOver,
};

GameState gameState = GameState::TitleScreen;

// 28 Pixels on the X for Made By Ashteroide

// Constants
constexpr int targetScore = 3;

constexpr float initialVelocityX[2] { -1, 1 };
constexpr float initialVelocityY[2] { -1, 1 };

const int initialPaddleY = ((Arduboy2::height() - paddleHeight) / 2);

const int screenCentreX = (Arduboy2::width() / 2);

const int initBallQuarter = (Arduboy2::height() / 4);
const int initBallHalf = (Arduboy2::height() / 2);
const int initialBallY[3] = { initBallQuarter, initBallHalf, (Arduboy2::height() - initBallQuarter) };

// Player 1
int player1Y;
const int player1X = 2;
int player1Points;

// Player 2
int player2Y;
const int player2X = (Arduboy2::width() - paddleWidth - 2);
int player2Points;

// Ball
int ballX;
int ballY;

// Ball Velocity
int velocityX;
int velocityY;

// Screen
const int screenRightEdge = Arduboy2::width() - 2;
const int screenTopEdge = 0;
const int screenBottomEdge = (Arduboy2::height() - 2);

// Reset
void reset()
{
  randomiseVelocity();
  resetPaddles();
  resetBall();
  resetPoints();
}

void resetGame()
{
  resetPaddles();
  randomiseVelocity();
  resetBall();
}

void resetPaddles()
{
  player1Y = initialPaddleY;
  player2Y = initialPaddleY;
}

void resetPoints()
{
  player1Points = 0;
  player2Points = 0;
}

void resetBall()
{
  ballX = screenCentreX;

  size_t ballIndexY = random(0, 2);
  ballY = initialBallY[ballIndexY];
}

// Randomise Velocity
void randomiseVelocity()
{
  randomiseVelocityX();
  randomiseVelocityY();
}

int randomiseVelocityX()
{
  size_t indexX = random(0, 1);
  return velocityX = initialVelocityX[indexX];
}

int randomiseVelocityY()
{
  size_t indexY = random(0, 1);
  return velocityY = initialVelocityY[indexY];
}

void setup()
{
  arduboy.begin();
}

void loop()
{
  if(!arduboy.nextFrame())
    return;

  arduboy.pollButtons();

  arduboy.clear();

  switch (gameState)
  {
    case GameState::TitleScreen:
      updateTitleScreen();
      drawTitleScreen();
      break;

    case GameState::GamePlay:
      updateGamePlay();
      drawGamePlay();
      break;

    case GameState::GameOver:
      updateGameOver();
      drawGameOver();
      break;
  }

  arduboy.display();
}

// Title Screen
void updateTitleScreen()
{
  if (arduboy.justPressed(LEFT_BUTTON))
  {
    gameState = GameState::GamePlay;
    arduboy.initRandomSeed();
    reset();
  }
}

// Draw Title Screen
void drawTitleScreen()
{
  Sprites::drawSelfMasked(20, 11, titleArduPong, 0);
  arduboy.setCursor(7, 35);
  arduboy.print(F("Press Left to Start"));
  
}

// Game Play
void updateGamePlay()
{
  updatePaddle();
  updateBall();
  updatePlayerPoints();
}

// Draw Game Play
void drawGamePlay()
{
  drawBall();
  Sprites::drawSelfMasked(0, 0, court, 0);
  drawPlayerPoints();
  drawPaddle();
}


// Game Over!
void updateGameOver()
{
  if(arduboy.justPressed(RIGHT_BUTTON))
    gameState = GameState::TitleScreen;
}

// Draw Game Over
void drawGameOver()
{
  arduboy.setCursor(25, 24);

  if(player1Points == targetScore)
    arduboy.print(F("Player 1 Won!"));

  if(player2Points == targetScore)
    arduboy.print(F("Player 2 Won!"));

  arduboy.setCursor(22, 34);
  arduboy.print(F("Right to Reset"));
}

// Paddle
void updatePaddle()
{
  const int topLimit = 1;
  const int bottomLimit = (Arduboy2::height() - paddleHeight);

  if(arduboy.pressed(UP_BUTTON) && (player1Y > topLimit))
    --player1Y;
  
  if(arduboy.pressed(DOWN_BUTTON) && (player1Y < bottomLimit))
    ++player1Y;
  
  if(arduboy.pressed(B_BUTTON) && (player2Y > topLimit))
    --player2Y;
  
  if(arduboy.pressed(A_BUTTON) && (player2Y < bottomLimit))
    ++player2Y;
}

// Draw Paddle
void drawPaddle()
{
  Sprites::drawSelfMasked(player1X, player1Y, playerPaddle, 0);
  Sprites::drawSelfMasked(player2X, player2Y, playerPaddle, 0);
}

// Bool Function
bool isBetween(int value, int inclusiveMinimum, int exclusiveMaximum)
{
  return ((value >= inclusiveMinimum) && (value < exclusiveMaximum));
}

// Ball
void updateBall()
{
  ballX += velocityX;
  ballY += velocityY;
  
  if((ballY < 2) || (ballY > (Arduboy2::height() - 2)))
    velocityY = -velocityY;
  
  if(ballOnPlayer1())
    velocityX = -velocityX;
  
  if(ballOnPlayer2())
    velocityX = -velocityX;

  if((arduboy.pressed(UP_BUTTON) && isBallGoingDown()) || (arduboy.pressed(B_BUTTON) && isBallGoingDown()))
  {
    if(ballOnPlayer1()) velocityY = -velocityY;

    if(ballOnPlayer2())velocityY = -velocityY;
  }
  
  if((arduboy.pressed(DOWN_BUTTON) && isBallGoingUp()) || (arduboy.pressed(A_BUTTON) && isBallGoingUp()))
  {
    if(ballOnPlayer1())
      velocityY = -velocityY;
    if(ballOnPlayer2())
      velocityY = -velocityY;
  }
}

// Ball on Player Paddle, True or False?
bool ballOnPlayer1()
{
  return ((ballY >= player1Y) && (ballY < (player1Y + paddleHeight)) && (ballX == player1X + 1));
}

bool ballOnPlayer2()
{
  return ((ballY >= player2Y) && (ballY < (player2Y + paddleHeight)) && (ballX == player2X - 1));
}

// Ball Direction, True or False?
bool isBallGoingUp()
{
  return (velocityY < 0);
}

bool isBallGoingDown()
{
  return (velocityY > 0);
}

// Draw Ball
void drawBall()
{
  Sprites::drawSelfMasked(ballX, ballY, ball, 0);
}

// Player Points
void updatePlayerPoints()
{
  if (ballX > Arduboy2::width())
  {
    resetGame();
    player1Points += 1;
  }
  else if(ballX < -paddleWidth)
  {
    resetGame();
    player2Points += 1;
  }

  if(player1Points == targetScore)
    gameState = GameState::GameOver;
  
  if(player2Points == targetScore)
    gameState = GameState::GameOver;
}

// Draw Player Points
void drawPlayerPoints()
{
  arduboy.setCursor(29, 2);
  arduboy.print(player1Points);
  arduboy.setCursor((Arduboy2::width() - 29), 2);
  arduboy.print(player2Points);
}