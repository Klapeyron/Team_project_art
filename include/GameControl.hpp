#pragma once

#include "Card.hpp"
#include "IGameControl.hpp"
#include "Position.hpp"
#include "ButtonsSnapshot.hpp"
#include <unistd.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <string.h>
class GameControl :public IGameControl 
{
private:
  void setPosition(const Position & position);
  void mouseClick(int button);
public:
  void pickCardFromHiddenStack();
  void pickCardFromStack();
  void throwMyCard(const Card &);
  void endGame();
  void touchCard(const Card &);
  void pressOK();
  
};
