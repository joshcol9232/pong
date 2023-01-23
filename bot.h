#ifndef BOT_H
#define BOT_H

#include "paddle.h"
#include "ball.h"
#include <random>
#include <Eigen/Dense>

using Eigen::Vector2d;

class Bot {
 public:
  Bot();

  void update(const double dt, Paddle* target) const;
  void change_target(const Ball& b, const bool lefts_turn);

 private:
  double solve(const double x_pos,
               const double y_pos,
               const double radius,
               Vector2d ball_direction,
               const double target_x) const;

  double current_target_y_;
  std::mt19937 rand_generator_;
};

#endif // BOT_H
