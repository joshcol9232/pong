#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>

namespace constants {
  const size_t WINDOW_WIDTH = 800;
  const size_t WINDOW_HEIGHT = 600;

  const double BALL_STARTING_SPEED = 100.0;
  const double BALL_RADIUS = 10.0;
  const double BALL_SPEED_MULTIPLIER = 1.1;

  const double PADDLE_WIDTH = 15.0;
  const double PADDLE_HEIGHT = 150.0;
  const double PADDLE_OFFSET = PADDLE_WIDTH + 10.0;
  const double PADDLE_SPEED = 500.0;
}  // namespace constants

#endif // CONSTANTS_H
