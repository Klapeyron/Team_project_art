#include "Position.hpp"

void Position::setNewPosition(unsigned newX, unsigned newY)
{
  x = newX;
  y = newY;
}

void Position::setNewPosition(Position position)
{
  x = position.getX();
  y = position.getY();
}

unsigned Position::getX() const
{
  return x;
}

unsigned Position::getY() const
{
  return y;
}

Position Position::getPosition() const
{
  return Position(x,y);
}
