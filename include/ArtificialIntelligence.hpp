#pragma once

#include "IGameControl.hpp"
#include "TableObserver.hpp"

class ArtificialIntelligence :public TableObserver {
  IGameControl & gameControl;
 public:
  ArtificialIntelligence(IGameControl & gameControlInterface) :gameControl(gameControlInterface) {}
  void onUpdate(TableSnapshot const&);
};
