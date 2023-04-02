#ifndef BALL_H
#define BALL_H

#include "paddle.h"
#include "collisionidentifier.h"

class Paddle;  // declare

class Ball {
 public:
  Ball(float pos_x, float pos_y, float radius);

  void update(const float dt);
  void collide(const Paddle& p);
  void reset();
  void increase_speed();

  inline float get_x() const { return pos_x_; }
  inline float get_y() const { return pos_y_; }
  inline float get_radius() const { return radius_; }
  inline float get_speed() const { return speed_; }
  inline float get_direction_x() const { return direction_x_; }
  inline float get_direction_y() const { return direction_y_; }

 private:
  float pos_x_, pos_y_;
  float speed_;
  float radius_;
  float direction_x_, direction_y_;  // Unit vector
  CollisionIdentifier last_collided_with_;
};

#endif // BALL_H
