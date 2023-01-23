#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <cmath>

namespace constants {
  const size_t WINDOW_WIDTH = 800;
  const size_t WINDOW_HEIGHT = 600;

  const double START_ANGLE_CLAMP = std::atan2(WINDOW_HEIGHT/2, WINDOW_WIDTH/2);

  // 0 = two humans
  // 1 = left -> human, right -> bot
  // 2 = both bots
  const char BOTS = 1;

  const double BALL_START_SPEED = 350.0;
  const double BALL_RADIUS = 10.0;
  const double BALL_SPEED_INCREMENT = 50.0;
  const double HIT_RANDOM_LIM = M_PI/2;
  const double HIT_RANDOM_STD_DEV = 0.15;

  const double PADDLE_WIDTH = 15.0;
  const double PADDLE_HEIGHT = 150.0;
  const double PADDLE_OFFSET = PADDLE_WIDTH + 10.0;
  const double PADDLE_START_SPEED = 400.0 + BALL_START_SPEED;
  const double PADDLE_SPEED_INCREMENT = BALL_SPEED_INCREMENT;
}  // namespace constants

#endif // CONSTANTS_H
