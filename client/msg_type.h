#ifndef MSG_TYPE_H
#define MSG_TYPE_H

enum class MessageType : int {
  GameData = 0,
  MoveUp = 1,
  MoveDown = 2,
  YouAreLeft = 3,
  YouAreRight = 4
};

#endif // MSG_TYPE_H
