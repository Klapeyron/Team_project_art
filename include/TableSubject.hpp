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

  bool canIWork;

  std::thread thread;
  void notifyAllObservers(TableSnapshot const& snapshot);
  void processInBackground();
 public:
  TableSubject();
  ~TableSubject();

  void registerOberver(TableObserver &);
  void notify(TableSnapshot const&);
};
