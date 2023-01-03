#ifndef MSG_TYPE_H
#define MSG_TYPE_H

#include <SFML/Network.hpp>

enum class MessageType : int {
  GameData = 0,
  MoveUp = 1,
  MoveDown = 2,
  YouAreLeft = 3,
  YouAreRight = 4,
  DoNothing = 5
};

MessageType recv_msg_type(sf::TcpSocket& socket);
void send_msg_type(sf::TcpSocket& socket, MessageType m);

#endif // MSG_TYPE_H
