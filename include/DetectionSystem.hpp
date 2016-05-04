#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"
#include "ImageAnalyzer.hpp"
#include "ImageTemplates.hpp"
#include "AreaOfInterestCutter.hpp"

#include <future>

using Image = cv::Mat;

class DetectionSystem :public TableSubject, public PrintScreenMaker, ImageAnalyzer, ImageTemplates, AreaOfInterestCutter {
  const std::string tableImageFilePath;

  TableSnapshot previousTableSnapshot;
  Position leftUpperPosition;

  std::vector<Card> getCardsFromSelectedArea(std::array<Image,13> const& imageTemplates, Image const& areaImage, Card_Color colorOfCardsInArea);
  Card findStackCard(Image const& stackArea);
  Image cutGreenField(Image const& tableImage);
 public:
  DetectionSystem(std::string const& imageFilePath);
  void processTable();
};
