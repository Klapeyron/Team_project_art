#pragma once

class Position
{
  int x,y;
 public:
  constexpr explicit Position(int posX = 0, int posY = 0) :x(posX), y(posY) {}
  void setNewPosition(int newX, int newY);
  int getX();
  int getY();
};
