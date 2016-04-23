#pragma once

#include "TableObserver.hpp"
#include <vector>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <deque>
#include <atomic>

class TableSubject {
  std::vector<TableObserver*> observers;
  std::deque<TableSnapshot> tableSnapshots;

  mutable std::mutex mutex;
  std::condition_variable cv;
  std::atomic<bool> canIWork;

  void notifyAllObservers(TableSnapshot const& snapshot);
 public:
  TableSubject();
  void processInBackground();
  void registerOberver(TableObserver &);
  void notify(TableSnapshot const&);
};
