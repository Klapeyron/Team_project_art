#include "gtest/gtest.h"

#include "DetectionSystem.hpp"
#include "TableObserverMock.hpp"
#include "SnapshotMatcher.hpp"

TEST (DetectionSystemTest, 1366x768DetectionTest)
{
  const std::string tableSnapshotFilePath = "../test/DetectionSystemImages/tableSnapshot1366x768.tiff";
  DetectionSystem detectionSystem (tableSnapshotFilePath);
  
  TableObserverMock tableObserver;
  detectionSystem.registerOberver(tableObserver);
 
  TableSnapshot output;
  output.playerCards.emplace_back(Card_Figure::FOUR, Card_Color::HEART);
  output.playerCards.emplace_back(Card_Figure::KING, Card_Color::HEART);

  output.playerCards.emplace_back(Card_Figure::FOUR, Card_Color::SPADE);
  output.playerCards.emplace_back(Card_Figure::SIX, Card_Color::SPADE);
  output.playerCards.emplace_back(Card_Figure::SEVEN, Card_Color::SPADE);
  output.playerCards.emplace_back(Card_Figure::TEN, Card_Color::SPADE);

  output.playerCards.emplace_back(Card_Figure::FIVE, Card_Color::DIAMOND);
  output.playerCards.emplace_back(Card_Figure::SIX, Card_Color::DIAMOND);

  output.playerCards.emplace_back(Card_Figure::SIX, Card_Color::CLUB);
  output.playerCards.emplace_back(Card_Figure::JACK, Card_Color::CLUB);

  output.stackCard = Card(Card_Figure::EIGHT, Card_Color::HEART);
  output.myMove = true;
  
  EXPECT_CALL(tableObserver, onUpdate(ContainsThisSnapshot(output)));
  detectionSystem.run();
}
