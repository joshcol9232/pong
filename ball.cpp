#include <math.h>
#include <algorithm>
#include <random>

#include "ball.h"
#include "constants.h"

namespace {

float random_angle(const float low, const float high) {
  std::random_device os_seed;
  const auto seed = os_seed();
  std::mt19937 generator(seed);
  std::uniform_real_distribution<float> distribute(low, high);
  return distribute(generator);
}

inline float random_start_angle() {
  return random_angle(-constants::START_ANGLE_CLAMP,
                      constants::START_ANGLE_CLAMP);
}

}

Ball::Ball(float pos_x, float pos_y, float radius) :
  pos_x_(pos_x), pos_y_(pos_y), radius_(radius),
  speed_(constants::BALL_START_SPEED)
{
  const float angle = random_start_angle();

  direction_x_ = std::cos(angle);
  direction_y_ = std::sin(angle);
}

void Ball::update(const float dt) {
  pos_x_ += direction_x_ * speed_ * dt;
  pos_y_ += direction_y_ * speed_ * dt;

  // wall bouncing
  if ((pos_y_ < radius_ && last_collided_with_ != CollisionIdentifier::BottomWall) ||
      (pos_y_ > constants::WINDOW_HEIGHT - radius_ &&
       last_collided_with_ != CollisionIdentifier::TopWall)) {

    direction_y_ *= -1;

    if (pos_y_ < radius_) {
      last_collided_with_ = CollisionIdentifier::BottomWall;
    } else {
      last_collided_with_ = CollisionIdentifier::TopWall;
    }
  }
}

void Ball::reset() {
  pos_x_ = constants::WINDOW_WIDTH/2;
  pos_y_ = constants::WINDOW_HEIGHT/2;

  speed_ = constants::BALL_START_SPEED;

  const float angle = random_start_angle();
  direction_x_ = cos(angle);
  direction_y_ = sin(angle);
}

void Ball::collide(const Paddle& p) {
  const float y_dist = p.get_y() - pos_y_;
  const float angle_ratio = y_dist / constants::PADDLE_HEIGHT/2.0;

  float deflection_angle = angle_ratio * constants::PI;

  if (p.is_left()) {
    deflection_angle *= -1;
  } else {
    deflection_angle += constants::PI;
  }

  last_collided_with_ = p.collision_id();

  direction_x_ = std::cos(deflection_angle);
  direction_y_ = std::sin(deflection_angle);
}

void Ball::increase_speed() {
  speed_ += constants::BALL_SPEED_INCREMENT;
}
