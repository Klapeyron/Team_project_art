#pragma once

class Position
{
  unsigned x,y;
 public:
  constexpr explicit Position(unsigned posX = 0, unsigned posY = 0) :x(posX), y(posY) {}
  void setNewPosition(unsigned newX, unsigned newY);
  void setNewPosition(Position position);
  unsigned getX() const;
  unsigned getY() const;
  Position getPosition() const;
  bool operator ==(const Position& elem) const { return this->x == elem.x and this->y == elem.y; };
  bool operator !=(const Position& elem) const { return !(*this == elem); };
  Position operator +(const Position& elem) const { return Position(this->x + elem.getX(), this->y + elem.getY()); };
};
