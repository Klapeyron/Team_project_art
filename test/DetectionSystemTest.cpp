#include "gtest/gtest.h"

#include "DetectionSystem.hpp"
#include "TableObserverMock.hpp"
#include "SnapshotMatcher.hpp"

using testing::_;
using testing::Invoke;

TEST (DetectionSystemTest, sampleDetectionTest)
{
  DetectionSystem detectionSystem;
  TableObserverMock tableObserver;
  detectionSystem.registerOberver(tableObserver);

  std::string tableSnapshotFilePath = "../test/tableSnapshot.tiff";
 
  TableSnapshot output;
  // EXPECT_CALL(tableObserver,onUpdate(testing::_)).Times(1).WillOnce(Invoke(findCard));
  EXPECT_CALL(tableObserver, onUpdate(ContainsThisSnapshot(output)));
  
  detectionSystem.processSnapshot(tableSnapshotFilePath);
}
