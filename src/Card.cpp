#include "../include/Card.hpp"

const Card_Color Card::getColor() { return this->color; }
const Card_Figure Card::getFigure() { return this->figure; }

bool Card::operator ==(const Card& card) {
  return (card.color == this->color) && (card.figure == this->figure);
}

bool Card::operator !=(const Card& card) {
  return !(*this == card);
}
