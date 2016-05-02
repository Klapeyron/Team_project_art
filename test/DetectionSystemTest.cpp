#include "gtest/gtest.h"

#include "CardOperators.hpp"
#include "DetectionSystem.hpp"
#include "TableObserverMock.hpp"
#include "SnapshotMatcher.hpp"

TEST (DetectionSystemTest, 1366x768DetectionTest)
{
  const std::string tableSnapshotFilePath = "../test/DetectionSystemImages/tableSnapshot1366x768.png";
  DetectionSystem detectionSystem (tableSnapshotFilePath);
  
  TableObserverMock tableObserver;
  detectionSystem.registerOberver(tableObserver);
 
  TableSnapshot output;
  output.playerCards.emplace_back(Card_Figure::TWO, Card_Color::HEART);
  output.playerCards.emplace_back(Card_Figure::THREE, Card_Color::HEART);
  output.playerCards.emplace_back(Card_Figure::EIGHT, Card_Color::HEART);

  output.playerCards.emplace_back(Card_Figure::EIGHT, Card_Color::CLUB);
  output.playerCards.emplace_back(Card_Figure::JACK, Card_Color::CLUB);

  output.playerCards.emplace_back(Card_Figure::FIVE, Card_Color::DIAMOND);
  output.playerCards.emplace_back(Card_Figure::KING, Card_Color::DIAMOND);
  
  output.playerCards.emplace_back(Card_Figure::FIVE, Card_Color::SPADE);
  output.playerCards.emplace_back(Card_Figure::EIGHT, Card_Color::SPADE);
  output.playerCards.emplace_back(Card_Figure::QUEEN, Card_Color::SPADE);

  output.stackCard = Card(Card_Figure::JACK, Card_Color::SPADE);
  output.myMove = true;
  
  EXPECT_CALL(tableObserver, onUpdate(ContainsThisSnapshot(output)));
  detectionSystem.processTable();
}

TEST (DetectionSystemTest, 1680x1050DetectionTest)
{
  const std::string tableSnapshotFilePath = "../test/DetectionSystemImages/tableSnapshot1680x1050.png";
  DetectionSystem detectionSystem (tableSnapshotFilePath);
  
  TableObserverMock tableObserver;
  detectionSystem.registerOberver(tableObserver);
 
  TableSnapshot output;
  output.playerCards.emplace_back(Card_Figure::TWO, Card_Color::CLUB);
  output.playerCards.emplace_back(Card_Figure::THREE, Card_Color::CLUB);
  output.playerCards.emplace_back(Card_Figure::EIGHT, Card_Color::CLUB);
  output.playerCards.emplace_back(Card_Figure::NINE, Card_Color::CLUB);
  output.playerCards.emplace_back(Card_Figure::KING, Card_Color::CLUB);

  output.playerCards.emplace_back(Card_Figure::FIVE, Card_Color::DIAMOND);
  
  output.playerCards.emplace_back(Card_Figure::A, Card_Color::SPADE);
  output.playerCards.emplace_back(Card_Figure::SIX, Card_Color::SPADE);
  output.playerCards.emplace_back(Card_Figure::SEVEN, Card_Color::SPADE);
  
  output.stackCard = Card(Card_Figure::FOUR, Card_Color::DIAMOND);
  output.myMove = true;
  
  EXPECT_CALL(tableObserver, onUpdate(ContainsThisSnapshot(output)));
  detectionSystem.processTable();
}
