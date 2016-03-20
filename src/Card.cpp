#include "Card.hpp"

bool Card::operator ==(const Card& card) const {
  return (card.color == this->color) && (card.figure == this->figure);
}

bool Card::operator !=(const Card& card) const {
  return !(*this == card);
}
