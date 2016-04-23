#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"
#include "TableAnalyzer.hpp"

class DetectionSystem :public TableSubject, public PrintScreenMaker, TableAnalyzer {
  const std::string imagePath;
 public:
  DetectionSystem(std::string const& imageFilePath) :imagePath(imageFilePath) {}
  void run();
  ~DetectionSystem();
};
