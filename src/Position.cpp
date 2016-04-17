#include "Position.hpp"

void Position::setNewPosition(int newX, int newY)
{
  x = newX;
  y = newY;
}

int Position::getX()const
{
  return x;
}

int Position::getY()const
{
  return y;
}
