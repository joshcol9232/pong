#ifndef PADDLE_H
#define PADDLE_H

#include "collisionidentifier.h"
#include "ball.h"

class Ball;

class Paddle {
public:
  Paddle(bool is_left);

  bool is_left() const { return left_side_; }
  void move(const double dt, const bool direction);
  void reset();
  void increase_speed();

  const double get_x() const { return x_pos_; }
  const double get_y() const { return y_pos_; }

  bool check_collision(const Ball& b) const;

  CollisionIdentifier collision_id() const;

private:
  bool left_side_;  // left = true, right = false
  double y_pos_;    // Position of centre of paddle
  const double x_pos_;
  double speed_;
};

#endif // PADDLE_H
