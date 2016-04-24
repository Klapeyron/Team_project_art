#include "gtest/gtest.h"

#include "ImageAnalyzer.hpp"

TEST (ImageAnalyzerTest, black10DetectionTest)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.tiff");
  auto templateImage = cv::imread("../templates/10b.tiff");

  ImageAnalyzer imageAnalyzer;

  bool containsImage = imageAnalyzer.containsImageTemplate(sourceImage, templateImage);
  EXPECT_TRUE(containsImage);
}

TEST (ImageAnalyzerTest, hiddenRed5DetectionTest)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.tiff");
  auto templateImage = cv::imread("../templates/5c.tiff");

  ImageAnalyzer imageAnalyzer;

  bool containsImage = imageAnalyzer.containsImageTemplate(sourceImage, templateImage);
  EXPECT_TRUE(containsImage);
}

TEST (ImageAnalyzerTest, multiple6DetectionTest)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.tiff");
  auto templateImageBlack = cv::imread("../templates/6b.tiff");
  auto templateImageRed = cv::imread("../templates/6c.tiff");

  ImageAnalyzer imageAnalyzer;

  bool containsBlack = imageAnalyzer.containsImageTemplate(sourceImage, templateImageBlack);
  bool containsRed = imageAnalyzer.containsImageTemplate(sourceImage, templateImageRed);
  EXPECT_TRUE(containsBlack);
  EXPECT_TRUE(containsRed);
}

TEST (ImageAnalyzerTest, blackQueenDetectionTest_notMatched)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.tiff");
  auto templateImage = cv::imread("../templates/Qb.tiff");

  ImageAnalyzer imageAnalyzer;

  bool containsImage = imageAnalyzer.containsImageTemplate(sourceImage, templateImage);
  EXPECT_FALSE(containsImage);
}

TEST (ImageAnalyzerTest, blackFiguresMatchTest_notMatched)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.tiff");

  ImageAnalyzer imageAnalyzer;

  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Ab.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/2b.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/3b.tiff")));
  EXPECT_TRUE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/4b.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/5b.tiff")));
  EXPECT_TRUE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/6b.tiff")));
  EXPECT_TRUE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/7b.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/8b.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/9b.tiff")));
  EXPECT_TRUE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/10b.tiff")));
  EXPECT_TRUE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Jb.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Qb.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Kb.tiff")));
}

TEST (ImageAnalyzerTest, redFiguresMatchTest_notMatched)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.tiff");

  ImageAnalyzer imageAnalyzer;

  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Ac.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/2c.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/3c.tiff")));
  EXPECT_TRUE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/4c.tiff")));
  EXPECT_TRUE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/5c.tiff")));
  EXPECT_TRUE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/6c.tiff")));
  EXPECT_FALSE (imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/7c.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/8c.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/9c.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/10c.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Jc.tiff")));
  EXPECT_FALSE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Qc.tiff")));
  EXPECT_TRUE(imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Kc.tiff")));
}
