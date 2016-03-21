#include "gtest/gtest.h"

#include "ArtificialIntelligence.hpp"
#include "GameControlMock.hpp"
#include "TableSnapshot.hpp"

TEST (ArtificialIntelligenceTest, missingSequenceCardShouldBeTakenFromStack)
{
  TableSnapshot tableSnapshot;

  tableSnapshot.playerCards = {
    Card(Card_Figure::SEVEN, Card_Color::SPADE),
    Card(Card_Figure::SEVEN, Card_Color::DIAMOND),
    Card(Card_Figure::SEVEN, Card_Color::HEART)
  };

  tableSnapshot.stackCard = Card(Card_Figure::SEVEN, Card_Color::CLUB);
  
  GameControlMock gameControl;
  ArtificialIntelligence artificialIntelligence(gameControl);

  EXPECT_CALL(gameControl,pickCardFromStack())
      .Times(1);
  EXPECT_CALL(gameControl,throwMyCard(testing::_))
      .Times(0);

  artificialIntelligence.onUpdate(tableSnapshot);
}

TEST (ArtificialIntelligenceTest, cardOnStackIsNotMissingInSequence_cardFromHiddenStackShouldBeTaken)
{
  TableSnapshot tableSnapshot;

  tableSnapshot.playerCards = {
    Card(Card_Figure::SEVEN, Card_Color::SPADE),
    Card(Card_Figure::SEVEN, Card_Color::DIAMOND),
    Card(Card_Figure::SEVEN, Card_Color::HEART)
  };

  tableSnapshot.stackCard = Card(Card_Figure::TWO, Card_Color::SPADE);
  
  GameControlMock gameControl;
  ArtificialIntelligence artificialIntelligence(gameControl);

  EXPECT_CALL(gameControl,pickCardFromStack())
      .Times(0);
  EXPECT_CALL(gameControl,pickCardFromHiddenStack())
      .Times(1);

  artificialIntelligence.onUpdate(tableSnapshot);
}

TEST (ArtificialIntelligenceTest, cardOnStackIsNotMissingInSequence_cardFromHiddenStackShouldBeTaken)
{
  TableSnapshot tableSnapshot;

  tableSnapshot.playerCards = {
    Card(Card_Figure::SEVEN, Card_Color::SPADE),
    Card(Card_Figure::SEVEN, Card_Color::DIAMOND),
    Card(Card_Figure::SEVEN, Card_Color::HEART)
  };

  tableSnapshot.stackCard = Card(Card_Figure::TWO, Card_Color::SPADE);
  
  GameControlMock gameControl;
  ArtificialIntelligence artificialIntelligence(gameControl);

  EXPECT_CALL(gameControl,pickCardFromStack())
      .Times(0);
  EXPECT_CALL(gameControl,pickCardFromHiddenStack())
      .Times(1);

  artificialIntelligence.onUpdate(tableSnapshot);
}
