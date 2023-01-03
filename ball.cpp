#include "ball.h"

Ball::Ball() {}

Ball::Ball(Vector2d pos, double radius, double start_speed) :
    pos_(pos), radius_(radius), speed_(start_speed),
    direction_(Vector2d(1.0, 0.0))
{}

void Ball::update(const double dt) {
  pos_ += direction_ * speed_ * dt;
}
