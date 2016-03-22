#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"

#include <string>

class DetectionSystem :public TableSubject, public PrintScreenMaker {
  const std::string imagePath;
 public:
  DetectionSystem(std::string const& imageFilePath) :imagePath(imageFilePath) {}
  void run();
};
