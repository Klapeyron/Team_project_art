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
#include <chrono>
#include <thread>
class GameControl :public IGameControl, public TableObserver
{
private:
  
  TableSnapshot tableSnapshot;
  std::string windowId;
  void getWindowId();
  void setPosition(const Position & position);
  void mouseClick(int button);
  Position getPositionOfCard(const Card & card);
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

};
