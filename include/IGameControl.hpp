#pragma once

#include "Card.hpp"

class IGameControl {
 public:
  virtual void pickCardFromHiddenStack() = 0;
  virtual void pickCardFromStack() = 0;
  virtual void throwMyCard(Card const&) = 0;
  virtual void endGame() = 0;
  virtual void touchCard(Card const&) = 0;
  virtual void pressOK() = 0;
  virtual void pressPass() = 0;
  virtual void pressKnock() = 0;
};
