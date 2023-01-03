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

  bool is_left;
  const MessageType m = recv_msg_type(socket);
  if (m == MessageType::YouAreLeft) {
    is_left = true;
  } else if (m == MessageType::YouAreRight) {
    is_left = false;
  } else {
    std::cerr << "Err: Got " << static_cast<int>(m) << " message type." << std::endl;
    throw std::runtime_error("Error: Expected a YouAreLeft "
                             "or YouAreRight message.");
  }

  // [ball.x, ball.y, enemy.y, left score incr, right score incr]
#define GAME_DATA_SIZE 6
  constexpr size_t expected_size = sizeof(double) * GAME_DATA_SIZE;
  double game_data[GAME_DATA_SIZE] = {0, 0, 0, 0, 0, 0};

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
      send_msg_type(socket, MessageType::MoveUp);
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
      send_msg_type(socket, MessageType::MoveDown);
    }

    // Get positions from server
    MessageType msg_type = recv_msg_type(socket);

    if (msg_type == MessageType::GameData) {
//      std::cout << "Getting game data..." << std::endl;

      // Collect game data
      size_t recv;
      socket.receive(&game_data, expected_size, recv);
      if (recv != expected_size) {
        throw std::runtime_error("Error: Failed to collect game data.");
      }
      /*
      std::cout << game_data[0] << ", " << game_data[1] << ", "
                << game_data[2] << ", " << game_data[3] << ", "
                << game_data[4] << ", " << game_data[5] << std::endl;
      */
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
    std::cout << "2: " << game_data[2] << std::endl;
    std::cout << "3: " << game_data[3] << std::endl;
    paddle_shape.setPosition(constants::PADDLE_OFFSET, game_data[2]);
    window.draw(paddle_shape);
    paddle_shape.setPosition(constants::WINDOW_WIDTH - constants::PADDLE_OFFSET, game_data[3]);
    window.draw(paddle_shape);


    window.display();
  }

  return 0;
}

