#include <iostream>
#include <string>

#include <SFML/Graphics.hpp>

#include <Eigen/Dense>

#include "ball.h"
#include "paddle.h"
#include "constants.h"

#define DT 1.0/60.0

using Eigen::Vector2d;

namespace {

inline bool check_goal(const Ball& b, bool check_left) {
  return (check_left && b.get_x() < 0.0) ||
      (!check_left && b.get_x() > constants::WINDOW_WIDTH);
}

}  // anonymous namespace

int main() {
  Ball ball(Vector2d(constants::WINDOW_WIDTH/2, constants::WINDOW_HEIGHT/2),
            constants::BALL_RADIUS);

  Paddle left_paddle(true);
  Paddle right_paddle(false);

  size_t left_score = 0;
  size_t right_score = 0;

  auto reset = [&]() {
    left_paddle.reset();
    right_paddle.reset();
    ball.reset();
  };

  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Pong");

//  window.setFramerateLimit(60);
  sf::Clock deltaClock;

  // setup
  sf::Font font;
  // NOTE: Copy this fonts directory next to your executable
  font.loadFromFile("./fonts/Hack-Bold.ttf");

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

  sf::CircleShape ball_shape(constants::BALL_RADIUS);
  ball_shape.setFillColor(sf::Color::White);
  ball_shape.setOrigin(constants::BALL_RADIUS,
                       constants::BALL_RADIUS);

  sf::RectangleShape paddle_shape(sf::Vector2f(constants::PADDLE_WIDTH,
                                               constants::PADDLE_HEIGHT));
  paddle_shape.setFillColor(sf::Color::White);
  paddle_shape.setOrigin(constants::PADDLE_WIDTH/2.0,
                         constants::PADDLE_HEIGHT/2.0);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        window.close();
      }
    }

    sf::Time dt_Time = deltaClock.restart();
    const double dt = static_cast<double>(dt_Time.asSeconds());

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
      ++right_score;
      right_score_text.setString(std::to_string(right_score));
      reset();
    } else if (check_goal(ball, false)) {
      ++left_score;
      left_score_text.setString(std::to_string(left_score));
      reset();
    }

    // debug with mouse
//    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
//    ball.update_mouse(static_cast<double>(localPosition.x),
//                      static_cast<double>(localPosition.y));

    if (left_paddle.check_collision(ball)) {
      ball.collide(left_paddle);
    } else if (right_paddle.check_collision(ball)) {
      ball.collide(right_paddle);
    }

    // Draw
    window.clear(sf::Color::Black);
    // draw score
    window.draw(left_score_text);
    window.draw(right_score_text);

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

