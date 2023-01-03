#include "paddle.h"
#include "constants.h"

Paddle::Paddle(bool is_left) :
  left_side_(is_left), y_pos_(constants::WINDOW_HEIGHT/2.0)
{}

void Paddle::move(const double dt, const bool direction) {
  y_pos_ += constants::PADDLE_SPEED * direction * dt;
}
