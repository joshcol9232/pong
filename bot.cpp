#include "bot.h"
#include "constants.h"
#include <iostream>
#include <iomanip>

Bot::Bot()
{

}

void Bot::update(const double dt, Paddle* target_paddle) const {
  const double y = target_paddle->get_y();

  if (current_target_y_ == y) return;

  const double dy = dt * target_paddle->get_speed();

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

double Bot::solve(const double x_pos,
                  const double y_pos,
                  const double radius,
                  Vector2d ball_direction,
                  const double target_x) const {
  std::cout << std::setprecision(2) << std::fixed << std::showpoint;
  std::cout << "Bot::solve:\nInputs:" << std::endl
            << "\tx_pos=" << x_pos
            << "\ty_pos=" << y_pos << std::endl
            << "\tball_dir_x=" << ball_direction.x() << std::endl
            << "\tball_dir_y=" << ball_direction.y() << std::endl
            << "\ttarget_x=" << target_x << std::endl;

  const double m = ball_direction.y() / ball_direction.x();
  const double c = y_pos - m * x_pos;

  std::cout << "Calc:" << std::endl;
  std::cout << "\tm=" << m << "\tc=" << c << std::endl;

  // y of line at target_x
  const double y_result = m * target_x + c;
  std::cout << "\ty_result=" << y_result << std::endl;

  if (y_result < radius) {
    std::cout << "Bot::solve: wall collision..." << std::endl;
    // Get position of collision with wall
    // When y = radius
    const double x_col = (radius - c) / m;
    // reverse y direction
    ball_direction.y() *= -1;
    return solve(x_col, radius, radius, ball_direction, target_x);

  } else if (y_result > constants::WINDOW_HEIGHT - radius) { // collision
    std::cout << "Bot::solve: wall collision..." << std::endl;
    // Get position of collision with wall
    // When y = constants::WINDOW_HEIGHT - radius
    const double x_col = (constants::WINDOW_HEIGHT - radius - c) / m;
    // reverse y direction
    ball_direction.y() *= -1;
    return solve(x_col,
                 constants::WINDOW_HEIGHT - radius,
                 radius, ball_direction, target_x);
  }

  std::cout << "---------------------------------------" << std::endl;

  return y_result;
}

void Bot::change_target(const Ball& b, const bool lefts_turn) {
  const auto& ball_direction = b.get_direction();

  const double leading_paddle_edge = constants::PADDLE_OFFSET +
                                     constants::PADDLE_WIDTH/2;

  if (lefts_turn) {
    // Find intersection with left wall
    // x = 0
    current_target_y_ = solve(b.get_x(), b.get_y(), b.get_radius(),
                              ball_direction, leading_paddle_edge);
  } else {    // moving right
    // right wall
    // x = WINDOW_WIDTH
    current_target_y_ = solve(b.get_x(), b.get_y(), b.get_radius(), ball_direction,
                              constants::WINDOW_WIDTH - leading_paddle_edge);
  }
}
