#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"
#include "ImageAnalyzer.hpp"
#include "ImageTemplates.hpp"

#include <future>

class DetectionSystem :public TableSubject, public PrintScreenMaker, ImageAnalyzer, ImageTemplates {
  const std::string tableImageFilePath;

  TableSnapshot previousTableSnapshot;
  Position leftUpperPosition;

  std::vector<Card> getCardsFromSelectedArea(std::array<Image,13> const& imageTemplates, Image const& areaImage, Card_Color colorOfCardsInArea);
  Card findStackCard(Image const& stackArea);

  Image cutGreenField(Image const& tableImage);
  Image cutUpperCards(Image const& greenField);
  Image cutLowerCards(Image const& greenField);
  Image cutEnemyCards(Image const& greenField);
  Image cutMiddlePart(Image const& greenField);
  Image cutStackPart(Image const& greenField);
 public:
  DetectionSystem(std::string const& imageFilePath);
  void processTable();
  using TableSubject::waitForUnfinishedJobs;
};
