#pragma once

#include "gmock/gmock.h"
#include "TableObserver.hpp"

class TableObserverMock :public TableObserver
{
 public:
  MOCK_METHOD0(pickCardFromStack,void());
  MOCK_METHOD1(onUpdate,void(TableSnapshot const&));
};
