#pragma once

#include "Card.hpp"
#include "IGameControl.hpp"
#include "Position.hpp"
#include "ButtonsSnapshot.hpp"

class GameControl :public IGameControl 
{
private:
  void makeMove(const Position & position);
public:
  void pickCardFromHiddenStack();
  void pickCardFromStack();
  void throwMyCard(Card const&);
  void endGame();
  void touchCard(Card const&);
  void pressOK();
  
};
