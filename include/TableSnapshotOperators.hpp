#pragma once

#include "TableSnapshot.hpp"
#include "CardOperators.hpp"

inline std::ostream& operator<<(::std::ostream& os, const TableSnapshot& snapshot) {
  os << "Player cards: ";
  for(auto const& playerCard : snapshot.playerCards)
    os << playerCard << " | ";
  os << "stackCard: " << snapshot.stackCard << " | " << "myMove: " << snapshot.myMove << std::endl;
  os << "enemyEndsGame: " << snapshot.enemyEndsGame << " | " << "enemyTookCard: " << snapshot.enemyTookCard;
  return os;
}
