#pragma once

#include "Card.hpp"
#include <vector>

struct TableSnapshot {
  std::vector<Card> playerCards;
  Card stackCard;
  bool myMove = true;
};
