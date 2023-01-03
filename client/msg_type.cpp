#include "msg_type.h"

#include <stdexcept>
#include <iostream>
#include <SFML/Network.hpp>

MessageType recv_msg_type(sf::TcpSocket& socket) {
  size_t recv;
  int msg_type;
  socket.receive(&msg_type, sizeof(int), recv);
  if (recv == 0) {
    std::cout << "Got 0 bytes in recv_msg_type." << std::endl;
    return MessageType::DoNothing;
  } else if (recv != sizeof(int)) {
    std::cerr << "Recieved: " << recv << std::endl;
    throw std::runtime_error("Error: Wrong num of bytes recieved (MessageType).");
  }
  std::cout << "RECEIVING: " << msg_type << std::endl;
  return static_cast<MessageType>(msg_type);
}

void send_msg_type(sf::TcpSocket& socket, MessageType m) {
  size_t sent;
  socket.send(&m, sizeof(int), sent);
  if (sent != sizeof(int)) {
    std::cerr << "Sent: " << sent << std::endl;
    throw std::runtime_error("Error: Could not send message type (MessageType).");
  }
  std::cout << "SENT MESSAGE: " << static_cast<int>(m) << std::endl;
}
