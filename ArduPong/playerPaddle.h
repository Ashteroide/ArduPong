// Paddle, 2x10px
#pragma once

#include <stdint.h>

constexpr uint8_t paddleWidth = 2;
constexpr uint8_t paddleHeight = 10;

const unsigned char playerPaddle[] PROGMEM = {
  // Dimensions
  paddleWidth, paddleHeight,

  // Frame 0 - playerPaddle
  0xff, 0xff, 0x03, 0x03,
};
