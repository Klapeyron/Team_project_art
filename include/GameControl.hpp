#pragma once

#include "IGameControl.hpp"
#include "Position.hpp"
#include "ButtonsSnapshot.hpp"
#include "TableObserver.hpp"
#include <unistd.h>
#include <iostream>
#include "CardOperators.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include <algorithm>

class GameControl :public IGameControl, public TableObserver
{
private:
  void setPosition(const Position & position);
  void mouseClick(int button);
  TableSnapshot tableSnapshot;
  Position getPositionOfCard(const Card & card);
  std::string windowId;
public:
  GameControl();
  void onUpdate(TableSnapshot const& tableSnapshot);
  void pickCardFromHiddenStack();
  void pickCardFromStack();
  void throwMyCard(const Card &);
  void endGame();
  void touchCard(const Card &);
  void pressOK();  
  void pressPass();
  void pressKnock();
  void pressOkDisabled();
  void pressPassDisabled();
  void pressKnockDisabled();
  void getWindowId();
};
