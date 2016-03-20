#include "gtest/gtest.h"

#include "DetectionSystem.hpp"
#include "TableObserverMock.hpp"
#include "SnapshotMatcher.hpp"

TEST (DetectionSystemTest, sampleDetectionTest)
{
  DetectionSystem detectionSystem;
  TableObserverMock tableObserver;
  detectionSystem.registerOberver(tableObserver);

  std::string tableSnapshotFilePath = "../test/tableSnapshot.tiff";
 
  TableSnapshot output;
  EXPECT_CALL(tableObserver, onUpdate(ContainsThisSnapshot(output)));
  
  detectionSystem.processSnapshot(tableSnapshotFilePath);
}
