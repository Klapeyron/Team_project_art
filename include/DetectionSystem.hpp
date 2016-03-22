#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"

#include <string>

class DetectionSystem :public TableSubject, public PrintScreenMaker {
 public:
  void processSnapshot(std::string const& imageFilePath);
};
