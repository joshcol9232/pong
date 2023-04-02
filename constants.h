#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstddef>
#include <cmath>

namespace constants {
  const float PI = 3.14159265358979323846264338327950288419716939937510;
  const size_t WINDOW_WIDTH = 800;
  const size_t WINDOW_HEIGHT = 600;

  const float START_ANGLE_CLAMP = std::atan2(WINDOW_HEIGHT/2, WINDOW_WIDTH/2);

  const float BALL_START_SPEED = 500.0;
  const float BALL_RADIUS = 10.0;
  const float BALL_SPEED_INCREMENT = 100.0;

  const float PADDLE_WIDTH = 15.0;
  const float PADDLE_HEIGHT = 150.0;
  const float PADDLE_OFFSET = PADDLE_WIDTH + 10.0;
  const float PADDLE_START_SPEED = 400.0 + BALL_START_SPEED;
  const float PADDLE_SPEED_INCREMENT = BALL_SPEED_INCREMENT;

  // 0 = two humans
  // 1 = left -> human, right -> bot
  // 2 = both bots
  const char BOTS = 2;
  const float BOT_INNACURACY_STD_DEV = 0.25 * PADDLE_HEIGHT/2;
}  // namespace constants

#endif // CONSTANTS_H
