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

TEST (ArtificialIntelligenceTest, cardIsNotInFigureSequence_shouldntBeTaken)
{
  TableSnapshot tableSnapshot;

  tableSnapshot.playerCards = {
    Card(Card_Figure::JACK, Card_Color::HEART),
    Card(Card_Figure::QUEEN, Card_Color::HEART),
    Card(Card_Figure::KING, Card_Color::HEART)
  };

  tableSnapshot.stackCard = Card(Card_Figure::A, Card_Color::HEART);

  GameControlMock gameControl;
  ArtificialIntelligence artificialIntelligence(gameControl);

  EXPECT_CALL(gameControl,pickCardFromStack())
      .Times(0);
  EXPECT_CALL(gameControl,pickCardFromHiddenStack())
      .Times(1);
  EXPECT_CALL(gameControl,throwMyCard(testing::_))
      .Times(0);

  artificialIntelligence.onUpdate(tableSnapshot);
}

TEST (ArtificialIntelligenceTest, cardIsInFigureSequence_shouldBeTaken)
{
  TableSnapshot tableSnapshot;

  tableSnapshot.playerCards = {
    Card(Card_Figure::JACK, Card_Color::HEART),
    Card(Card_Figure::QUEEN, Card_Color::HEART),
    Card(Card_Figure::KING, Card_Color::HEART)
  };

  tableSnapshot.stackCard = Card(Card_Figure::TEN, Card_Color::HEART);

  GameControlMock gameControl;
  ArtificialIntelligence artificialIntelligence(gameControl);

  EXPECT_CALL(gameControl,pickCardFromStack())
      .Times(1);
  EXPECT_CALL(gameControl,pickCardFromHiddenStack())
      .Times(0);
  EXPECT_CALL(gameControl,throwMyCard(testing::_))
      .Times(0);

  artificialIntelligence.onUpdate(tableSnapshot);
}

TEST (ArtificialIntelligenceTest,PlayerHaveSequence_EndGame_ByPickingCardFromStack_ThrowBrushCard)
{
  TableSnapshot tableSnapshot;

  tableSnapshot.playerCards = {
    Card(Card_Figure::SEVEN, Card_Color::SPADE),
    Card(Card_Figure::SEVEN, Card_Color::DIAMOND),
    Card(Card_Figure::SEVEN, Card_Color::HEART),
    Card(Card_Figure::NINE, Card_Color::SPADE),
    Card(Card_Figure::NINE, Card_Color::DIAMOND),
    Card(Card_Figure::NINE, Card_Color::HEART),
    Card(Card_Figure::A, Card_Color::SPADE),
    Card(Card_Figure::TWO, Card_Color::SPADE),
    Card(Card_Figure::THREE, Card_Color::SPADE),
    Card(Card_Figure::FOUR, Card_Color::HEART)
  };

  tableSnapshot.stackCard = Card(Card_Figure::FOUR, Card_Color::SPADE);

  GameControlMock gameControl;
  ArtificialIntelligence artificialIntelligence(gameControl);

  EXPECT_CALL(gameControl,pickCardFromStack())
      .Times(1);
  EXPECT_CALL(gameControl, pickCardFromHiddenStack() )
      .Times(0);
  EXPECT_CALL(gameControl,touchCard( Card(Card_Figure::FOUR, Card_Color::HEART)))
      .Times(1);
  EXPECT_CALL(gameControl,endGame())
      .Times(1);

  artificialIntelligence.onUpdate(tableSnapshot);
}
