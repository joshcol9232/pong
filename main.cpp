#include <iostream>

#include <SFML/Graphics.hpp>

#include <Eigen/Dense>

#include "ball.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define BALL_RADIUS 5.0d

using Eigen::Vector2d;


int main() {
  Ball ball(Vector2d(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), BALL_RADIUS, 1.0);

  sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Pong");

  // setup
  sf::CircleShape ball_shape(BALL_RADIUS);
  ball_shape.setFillColor(sf::Color::White);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();
    }

    ball.update(1.0/60.0);

    window.clear(sf::Color::Black);
    ball_shape.setPosition(ball.get_pos().x(), ball.get_pos().y());
    window.draw(ball_shape);

    window.display();
  }

  return 0;
}

