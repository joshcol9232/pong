#ifndef BALL_H
#define BALL_H

#include <Eigen/Dense>

using Eigen::Vector2d;

class Ball {
 public:
  Ball();
  Ball(Vector2d pos, double radius, double start_speed);

  void update(const double dt);

  inline const Vector2d& get_pos() const { return pos_; }

 private:
  Vector2d pos_;
  double speed_;
  double radius_;
  Vector2d direction_;  // Unit vector
};

#endif // BALL_H
