#ifndef BOT_H
#define BOT_H

#include "paddle.h"
#include "ball.h"
#include "SfLine.h"

#include <random>
#include <Eigen/Dense>
#include <SFML/Graphics.hpp>

using Eigen::Vector2d;

struct TargetLine {
  sf::Vector2f start;
  sf::Vector2f end;
};

class Bot : public sf::Drawable {
 public:
  Bot();

  void update(const double dt, Paddle* target) const;
  void change_target(const Ball& b, const bool lefts_turn);

  void draw(sf::RenderTarget &target, sf::RenderStates states) const;

 private:
  double solve(const double x_pos,
               const double y_pos,
               const double radius,
               Vector2d ball_direction,
               const double target_x);
  
  void clearSolveHistory() { solveHistory_.clear(); }

  std::vector<TargetLine> solveHistory_;

  double current_target_y_;
  std::mt19937 rand_generator_;
};

#endif // BOT_H
