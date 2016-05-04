#include "TableSubject.hpp"
#include <iostream>
#include <chrono>

TableSubject::TableSubject() :canIWork{true}, thread{&TableSubject::processInBackground, this} {}

TableSubject::~TableSubject()
{
  {
    std::unique_lock<std::mutex> locker(mutex);
    canIWork = false;
    cv.notify_all();
  }

  if(thread.joinable()) thread.join();
}

void TableSubject::processInBackground()
{
  while(true)
  {
    TableSnapshot snapshot;
    {
      std::unique_lock<std::mutex> locker(mutex);
      auto tableSnapshotsNotEmpty = [&]() { return not canIWork or not tableSnapshots.empty(); };
      cv.wait(locker, tableSnapshotsNotEmpty);
      if(tableSnapshots.empty() and not canIWork)
        return;
      snapshot = std::move(tableSnapshots.back());
      tableSnapshots.pop_back();
    }
    notifyAllObservers(snapshot);
  }
}

void TableSubject::notifyAllObservers(TableSnapshot const& snapshot)
{
  for(auto const& observer : observers)
    observer->onUpdate(snapshot);
}

void TableSubject::notify(TableSnapshot const& snapshot) {
  std::unique_lock<std::mutex> locker(mutex);
  tableSnapshots.emplace_front( std::move(snapshot) );
  cv.notify_one();
}

void TableSubject::registerOberver(TableObserver & observer) {
  std::unique_lock<std::mutex> locker(mutex);
  observers.push_back(&observer);
}
