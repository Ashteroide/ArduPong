#pragma once
#include <stdint.h>

constexpr uint8_t ballWidth = 2;
constexpr uint8_t ballHeight = 2;

const unsigned char ball[] PROGMEM = {
  // Dimensions
  ballWidth, ballHeight,

  // Frame 0 - ball
  0x03, 0x03, 0x00, 0x00,
};
