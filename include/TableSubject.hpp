#pragma once

#include "TableObserver.hpp"
#include <vector>

class TableSubject {
  std::vector<TableObserver*> observers;
 public:
  void registerOberver(TableObserver& );
  void notify(TableSnapshot const&);
};
