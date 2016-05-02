#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"
#include "ImageAnalyzer.hpp"

#include <array>

class DetectionSystem :public TableSubject, public PrintScreenMaker, ImageAnalyzer {
  const std::array<std::string, 13> blackTemplateFileNames
  {{"Ab.tiff","2b.tiff","3b.tiff","4b.tiff","5b.tiff","6b.tiff","7b.tiff","8b.tiff","9b.tiff","10b.tiff","Jb.tiff","Qb.tiff","Kb.tiff"}};
  const std::array<std::string, 13> redTemplateFileNames
  {{"Ar.tiff","2r.tiff","3r.tiff","4r.tiff","5r.tiff","6r.tiff","7r.tiff","8r.tiff","9r.tiff","10r.tiff","Jr.tiff","Qr.tiff","Kr.tiff"}};

  const std::string leftUpperCornerFileName = "LUcorner.tiff";
  const std::string rightUpperCornerFileName = "RUcorner.tiff";
  const std::string myTurnFileName = "myTurn.tiff";
  
  const std::string tableImageFilePath;
  const std::string templatesDirectory = "../templates/";

  std::array<Image, 13> blackCardTemplates;
  std::array<Image, 13> redCardTemplates;
  Image leftUpperCorner, rightUpperCorner, myTurn;
  Position leftUpperPosition;
  
  Image cutGreenField(Image const& tableImage);
  Image cutUpperCards(Image const& greenField);
  Image cutLowerCards(Image const& greenField);
  Image cutEnemyCards(Image const& greenField);
  Image cutMiddlePart(Image const& greenField);
  Image cutStackPart(Image const& greenField);
 public:
  DetectionSystem(std::string const& imageFilePath);
  void processTable();
};
