#ifndef BALL_H
#define BALL_H

#include <Eigen/Dense>

using Eigen::Vector2d;

class Ball {
 public:
  Ball();
  Ball(Vector2d pos, double radius);

  void update(const double dt);
//  void bounce();

  inline const double get_x() const { return pos_.x(); }
  inline const double get_y() const { return pos_.y(); }

 private:
  Vector2d pos_;
  double speed_;
  double radius_;
  Vector2d direction_;  // Unit vector
};

#endif // BALL_H
