#include <iostream>
#include <stdexcept>

#include <SFML/System.hpp>
#include <SFML/Network.hpp>
#include <Eigen/Dense>

#include "ball.h"
#include "paddle.h"
#include "constants.h"
#include "msg_type.h"

#define DT 1.0/60.0

using Eigen::Vector2d;

namespace {

inline bool check_goal(const Ball& b, bool check_left) {
  return (check_left && b.get_x() < 0.0) ||
      (!check_left && b.get_x() > constants::WINDOW_WIDTH);
}

void get_non_blocking_client(sf::TcpSocket& client, sf::TcpListener& listener) {
  client.setBlocking(false);
  const auto status = listener.accept(client);
  if (status != sf::Socket::Done) {
    std::cerr << "Err: " << status << std::endl;
    throw std::runtime_error("ERROR: Failed to connect to client...");
  }
  std::cout << "Player connected: " << client.getRemoteAddress()
            << std::endl;
}

void send_message_type(sf::TcpSocket& client, MessageType m) {
  size_t sent;
  client.send(&m, sizeof(int), sent);
  if (sent != sizeof(int)) {
    throw std::runtime_error("Error: Could not send message type.");
  }
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

  sf::TcpListener listener;
  if (listener.listen(53000) != sf::Socket::Done) {
    throw std::runtime_error("ERROR: Failed to bind to socket 53000...");
  }

  sf::TcpSocket left_client, right_client;
  get_non_blocking_client(left_client, listener);
  get_non_blocking_client(right_client, listener);

  // client setup
  send_message_type(left_client, MessageType::YouAreLeft);
  send_message_type(right_client, MessageType::YouAreRight);

  sf::Clock deltaClock;
  bool stop = false;

  std::cout << "Starting game..." << std::endl;

  while (!stop) {
    sf::Time dt_Time = deltaClock.restart();
    const double dt = static_cast<double>(dt_Time.asSeconds());

    /*
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
    */

    // update
    std::cout << "Updating ball: " << dt << std::endl;
    ball.update(dt);
    std::cout << "Ball pos: " << ball.get_x()
              << ", " << ball.get_y() << std::endl;

    double left_score_incr = 0.0;
    double right_score_incr = 0.0;
    if (check_goal(ball, true)) { // check left
      ++right_score;
      right_score_incr = 1.0;
      // TODO: Send score
      reset();
    } else if (check_goal(ball, false)) {
      ++left_score;
      left_score_incr = 1.0;
      // TODO: Send score
      reset();
    }

    if (left_paddle.check_collision(ball)) {
      ball.collide(left_paddle);
    } else if (right_paddle.check_collision(ball)) {
      ball.collide(right_paddle);
    }

    double left_data[5] = { ball.get_x(), ball.get_y(), right_paddle.get_y(), left_score_incr, right_score_incr };

    send_message_type(left_client, MessageType::GameData);
    size_t sent;

    left_client.send(&left_data, sizeof(double) * 5, sent);
    if (sent != sizeof(double) * 5) {
      throw std::runtime_error("Error: Could not send game data.");
    }

    double right_data[5] {
      constants::WINDOW_WIDTH - ball.get_x(),  // flip
      ball.get_y(),
      left_paddle.get_y(),
      left_score_incr,
      right_score_incr
    };

    send_message_type(right_client, MessageType::GameData);
    right_client.send(&right_data, sizeof(double) * 5, sent);
    if (sent != sizeof(double) * 5) {
      throw std::runtime_error("Error: Could not send game data.");
    }


    sf::sleep(sf::milliseconds(1));
  }

  return 0;
}

