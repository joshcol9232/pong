#include <cmath>
#include <iostream>
#include <algorithm>
#include <random>

#include "ball.h"
#include "constants.h"

namespace {

double random_angle(const double low, const double high) {
  std::random_device os_seed;
  const auto seed = os_seed();
  std::mt19937 generator(seed);
  std::uniform_real_distribution<double> distribute(low, high);
  return distribute(generator);
}

inline double random_start_angle() {
  return random_angle(-constants::START_ANGLE_CLAMP,
                      constants::START_ANGLE_CLAMP);
}

}

Ball::Ball(Vector2d pos, double radius) :
  pos_(pos), radius_(radius),
  speed_(constants::BALL_START_SPEED)
{
  const double angle = random_start_angle();

  direction_.x() = std::cos(angle);
  direction_.y() = std::sin(angle);
}

void Ball::update(const double dt) {
  pos_ += direction_ * speed_ * dt;

  // wall bouncing
  if ((pos_.y() < radius_ && last_collided_with_ != CollisionIdentifier::BottomWall) ||
      (pos_.y() > constants::WINDOW_HEIGHT - radius_ &&
       last_collided_with_ != CollisionIdentifier::TopWall)) {

    direction_.y() *= -1;

    if (pos_.y() < radius_) {
      last_collided_with_ = CollisionIdentifier::BottomWall;
    } else {
      last_collided_with_ = CollisionIdentifier::TopWall;
    }
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

  speed_ = constants::BALL_START_SPEED;

  const double angle = random_start_angle();
  std::cout << "ANGLE: " << angle << std::endl;
  direction_.x() = cos(angle);
  direction_.y() = sin(angle);
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

  last_collided_with_ = p.collision_id();

  direction_.x() = std::cos(deflection_angle);
  direction_.y() = std::sin(deflection_angle);
}

void Ball::increase_speed() {
  speed_ += constants::BALL_SPEED_INCREMENT;
}
