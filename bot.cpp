#include "bot.h"
#include "constants.h"
#include <iostream>
#include <iomanip>
#include <memory>

Bot::Bot() :
  solveHistory_(), current_target_y_(0)
{
  std::random_device os_seed;
  const auto seed = os_seed();
  rand_generator_ = std::mt19937(seed);
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
                  const double target_x) {
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

  solveHistory_.push_back({ sf::Vector2f(static_cast<float>(x_pos),    static_cast<float>(y_pos)),
                            sf::Vector2f(static_cast<float>(target_x), static_cast<float>(y_result)) });

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
  clearSolveHistory();  // Clear the solving history from current target
  const auto& ball_direction = b.get_direction();

  double target_x = constants::PADDLE_OFFSET +
                    constants::PADDLE_WIDTH/2;

  if (!lefts_turn) {   // Aim to other side of window
    target_x = constants::WINDOW_WIDTH - target_x;
  }

  current_target_y_ = solve(b.get_x(), b.get_y(),
                            b.get_radius(),
                            ball_direction,
                            target_x);

  // Add some innacuracy
  std::normal_distribution<double> distr(0, constants::BOT_INNACURACY_STD_DEV);
  current_target_y_ += std::clamp(distr(rand_generator_),
                                  -constants::PADDLE_HEIGHT/2 + 0.01,
                                  constants::PADDLE_HEIGHT/2 - 0.01);
}

void Bot::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  constexpr double half_thickness = 3.0 / 2.0;

  std::unique_ptr<SfLine> line;

  bool first = true;
  for (const auto& targetLine : solveHistory_) {
    if (first) {
      line.reset(new SfLine(targetLine.start, targetLine.end));
      first = false;
    } else {
      const SfLine line_segment(targetLine.start, targetLine.end);
      *line += line_segment;
    }
  }

  if (!first) [[likely]]
    target.draw(*line);
}
