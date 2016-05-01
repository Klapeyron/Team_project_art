#pragma once

#include "Card.hpp"
#include "IGameControl.hpp"
#include "Position.hpp"
#include "ButtonsSnapshot.hpp"
#include "TableObserver.hpp"
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
  TableSnapshot tableSnapshot;
public:
  void onUpdate(TableSnapshot const& tableSnapshot);
  void pickCardFromHiddenStack();
  void pickCardFromStack();
  void throwMyCard(const Card &);
  void endGame();
  void touchCard(const Card &);
  void pressOK();  
};
