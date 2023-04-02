#include <string>

#include "ball.h"
#include "paddle.h"
#include "constants.h"
#include "bot.h"

#define DT 1.0/60.0

namespace {

inline bool check_goal(const Ball& b, bool check_left) {
  return (check_left && b.get_x() < 0.0) ||
      (!check_left && b.get_x() > constants::WINDOW_WIDTH);
}

inline void increase_speed(Ball& b, Paddle& left, Paddle& right) {
  b.increase_speed();
  left.increase_speed();
  right.increase_speed();
}

}  // anonymous namespace

int main() {
  // setup
  Ball ball(constants::WINDOW_WIDTH/2, constants::WINDOW_HEIGHT/2,
            constants::BALL_RADIUS);

  Paddle left_paddle(true);
  Paddle right_paddle(false);

  size_t left_score = 0;
  size_t right_score = 0;

  // ------ Set up sprites ------

  // sf::CircleShape ball_shape(constants::BALL_RADIUS);
  // ball_shape.setFillColor(sf::Color::White);
  // ball_shape.setOrigin(constants::BALL_RADIUS,
  //                      constants::BALL_RADIUS);

  // sf::RectangleShape paddle_shape(sf::Vector2f(constants::PADDLE_WIDTH,
  //                                              constants::PADDLE_HEIGHT));
  // paddle_shape.setFillColor(sf::Color::White);
  // paddle_shape.setOrigin(constants::PADDLE_WIDTH/2.0,
  //                        constants::PADDLE_HEIGHT/2.0);

  // ----------------------------

  bool lefts_turn = false;   // left's turn to shoot?
  Paddle* target_paddle = &right_paddle;
  Bot bot;
  bot.change_target(ball, lefts_turn);

  float ball_max_speed = ball.get_speed();

  auto reset = [&]() {
    left_paddle.reset();
    right_paddle.reset();
    ball.reset();
    lefts_turn = false;
    target_paddle = &right_paddle;
    bot.change_target(ball, lefts_turn);
  };

  bool stop = false;
  
  while (!stop) {
    // 1) Get time diff?
    const float dt = 1.0/30;

    // update BOT
    if (constants::BOTS > 0)
      bot.update(dt, target_paddle);

    // 2) poll keyboard
    // Process inputs
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
    //   right_paddle.move(dt, false);
    // } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
    //   right_paddle.move(dt, true);
    // }

    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
    //   left_paddle.move(dt, false);
    // } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    //   left_paddle.move(dt, true);
    // }

    // 3) update
    ball.update(dt);

    if (check_goal(ball, true)) { // check left
      ++right_score;
      reset();
    } else if (check_goal(ball, false)) {
      ++left_score;
      reset();
    }

    if (lefts_turn && left_paddle.check_collision(ball)) {
      ball.collide(left_paddle);
      increase_speed(ball, left_paddle, right_paddle);
      lefts_turn = false;

      if (constants::BOTS > 0) {
        target_paddle = &right_paddle;
      }
      bot.change_target(ball, lefts_turn);
    } else if (!lefts_turn && right_paddle.check_collision(ball)) {
      ball.collide(right_paddle);
      increase_speed(ball, left_paddle, right_paddle);
      lefts_turn = true;

      if (constants::BOTS == 2) {
        target_paddle = &left_paddle;
        bot.change_target(ball, lefts_turn);
      }
    }

    // Draw

    // draw ball

    // draw paddles


  }

  return 0;
}

