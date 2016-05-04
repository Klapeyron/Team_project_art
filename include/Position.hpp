#pragma once

class Position
{
  unsigned x,y;
 public:
  constexpr explicit Position(unsigned posX = 0, unsigned posY = 0) :x(posX), y(posY) {}
  void setNewPosition(unsigned newX, unsigned newY);
  unsigned getX() const;
  unsigned getY() const;
  bool operator ==(const Position& elem) const { return this->x == elem.x and this->y == elem.y; };
  bool operator !=(const Position& elem) const { return !(*this == elem); };
};
