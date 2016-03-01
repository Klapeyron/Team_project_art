#include "gtest/gtest.h"
#include "Card.hpp"

TEST (CardTest, defaultValueOfColorAndFigureShouldBeNone )
{
  Card card;
  EXPECT_EQ (card.getColor(), Card_Color::None);
  EXPECT_EQ (card.getFigure(), Card_Figure::None);
}

TEST (CardTest, cardsShouldBeSuccessfulCompared)
{
  Card card1 (Card_Figure::JACK,Card_Color::DIAMOND);
  Card card2 (Card_Figure::JACK,Card_Color::DIAMOND);
  EXPECT_TRUE (card1 == card2);
}

TEST (CardTest, colorsAndFiguresShouldHaveIntegerRepresentation)
{
  EXPECT_EQ (static_cast<int> (Card_Color::SPADE), 0);
  EXPECT_EQ (static_cast<int> (Card_Figure::A), 0);
  EXPECT_EQ (static_cast<int> (Card_Color::DIAMOND), 3);
  EXPECT_EQ (static_cast<int> (Card_Figure::KING), 12);
  EXPECT_EQ (static_cast<int> (Card_Color::None), 4);
  EXPECT_EQ (static_cast<int> (Card_Figure::None), 13);
}
