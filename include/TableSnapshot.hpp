#pragma once

#include "Card.hpp"
#include "ButtonsSnapshot.hpp"
#include <vector>

struct TableSnapshot :public ButtonsSnapshot {
  std::vector<Card> playerCards;
  Card stackCard;
  bool myMove = true;
  bool enemyEndsGame = false;
  bool enemyTookCard = false;
  bool operator ==(const TableSnapshot&) const;
  bool operator !=(const TableSnapshot&) const;
};

inline bool TableSnapshot::operator ==(const TableSnapshot& snapshot) const {
  return this->stackCard == snapshot.stackCard and
      this->myMove == snapshot.myMove and
      this->enemyTookCard == snapshot.enemyTookCard and
      this->enemyEndsGame == snapshot.enemyEndsGame and
      this->playerCards == snapshot.playerCards;
}

inline bool TableSnapshot::operator !=(const TableSnapshot& snapshot) const {
  return !(*this == snapshot);
}
