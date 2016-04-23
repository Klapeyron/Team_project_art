#pragma once

#include "Card.hpp"
#include <vector>

struct TableSnapshot {
  std::vector<Card> playerCards;
  Card stackCard;
  bool myMove = true;
};

inline std::ostream& operator<<(::std::ostream& os, const TableSnapshot& snapshot) {
  os << "Player cards: ";
  for(auto const& playerCard : snapshot.playerCards)
    os << playerCard << " | ";
  return os << "stackCard: " << snapshot.stackCard << " | " << "myMove: " << snapshot.myMove;
}
