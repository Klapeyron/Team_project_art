#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"

#include <string>
#include <opencv/cv.hpp>

class DetectionSystem :public TableSubject, public PrintScreenMaker {
 public:
  void processSnapshot(std::string const& imageFilePath);
};
