
#pragma once

#include "gmock/gmock.h"
#include "TableObserver.hpp"

class TableObserverMock :public TableObserver
{
 public:
  MOCK_METHOD0(pickCardFromStack,void());
  MOCK_METHOD1(onUpdate,void(TableSnapshot const&));
};

inline std::ostream& operator<<(::std::ostream& os, const TableSnapshot& snapshot) {
  os << "Player cards: ";
  for(auto const& playerCard : snapshot.playerCards)
    os << playerCard << " | ";
  return os << "stackCard: " << snapshot.stackCard << " | " << "myMove: " << snapshot.myMove;
}
