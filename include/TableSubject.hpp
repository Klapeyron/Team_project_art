#pragma once

#include "TableObserver.hpp"
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <deque>
#include <memory>

class TableSubject {
  std::vector<TableObserver*> observers;
  std::deque<TableSnapshot> tableSnapshots;

  mutable std::mutex mutex;
  std::condition_variable cv;

  mutable std::mutex canIWorkMutex;
  std::condition_variable canIWorkNofify;
  bool canIWork;

  std::thread thread;
  void notifyAllObservers(TableSnapshot const& snapshot);
  void processInBackground();
 public:
  TableSubject();
  ~TableSubject();

  void waitForUnfinishedJobs();
  void registerOberver(TableObserver &);
  void notify(TableSnapshot const&);
};
