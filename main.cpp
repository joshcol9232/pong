#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include <Eigen/Dense>

#include "ball.h"
#include "paddle.h"
#include "constants.h"
#include "bot.h"

#define DT 1.0/60.0

using Eigen::Vector2d;

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
  Ball ball(Vector2d(constants::WINDOW_WIDTH/2, constants::WINDOW_HEIGHT/2),
            constants::BALL_RADIUS);

  Paddle left_paddle(true);
  Paddle right_paddle(false);

  size_t left_score = 0;
  size_t right_score = 0;

  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Pong");

//  window.setFramerateLimit(60);
  sf::Clock deltaClock;


  // ------ SET up text ------
  sf::Font font;
  // NOTE: Copy this fonts directory next to your executable
  font.loadFromFile("/home/josh/programming/cpp/pong/fonts/Hack-Bold.ttf");

  sf::Text left_score_text;
  left_score_text.setFont(font);
  left_score_text.setString("0");
  left_score_text.setPosition(sf::Vector2f(static_cast<float>
                                            (constants::WINDOW_WIDTH)/4.0,
                                           50.0));
  left_score_text.setFillColor(sf::Color::White);
  left_score_text.setCharacterSize(36);

  sf::Text right_score_text;
  right_score_text.setFont(font);
  right_score_text.setString("0");
  right_score_text.setPosition(sf::Vector2f(3 * static_cast<float>
                                            (constants::WINDOW_WIDTH)/4.0,
                                            50.0));
  right_score_text.setFillColor(sf::Color::White);
  right_score_text.setCharacterSize(36);

  sf::Text ball_speed_text;
  ball_speed_text.setFont(font);
  ball_speed_text.setString(std::to_string(static_cast<int>(constants::BALL_START_SPEED)));
  ball_speed_text.setPosition(sf::Vector2f(static_cast<float>(constants::WINDOW_WIDTH)/2 - 32, 10.0));
  ball_speed_text.setFillColor(sf::Color::Red);
  ball_speed_text.setCharacterSize(16);

  sf::Text ball_max_speed_text;
  ball_max_speed_text.setFont(font);
  ball_max_speed_text.setString("Max speed: " + std::to_string(static_cast<int>(constants::BALL_START_SPEED)));
  ball_max_speed_text.setPosition(10.0, 10.0);
  ball_max_speed_text.setFillColor(sf::Color::White);
  ball_max_speed_text.setCharacterSize(16);


  sf::Text fps_text;
  fps_text.setFont(font);
  fps_text.setString("0");
  fps_text.setPosition(constants::WINDOW_WIDTH - 60.0, 10.0);
  fps_text.setFillColor(sf::Color::Green);
  fps_text.setCharacterSize(12);

  // -------------------------

  // ------ Set up sprites ------

  sf::CircleShape ball_shape(constants::BALL_RADIUS);
  ball_shape.setFillColor(sf::Color::White);
  ball_shape.setOrigin(constants::BALL_RADIUS,
                       constants::BALL_RADIUS);

  sf::RectangleShape paddle_shape(sf::Vector2f(constants::PADDLE_WIDTH,
                                               constants::PADDLE_HEIGHT));
  paddle_shape.setFillColor(sf::Color::White);
  paddle_shape.setOrigin(constants::PADDLE_WIDTH/2.0,
                         constants::PADDLE_HEIGHT/2.0);

  // ----------------------------

  bool lefts_turn = false;   // left's turn to shoot?
  Paddle* target_paddle = &right_paddle;
  Bot bot;
  bot.change_target(ball, lefts_turn);

  double ball_max_speed = ball.get_speed();

  auto reset = [&]() {
    left_paddle.reset();
    right_paddle.reset();
    ball.reset();
    ball_speed_text.setString(std::to_string(static_cast<int>(constants::BALL_START_SPEED)));
    lefts_turn = false;
    target_paddle = &right_paddle;
    bot.change_target(ball, lefts_turn);
  };

  auto update_speed_text = [&]() {
    ball_speed_text.setString(std::to_string(static_cast<int>(ball.get_speed())));
    if (ball.get_speed() > ball_max_speed) {
      ball_max_speed = ball.get_speed();
      ball_max_speed_text.setString("Max speed: " + std::to_string(static_cast<int>(ball_max_speed)));
    }
  };

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    sf::Time dt_Time = deltaClock.restart();
    const double dt = static_cast<double>(dt_Time.asSeconds());

    // update BOT
    if (constants::BOTS > 0)
      bot.update(dt, target_paddle);

    // Process inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      right_paddle.move(dt, false);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      right_paddle.move(dt, true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      left_paddle.move(dt, false);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      left_paddle.move(dt, true);
    }

    // update
    ball.update(dt);

    if (check_goal(ball, true)) { // check left
      std::cout << "GOAL! ball_pos_y=" << ball.get_y() << std::endl;
      ++right_score;
      right_score_text.setString(std::to_string(right_score));
      reset();
    } else if (check_goal(ball, false)) {
      std::cout << "GOAL! ball_pos_y=" << ball.get_y() << std::endl;
      ++left_score;
      left_score_text.setString(std::to_string(left_score));
      reset();
    }

    // debug with mouse
//    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
//    ball.update_mouse(static_cast<double>(localPosition.x),
//                      static_cast<double>(localPosition.y));

    if (lefts_turn && left_paddle.check_collision(ball)) {
      ball.collide(left_paddle, constants::BOTS == 2);
      increase_speed(ball, left_paddle, right_paddle);
      lefts_turn = false;

      update_speed_text();

      if (constants::BOTS > 0) {
        target_paddle = &right_paddle;
      }
      bot.change_target(ball, lefts_turn);
    } else if (!lefts_turn && right_paddle.check_collision(ball)) {
      ball.collide(right_paddle, true);
      increase_speed(ball, left_paddle, right_paddle);
      lefts_turn = true;

      update_speed_text();

      if (constants::BOTS == 2) {
        target_paddle = &left_paddle;
        bot.change_target(ball, lefts_turn);
      }
    }

    // Draw
    window.clear(sf::Color::Black);
    // draw text
    window.draw(left_score_text);
    window.draw(right_score_text);
    window.draw(ball_speed_text);
    window.draw(ball_max_speed_text);

    // fps text
    fps_text.setString(std::to_string(1.0/dt));
    window.draw(fps_text);

    // draw ball
    ball_shape.setPosition(ball.get_x(), ball.get_y());
    window.draw(ball_shape);

    // draw paddles
    paddle_shape.setPosition(left_paddle.get_x(), left_paddle.get_y());
    window.draw(paddle_shape);
    paddle_shape.setPosition(right_paddle.get_x(),
                             right_paddle.get_y());
    window.draw(paddle_shape);

    window.display();
  }

  return 0;
}

