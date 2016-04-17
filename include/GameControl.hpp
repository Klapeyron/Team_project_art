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
  void throwMyCard(const Card &);
  void endGame();
  void touchCard(const Card &);
  void pressOK();
  
};
