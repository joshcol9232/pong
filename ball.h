#ifndef BALL_H
#define BALL_H

#include <Eigen/Dense>
#include <random>

#include "paddle.h"
#include "collisionidentifier.h"

using Eigen::Vector2d;

class Paddle;  // declare

class Ball {
 public:
  Ball(Vector2d pos, double radius);

  void update(const double dt);
  void set_pos(const double x, const double y);
  void collide(const Paddle& p, const bool bot);
  void reset();
  void increase_speed();

  inline double get_x() const { return pos_.x(); }
  inline double get_y() const { return pos_.y(); }
  inline double get_radius() const { return radius_; }
  inline double get_speed() const { return speed_; }
  inline const Vector2d& get_direction() const { return direction_; }

 private:
  Vector2d pos_;
  double speed_;
  double radius_;
  Vector2d direction_;  // Unit vector
  CollisionIdentifier last_collided_with_;
};

#endif // BALL_H
