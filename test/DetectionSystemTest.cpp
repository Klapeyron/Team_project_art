#include "gtest/gtest.h"

#include "DetectionSystem.hpp"
#include "TableObserverMock.hpp"
#include "SnapshotMatcher.hpp"

TEST (DetectionSystemTest, sampleDetectionTest)
{
  const std::string tableSnapshotFilePath = "../test/tableSnapshot.tiff";
  DetectionSystem detectionSystem (tableSnapshotFilePath);
  TableObserverMock tableObserver;
  detectionSystem.registerOberver(tableObserver);
 
  TableSnapshot output;
  EXPECT_CALL(tableObserver, onUpdate(ContainsThisSnapshot(output)));
  
  detectionSystem.run();
}
