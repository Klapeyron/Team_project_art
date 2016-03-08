#pragma once

#include "TableSnapshot.hpp"

class TableObserver {
 public:
  virtual void onUpdate(TableSnapshot const&) = 0;
};
