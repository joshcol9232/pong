#ifndef BALL_H
#define BALL_H

#include <Eigen/Dense>

#include "paddle.h"

using Eigen::Vector2d;

class Paddle;  // declare

class Ball {
 public:
  Ball();
  Ball(Vector2d pos, double radius);

  void update(const double dt);
  void update_mouse(const double x, const double y);
  void collide(const Paddle& p);

  inline double get_x() const { return pos_.x(); }
  inline double get_y() const { return pos_.y(); }
  inline double get_radius() const { return radius_; }

 private:
  Vector2d pos_;
  double speed_;
  double radius_;
  Vector2d direction_;  // Unit vector
};

#endif // BALL_H
