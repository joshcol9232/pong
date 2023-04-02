#include "bot.h"
#include "constants.h"
#include <math.h>

Bot::Bot(){
  std::random_device os_seed;
  const auto seed = os_seed();
  rand_generator_ = std::mt19937(seed);
}

void Bot::update(const float dt, Paddle* target_paddle) const {
  const float y = target_paddle->get_y();

  if (current_target_y_ == y) return;

  const float dy = dt * target_paddle->get_speed();

  if(current_target_y_ < y) {
    // Move up
    if (y - dy < current_target_y_) {  // Precise movement
      target_paddle->y_pos_ = current_target_y_;
    } else {
      target_paddle->move(dt, false);
    }
  } else {
    // Move down
    if (y + dy > current_target_y_) {  // Precise movement
      target_paddle->y_pos_ = current_target_y_;
    } else {
      target_paddle->move(dt, true);
    }
  }
}

float Bot::solve(const float x_pos, const float y_pos,
                 const float radius,
                 float ball_direction_x, float ball_direction_y,
                 const float target_x) const {
  const float m = ball_direction_y / ball_direction_x;
  const float c = y_pos - m * x_pos;

  // y of line at target_x
  const float y_result = m * target_x + c;

  if (y_result < radius) {
    // Get position of collision with wall
    // When y = radius
    const float x_col = (radius - c) / m;
    // reverse y direction
    ball_direction_y *= -1;
    return solve(x_col, radius, radius, ball_direction_x, ball_direction_y, target_x);

  } else if (y_result > constants::WINDOW_HEIGHT - radius) { // collision
    // Get position of collision with wall
    // When y = constants::WINDOW_HEIGHT - radius
    const float x_col = (constants::WINDOW_HEIGHT - radius - c) / m;
    // reverse y direction
    ball_direction_y *= -1;
    return solve(x_col,
                 constants::WINDOW_HEIGHT - radius,
                 radius, ball_direction_x, ball_direction_y, target_x);
  }

  return y_result;
}

void Bot::change_target(const Ball& b, const bool lefts_turn) {
  const float ball_direction_x = b.get_direction_x();
  const float ball_direction_y = b.get_direction_y();

  float target_x = constants::PADDLE_OFFSET +
                    constants::PADDLE_WIDTH/2;

  if (!lefts_turn) {   // Aim to other side of window
    target_x = constants::WINDOW_WIDTH - target_x;
  }

  current_target_y_ = solve(b.get_x(), b.get_y(),
                            b.get_radius(),
                            ball_direction_x, ball_direction_y,
                            target_x);

  // Add some innacuracy
  std::normal_distribution<float> distr(0, constants::BOT_INNACURACY_STD_DEV);
  current_target_y_ += std::clamp(distr(rand_generator_),
                                  -constants::PADDLE_HEIGHT/2,
                                  constants::PADDLE_HEIGHT/2);
}
