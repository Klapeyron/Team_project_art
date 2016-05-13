#pragma once



#include "IGameControl.hpp"
#include "Position.hpp"
#include "ButtonsSnapshot.hpp"
#include "TableObserver.hpp"
#include <unistd.h>
#include <iostream>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "CardOperators.hpp"
#include <iostream>
#include <cstring>

class GameControl :public IGameControl, public TableObserver
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
  void pressPass();
  void pressKnock();
};
