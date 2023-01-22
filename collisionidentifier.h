#ifndef IDENTIFIER_H
#define IDENTIFIER_H

enum class CollisionIdentifier : short int {
  None         = -1,
  LeftPaddle   = 0,
  RightPaddle  = 1,
  Ball         = 2,
  TopWall      = 3,
  BottomWall   = 4
};

#endif // IDENTIFIER_H
