#include "gtest/gtest.h"

#include "GameControlMock.hpp"
#include "TableSnapshot.hpp"

TEST (ArtificialIntelligenceTest, thisIsSampleTest )
{
  TableSnapshot tableSnapshot;
  GameControlMock gameControl;
  tableSnapshot.playerCards.push_back( Card(Card_Figure::SEVEN, Card_Color::SPADE) );
  tableSnapshot.playerCards.push_back( Card(Card_Figure::SEVEN, Card_Color::DIAMOND) );
  tableSnapshot.playerCards.push_back( Card(Card_Figure::SEVEN, Card_Color::HEART) );
  tableSnapshot.stackCard = Card(Card_Figure::SEVEN, Card_Color::CLUB);

  EXPECT_CALL(gameControl,pickCardFromStack())
      .Times(0);
  EXPECT_CALL(gameControl,throwMyCard(testing::_))
      .Times(0);
}
