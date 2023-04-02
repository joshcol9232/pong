#ifndef BOT_H
#define BOT_H

#include "paddle.h"
#include "ball.h"
#include <random>

class Bot {
 public:
  Bot();

  void update(const float dt, Paddle* target) const;
  void change_target(const Ball& b, const bool lefts_turn);

 private:
  float solve(const float x_pos,
               const float y_pos,
               const float radius,
               float ball_direction_x_,
               float ball_direction_y_,
               const float target_x) const;

  float current_target_y_;
  std::mt19937 rand_generator_;
};

#endif // BOT_H
