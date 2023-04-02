#include "paddle.h"
#include "constants.h"
#include "ball.h"

Paddle::Paddle(bool is_left) :
  left_side_(is_left), y_pos_(constants::WINDOW_HEIGHT/2.0),
  speed_(constants::PADDLE_START_SPEED),
  x_pos_([&]() -> const float {
    return is_left ? constants::PADDLE_OFFSET :
      constants::WINDOW_WIDTH - constants::PADDLE_OFFSET;
  }())
{}

void Paddle::move(const float dt, const bool direction) {
  const float dy = speed_ *
      (static_cast<float>(direction) * 2 - 1) * dt;

  y_pos_ += dy;
  y_pos_ = std::clamp(y_pos_,
                      constants::PADDLE_HEIGHT/2,
                      constants::WINDOW_HEIGHT - constants::PADDLE_HEIGHT/2);
}

bool Paddle::check_collision(const Ball& b) const {
  const float dist_x = b.get_x() - x_pos_;
  const float dist_y = b.get_y() - y_pos_;

  return std::abs(dist_x) < constants::PADDLE_WIDTH/2 + constants::BALL_RADIUS &&
      std::abs(dist_y) < constants::PADDLE_HEIGHT/2 + constants::BALL_RADIUS;
}

void Paddle::reset() {
  y_pos_ = static_cast<float>(constants::WINDOW_HEIGHT/2);
  speed_ = constants::PADDLE_START_SPEED;
}

void Paddle::increase_speed() {
  speed_ += constants::PADDLE_SPEED_INCREMENT;
}

CollisionIdentifier Paddle::collision_id() const {
  if(left_side_)
    return CollisionIdentifier::LeftPaddle;
  else
    return CollisionIdentifier::RightPaddle;
}

void Paddle::set_y(const float y) {
  y_pos_ = std::clamp(y,
                      constants::PADDLE_HEIGHT/2,
                      constants::WINDOW_HEIGHT - constants::PADDLE_HEIGHT/2);
}
