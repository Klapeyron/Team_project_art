#include "TableSubject.hpp"

#include <chrono>

TableSubject::TableSubject()
{
  auto process = [&]() { processInBackground(); };
  thread = std::thread(process);
}

TableSubject::~TableSubject()
{

}

bool TableSubject::allJobsFinished()
{
  thread.join();
  return tableSnapshots.empty();
}

void TableSubject::processInBackground()
{
     TableSnapshot snapshot;
      {
        std::unique_lock<std::mutex> locker(mutex);
        auto tableSnapshotsNotEmpty = [&]() { return not tableSnapshots.empty(); };
        cv.wait(locker, tableSnapshotsNotEmpty);
        snapshot = std::move(tableSnapshots.back());
        tableSnapshots.pop_back();
      }
      notifyAllObservers(snapshot);
}

void TableSubject::notifyAllObservers(TableSnapshot const& snapshot)
{
  for(auto const& observer : observers)
    observer->onUpdate(snapshot);
}

void TableSubject::notify(TableSnapshot const& snapshot) {
  std::unique_lock<std::mutex> locker(mutex);
  tableSnapshots.push_front( std::move(snapshot) );
  cv.notify_one();
}

void TableSubject::registerOberver(TableObserver& observer) {
  std::unique_lock<std::mutex> locker(mutex);
  observers.push_back(&observer);
}
