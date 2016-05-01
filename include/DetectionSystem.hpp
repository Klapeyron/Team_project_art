#pragma once

#include "TableSnapshot.hpp"
#include "TableSubject.hpp"
#include "PrintScreenMaker.hpp"
#include "ImageAnalyzer.hpp"

#include <array>

namespace {
const std::array<std::string, 13> blackTemplateFileNames {"Ab.tiff","2b.tiff","3b.tiff","4b.tiff","5b.tiff","6b.tiff",
      "7b.tiff","8b.tiff","9b.tiff","10b.tiff","Jb.tiff","Qb.tiff","Kb.tiff"};
const std::array<std::string, 13> redTemplateFileNames {"Ar.tiff","2r.tiff","3r.tiff","4r.tiff","5r.tiff","6r.tiff",
      "7r.tiff","8r.tiff","9r.tiff","10r.tiff","Jr.tiff","Qr.tiff","Kr.tiff"};
}

class DetectionSystem :public TableSubject, public PrintScreenMaker, ImageAnalyzer {
  const std::string tableImageFilePath;
  const std::string templatesDirectory = "../templates/";

  std::array<Image, 13> blackCardTemplates;
  std::array<Image, 13> redCardTemplates;
 public:
  DetectionSystem(std::string const& imageFilePath);
  void processTable();
};
