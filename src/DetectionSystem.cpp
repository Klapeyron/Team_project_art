#include "DetectionSystem.hpp"

void DetectionSystem::run()
{
  TableSnapshot tableSnapshot;
  
  TableSubject::notify(tableSnapshot);
  TableSubject::waitForUnfinishedJobs();
}
