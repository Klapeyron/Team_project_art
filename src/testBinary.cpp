#include "DetectionSystem.hpp"
#include "TableSnapshotOperators.hpp"

#include <iostream>

class Detection :public TableObserver {
 public:
  void onUpdate(TableSnapshot const& snapshot)
  {
    std::cout << "This snapshot is detected, is it correct?" << std::endl;
    std::cout << snapshot << std::endl;
  }
};

int main()
{
  std::cout << "This binary test if your computer support detection system" << std::endl;
  DetectionSystem detectionSystem(printScreenFileName);

  Detection detection;
  detectionSystem.registerOberver(detection);

  std::cout << "Please run game window and press on it" << std::endl;
  detectionSystem.detectWindowByMouseClickOnIt();

  detectionSystem.makePrintScreen();
  detectionSystem.processTable();

  detectionSystem.waitForUnfinishedJobs();
}
