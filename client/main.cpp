#include <iostream>
#include <string>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>

#include <Eigen/Dense>

#include "constants.h"
#include "msg_type.h"

#define DT 1.0/60.0

#define SERVER_IP "localhost"

using Eigen::Vector2d;

MessageType get_msg_type(sf::TcpSocket& socket) {
  size_t recv;
  int msg_type;
  socket.receive(&msg_type, sizeof(int), recv);
  if (recv != sizeof(int)) {
    throw std::runtime_error("Error: Wrong num of bytes recieved.");
  }
  return static_cast<MessageType>(msg_type);
}

int main() {
  size_t left_score = 0;
  size_t right_score = 0;

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


  // Connect to server
  sf::TcpSocket socket;
  socket.setBlocking(true);
  sf::Socket::Status status = socket.connect(SERVER_IP, 53000);
  if (status != sf::Socket::Done) {
    throw std::runtime_error("ERROR: Could not connect to server...");
  }

  std::cout << "Connected :) : " << socket.getRemoteAddress() << ":"
            << socket.getRemotePort() << std::endl;

  // [ball.x, ball.y, enemy.y, left score incr, right score incr]
#define GAME_DATA_SIZE 5
  constexpr size_t expected_size = sizeof(double) * GAME_DATA_SIZE;
  double game_data[GAME_DATA_SIZE] = {0, 0, 0, 0, 0};

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
//      right_paddle.move(dt, false);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
//      right_paddle.move(dt, true);
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
//      left_paddle.move(dt, false);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
//      left_paddle.move(dt, true);
    }

    // update


//    if (check_goal(ball, true)) { // check left
//      ++right_score;
//      right_score_text.setString(std::to_string(right_score));
//      reset();
//    } else if (check_goal(ball, false)) {
//      ++left_score;
//      left_score_text.setString(std::to_string(left_score));
//      reset();
//    }

    // debug with mouse
//    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
//    ball.update_mouse(static_cast<double>(localPosition.x),
//                      static_cast<double>(localPosition.y));

//    if (left_paddle.check_collision(ball)) {
//      ball.collide(left_paddle);
//    } else if (right_paddle.check_collision(ball)) {
//      ball.collide(right_paddle);
//    }

    // Get positions from server
    MessageType msg_type = get_msg_type(socket);

    if (msg_type == MessageType::GameData) {
      std::cout << "Getting game data..." << std::endl;

      // Collect game data
      size_t recv;
      socket.receive(&game_data, expected_size, recv);
      if (recv != expected_size) {
        throw std::runtime_error("Error: Failed to collect game data.");
      }
      std::cout << game_data[0] << ", " << game_data[1] << ", "
                << game_data[2] << ", " << game_data[3] << ", "
                << game_data[4] << std::endl;
    }

    // Draw
    window.clear(sf::Color::Black);
    // draw score
    window.draw(left_score_text);
    window.draw(right_score_text);

    // draw ball
    ball_shape.setPosition(game_data[0], game_data[1]);
    window.draw(ball_shape);

    // draw paddles
    paddle_shape.setPosition(10.0, game_data[2]);
    window.draw(paddle_shape);
//    paddle_shape.setPosition(right_paddle.get_x(),
//                             right_paddle.get_y());
//    window.draw(paddle_shape);

    window.display();
  }

  return 0;
}

