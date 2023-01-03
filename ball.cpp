#include <cmath>

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
  if (pos_.y() < radius_ || pos_.y() > constants::WINDOW_HEIGHT - radius_) {
    direction_.y() *= -1;
  }
}

// DEBUG WITH MOUSE
void Ball::set_pos(const double x, const double y) {
  pos_.x() = x;
  pos_.y() = y;
}

void Ball::reset() {
  set_pos(constants::WINDOW_WIDTH/2,
          constants::WINDOW_HEIGHT/2);

  speed_ = constants::BALL_STARTING_SPEED;
  direction_.x() = 1.0;
  direction_.y() = 0.0;
}

void Ball::collide(const Paddle& p) {
  const double y_dist = p.get_y() - pos_.y();
  const double angle_ratio = y_dist / constants::PADDLE_HEIGHT/2.0;

  double deflection_angle = angle_ratio * M_PI;
  if (p.is_left()) {
    deflection_angle *= -1;
  } else {
    deflection_angle += M_PI;
  }

#ifdef DEBUG
  std::cout << "Deflection angle: " << deflection_angle * 180/M_PI << std::endl;
#endif

  direction_.x() = std::cos(deflection_angle);
  direction_.y() = std::sin(deflection_angle);
  speed_ *= constants::BALL_SPEED_MULTIPLIER;
}
