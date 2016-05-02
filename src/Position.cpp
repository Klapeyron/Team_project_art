#include "Position.hpp"

void Position::setNewPosition(unsigned newX, unsigned newY)
{
  x = newX;
  y = newY;
}

unsigned Position::getX() const
{
  return x;
}

unsigned Position::getY() const
{
  return y;
}
