#pragma once

#include "Card.hpp"
#include "IGameControl.hpp"

class GameControl :public IGameControl {
 public:
  void pickCardFromHiddenStack();
  void pickCardFromStack();
  void throwMyCard(Card const&);
  void endGame();
  void touchCard(Card const&);
  void pressOK();
};
