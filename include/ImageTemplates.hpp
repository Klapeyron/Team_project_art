#pragma once

#include <array>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class ImageTemplates {
  const std::array<std::string, 13> blackTemplateFileNames
  {{"Ab.tiff","2b.tiff","3b.tiff","4b.tiff","5b.tiff","6b.tiff","7b.tiff","8b.tiff","9b.tiff","10b.tiff","Jb.tiff","Qb.tiff","Kb.tiff"}};
  const std::array<std::string, 13> redTemplateFileNames
  {{"Ar.tiff","2r.tiff","3r.tiff","4r.tiff","5r.tiff","6r.tiff","7r.tiff","8r.tiff","9r.tiff","10r.tiff","Jr.tiff","Qr.tiff","Kr.tiff"}};
  const std::array<std::string, 13> redStackTemplateFileNames
  {{"sAr.tiff","s2r.tiff","s3r.tiff","s4r.tiff","s5r.tiff","s6r.tiff","s7r.tiff","s8r.tiff","s9r.tiff","s10r.tiff","sJr.tiff","sQr.tiff","sKr.tiff"}};
  const std::array<std::string, 13> blackStackTemplateFileNames
  {{"sAb.tiff","s2b.tiff","s3b.tiff","s4b.tiff","s5b.tiff","s6b.tiff","s7b.tiff","s8b.tiff","s9b.tiff","s10b.tiff","sJb.tiff","sQb.tiff","sKb.tiff"}};
  const std::array<std::string, 4> stackColorFileNames
  {{"sSpade.tiff","sHeart.tiff","sClub.tiff","sDiamond.tiff"}};

  const std::string leftUpperCornerFileName = "LUcorner.tiff";
  const std::string rightUpperCornerFileName = "RUcorner.tiff";
  const std::string myTurnFileName = "myTurn.tiff";
  const std::string enemyCardTakenFileName = "enemyCardTaken.tiff";
  const std::string pasButtonFileName = "pas.tiff";
  const std::string okButtonFileName = "ok.tiff";
  const std::string stukamButtonFileName = "stukam.tiff";
  const std::string pasDisabledButtonFileName = "pasDisabled.tiff";
  const std::string okDisabledButtonFileName = "okDisabled.tiff";
  const std::string stukamDisabledButtonFileName = "stukamDisabled.tiff";
  const std::string startButtonFileName = "start.tiff";
  const std::string blueBackgroundFileName = "stackBackground.tiff";

  const std::string templatesDirectory = "../templates/";
 public:
  using ColorTemplatesType = std::array<cv::Mat, 4>;
  using FigureTemplatesType = std::array<cv::Mat, 13>;
  using CardTemplatesType = FigureTemplatesType;

  ImageTemplates(std::string const& templatesDir);
  FigureTemplatesType blackCardTemplates;
  FigureTemplatesType redCardTemplates;
  FigureTemplatesType blackStackCardTemplates;
  FigureTemplatesType redStackCardTemplates;
  ColorTemplatesType stackColorTemplates;

  const cv::Mat leftUpperCorner, rightUpperCorner, myTurn, enemyCardTaken;
  const cv::Mat pasButton, okButton, stukamButton, startButton, blueBackground;
  const cv::Mat pasDisabledButton, okDisabledButton, stukamDisabledButton;
};
