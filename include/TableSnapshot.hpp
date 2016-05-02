#pragma once

#include "Card.hpp"
#include <vector>

struct TableSnapshot {
  std::vector<Card> playerCards;
  Card stackCard;
  bool myMove = true;
  bool opponentTookCardFromHiddenStack = false;
  bool operator ==(const TableSnapshot& card) const;
  bool operator !=(const TableSnapshot& card) const;
};

inline bool TableSnapshot::operator ==(const TableSnapshot& snapshot) const {
  return this->stackCard == snapshot.stackCard and
      this->myMove == snapshot.myMove and
      this->opponentTookCardFromHiddenStack == snapshot.opponentTookCardFromHiddenStack and
      this->playerCards == snapshot.playerCards;
}

inline bool TableSnapshot::operator !=(const TableSnapshot& snapshot) const {
  return !(*this == snapshot);
}

