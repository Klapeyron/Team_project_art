#include "gtest/gtest.h"

#include "TableSnapshot.hpp"

TEST (ArtificialIntelligenceTest, thisIsSampleTest )
{
  TableSnapshot tableSnapshot;
  tableSnapshot.playerCards.push_back( Card(Card_Figure::SEVEN, Card_Color::SPADE) );
  tableSnapshot.playerCards.push_back( Card(Card_Figure::SEVEN, Card_Color::DIAMOND) );
  tableSnapshot.playerCards.push_back( Card(Card_Figure::SEVEN, Card_Color::HEART) );
  tableSnapshot.stackCard = Card(Card_Figure::SEVEN, Card_Color::CLUB);

  EXPECT_EQ(1,1);
}
