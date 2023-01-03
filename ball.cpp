#include "ball.h"
#include "constants.h"

Ball::Ball() {}

Ball::Ball(Vector2d pos, double radius) :
    pos_(pos), radius_(radius), speed_(constants::BALL_STARTING_SPEED),
    direction_(Vector2d(1.0, 0.0))
{}

void Ball::update(const double dt) {
  pos_ += direction_ * speed_ * dt;

  // wall bouncing
  if (pos_.y() < 0.0 || pos_.y() > constants::WINDOW_HEIGHT) {
    direction_.y() *= -1;
  }
}

// DEBUG WITH MOUSE
void Ball::update_mouse(const double x, const double y) {
  pos_.x() = x;
  pos_.y() = y;
}

void Ball::collide(const Paddle& p) {
  const double y_dist = p.get_y() - pos_.y();

//  const double deflection_angle_ratio = x_dist
}
