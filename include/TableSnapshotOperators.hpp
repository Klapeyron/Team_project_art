#pragma once

#include "TableSnapshot.hpp"
#include "CardOperators.hpp"

inline std::ostream& operator<<(::std::ostream& os, const TableSnapshot& snapshot) {
  os << "Player cards: ";
  for(auto const& playerCard : snapshot.playerCards)
    os << playerCard << " " << playerCard.getX() << " " << playerCard.getY() << " | " << std::endl;
  os << "stackCard: " << snapshot.stackCard << " " << snapshot.stackCard.getX() << " " << snapshot.stackCard.getY() << " | " << std::endl;
  os << "myMove: " << snapshot.myMove << std::endl;
  os << "enemyEndsGame: " << snapshot.enemyEndsGame << " | " << "enemyTookCard: " << snapshot.enemyTookCard << std::endl;
  return os;
}
