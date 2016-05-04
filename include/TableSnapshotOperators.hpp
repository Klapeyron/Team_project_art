#pragma once

#include "TableSnapshot.hpp"
#include "CardOperators.hpp"

inline std::ostream& operator<<(::std::ostream& os, const TableSnapshot& snapshot) {
  os << "Player cards: ";
  for(auto const& playerCard : snapshot.playerCards)
    os << playerCard << " | ";
  return os << "stackCard: " << snapshot.stackCard << " | " << "myMove: " << snapshot.myMove;
}
