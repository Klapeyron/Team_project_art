#include "gtest/gtest.h"

#include "DetectionSystem.hpp"
#include "TableObserverMock.hpp"
#include "SnapshotMatcher.hpp"

TEST (DetectionSystemTest, heartPlayerCardsDetectionTest)
{
  const std::string tableSnapshotFilePath = "../test/DetectionSystemImages/tableSnapshot.tiff";
  DetectionSystem detectionSystem (tableSnapshotFilePath);
  TableObserverMock tableObserver;
  detectionSystem.registerOberver(tableObserver);
 
  TableSnapshot output;
  output.playerCards.emplace_back(Card_Figure::KING, Card_Color::HEART);
  output.playerCards.emplace_back(Card_Figure::FOUR, Card_Color::HEART);
  
  EXPECT_CALL(tableObserver, onUpdate(ContainsThisSnapshot(output)));
  
  detectionSystem.run();
}
