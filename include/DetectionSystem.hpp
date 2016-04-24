#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"
#include "ImageAnalyzer.hpp"

class DetectionSystem :public TableSubject, public PrintScreenMaker, ImageAnalyzer {
  const std::string imagePath;
 public:
  DetectionSystem(std::string const& imageFilePath) :imagePath(imageFilePath) {}
  void run();
};
