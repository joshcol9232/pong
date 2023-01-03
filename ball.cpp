#include "ball.h"
#include "constants.h"

Ball::Ball() {}

Ball::Ball(Vector2d pos, double radius) :
    pos_(pos), radius_(radius), speed_(constants::BALL_STARTING_SPEED),
    direction_(Vector2d(1.0, 0.0))
{}

void Ball::update(const double dt) {
  pos_ += direction_ * speed_ * dt;
}
