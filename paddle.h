#ifndef PADDLE_H
#define PADDLE_H

#include <algorithm>
#include "collisionidentifier.h"

class Ball;

class Paddle {
public:
  Paddle(bool is_left);

  bool is_left() const { return left_side_; }
  void move(const float dt, const bool direction);
  void reset();
  void increase_speed();

  inline float get_x() const { return x_pos_; }
  inline float get_y() const { return y_pos_; }
  inline float get_speed() const { return speed_; }

  bool check_collision(const Ball& b) const;

  CollisionIdentifier collision_id() const;

  friend class Bot;

private:
  void set_y(const float y);

  bool left_side_;  // left = true, right = false
  float y_pos_;    // Position of centre of paddle
  const float x_pos_;
  float speed_;
};

#endif // PADDLE_H
