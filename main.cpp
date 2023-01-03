#include <iostream>

#include <SFML/Graphics.hpp>

#include <Eigen/Dense>

#include "ball.h"
#include "paddle.h"
#include "constants.h"

#define DT 1.0/60.0

using Eigen::Vector2d;

int main() {
  Ball ball(Vector2d(constants::WINDOW_WIDTH/2, constants::WINDOW_HEIGHT/2),
            constants::BALL_RADIUS);

  Paddle left_paddle(true);
  Paddle right_paddle(false);

  sf::RenderWindow window(sf::VideoMode(constants::WINDOW_WIDTH,
                                        constants::WINDOW_HEIGHT),
                          "Pong");

  // setup
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

    // Process inputs
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
      right_paddle.move(DT, false);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      right_paddle.move(DT, true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      left_paddle.move(DT, false);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      left_paddle.move(DT, true);
    }

    // update
    ball.update(DT);

    std::cout << "RIGHT: " << right_paddle.check_collision(ball) << std::endl;

    // Draw
    window.clear(sf::Color::Black);
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

