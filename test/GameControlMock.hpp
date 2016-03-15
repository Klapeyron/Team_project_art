#include "gmock/gmock.h"

#include "IGameControl.hpp"
#include "Card.hpp"

class GameControlMock :public IGameControl {
 public:
  MOCK_METHOD0(pickCardFromHiddenStack,void());
  MOCK_METHOD0(pickCardFromStack,void());
  MOCK_METHOD1(throwMyCard,void(Card const&));
  MOCK_METHOD0(endGame,void());
  MOCK_METHOD1(touchCard,void(Card const&));
  MOCK_METHOD0(pressOK,void());
};
