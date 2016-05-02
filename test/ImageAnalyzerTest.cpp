#include "gtest/gtest.h"

#include "ImageAnalyzer.hpp"

TEST (DISABLED_ImageAnalyzerTest, blackJDetectionTest)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.png");
  auto templateImage = cv::imread("../templates/Jb.tiff");

  ImageAnalyzer imageAnalyzer;

  bool matched;
  std::tie(matched, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, templateImage);
  EXPECT_TRUE(matched);
}

TEST (DISABLED_ImageAnalyzerTest, hiddenRed5DetectionTest)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.png");
  auto templateImage = cv::imread("../templates/5r.tiff");

  ImageAnalyzer imageAnalyzer;

  bool matched;
  std::tie(matched, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, templateImage);
  EXPECT_TRUE(matched);
}

TEST (DISABLED_ImageAnalyzerTest, multiple8DetectionTest)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.png");
  auto templateImageBlack = cv::imread("../templates/8b.tiff");
  auto templateImageRed = cv::imread("../templates/8r.tiff");

  ImageAnalyzer imageAnalyzer;

  bool containsBlack, containsRed;
  std::tie(containsBlack, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, templateImageBlack);
  std::tie(containsRed, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, templateImageRed);
  EXPECT_TRUE(containsBlack);
  EXPECT_TRUE(containsRed);
}

TEST (DISABLED_ImageAnalyzerTest, redQueenDetectionTest_notMatched)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.png");
  auto templateImage = cv::imread("../templates/Qr.tiff");

  ImageAnalyzer imageAnalyzer;

  bool matched;
  Position position;
  std::tie(matched, position) = imageAnalyzer.containsImageTemplate(sourceImage, templateImage);
  EXPECT_FALSE(matched);
}

TEST (DISABLED_ImageAnalyzerTest, blackFiguresMatchTest_notMatched)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.png");

  ImageAnalyzer imageAnalyzer;
  bool matchedAb, matched2b, matched3b, matched4b, matched5b, matched6b, matched7b,
      matched8b, matched9b, matched10b, matchedJb, matchedQb, matchedKb = false;

  std::tie(matchedAb, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Ab.tiff"));
  std::tie(matched2b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/2b.tiff"));
  std::tie(matched3b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/3b.tiff"));
  std::tie(matched4b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/4b.tiff"));
  std::tie(matched5b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/5b.tiff"));
  std::tie(matched6b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/6b.tiff"));
  std::tie(matched7b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/7b.tiff"));
  std::tie(matched8b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/8b.tiff"));
  std::tie(matched9b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/9b.tiff"));
  std::tie(matched10b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/10b.tiff"));
  std::tie(matchedJb, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Jb.tiff"));
  std::tie(matchedQb, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Qb.tiff"));
  std::tie(matchedKb, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Kb.tiff"));

  EXPECT_FALSE(matchedAb);
  EXPECT_FALSE(matched2b);
  EXPECT_FALSE(matched3b);
  EXPECT_FALSE(matched4b);
  EXPECT_TRUE(matched5b);
  EXPECT_FALSE(matched6b);
  EXPECT_FALSE(matched7b);
  EXPECT_TRUE(matched8b);
  EXPECT_FALSE(matched9b);
  EXPECT_FALSE(matched10b);
  EXPECT_TRUE(matchedJb);
  EXPECT_TRUE(matchedQb);
  EXPECT_FALSE(matchedKb);
}

TEST (DISABLED_ImageAnalyzerTest, redFiguresMatchTest_notMatched)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1366x768.png");
  ImageAnalyzer imageAnalyzer;
  bool matchedAc, matched2c, matched3c, matched4c, matched5c, matched6c, matched7c,
      matched8c, matched9c, matched10c, matchedJc, matchedQc, matchedKc = false;
  
  std::tie(matchedAc, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Ar.tiff"));
  std::tie(matched2c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/2r.tiff"));
  std::tie(matched3c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/3r.tiff"));
  std::tie(matched4c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/4r.tiff"));
  std::tie(matched5c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/5r.tiff"));
  std::tie(matched6c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/6r.tiff"));
  std::tie(matched7c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/7r.tiff"));
  std::tie(matched8c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/8r.tiff"));
  std::tie(matched9c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/9r.tiff"));
  std::tie(matched10c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/10r.tiff"));
  std::tie(matchedJc, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Jr.tiff"));
  std::tie(matchedQc, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Qr.tiff"));
  std::tie(matchedKc, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Kr.tiff"));

  EXPECT_FALSE(matchedAc);
  EXPECT_TRUE(matched2c);
  EXPECT_TRUE(matched3c);
  EXPECT_FALSE(matched4c);
  EXPECT_TRUE(matched5c);
  EXPECT_FALSE(matched6c);
  EXPECT_FALSE(matched7c);
  EXPECT_TRUE(matched8c);
  EXPECT_FALSE(matched9c);
  EXPECT_FALSE(matched10c);
  EXPECT_FALSE(matchedJc);
  EXPECT_FALSE(matchedQc);
  EXPECT_TRUE(matchedKc);
}

TEST (DISABLED_ImageAnalyzerTest, 1680x1050Test_fullSnapshot)
{
  auto sourceImage = cv::imread("../test/DetectionSystemImages/tableSnapshot1680x1050.png");
  ImageAnalyzer imageAnalyzer;
  bool matchedAc, matched2c, matched3c, matched4c, matched5c, matched6c, matched7c,
      matched8c, matched9c, matched10c, matchedJc, matchedQc, matchedKc = false;
  bool matchedAb, matched2b, matched3b, matched4b, matched5b, matched6b, matched7b,
      matched8b, matched9b, matched10b, matchedJb, matchedQb, matchedKb = false;

  std::tie(matchedAc, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Ar.tiff"));
  std::tie(matched2c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/2r.tiff"));
  std::tie(matched3c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/3r.tiff"));
  std::tie(matched4c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/4r.tiff"));
  std::tie(matched5c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/5r.tiff"));
  std::tie(matched6c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/6r.tiff"));
  std::tie(matched7c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/7r.tiff"));
  std::tie(matched8c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/8r.tiff"));
  std::tie(matched9c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/9r.tiff"));
  std::tie(matched10c, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/10r.tiff"));
  std::tie(matchedJc, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Jr.tiff"));
  std::tie(matchedQc, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Qr.tiff"));
  std::tie(matchedKc, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Kr.tiff"));

  std::tie(matchedAb, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Ab.tiff"));
  std::tie(matched2b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/2b.tiff"));
  std::tie(matched3b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/3b.tiff"));
  std::tie(matched4b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/4b.tiff"));
  std::tie(matched5b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/5b.tiff"));
  std::tie(matched6b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/6b.tiff"));
  std::tie(matched7b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/7b.tiff"));
  std::tie(matched8b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/8b.tiff"));
  std::tie(matched9b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/9b.tiff"));
  std::tie(matched10b, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/10b.tiff"));
  std::tie(matchedJb, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Jb.tiff"));
  std::tie(matchedQb, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Qb.tiff"));
  std::tie(matchedKb, std::ignore) = imageAnalyzer.containsImageTemplate(sourceImage, cv::imread("../templates/Kb.tiff"));
  
  EXPECT_TRUE(matchedAc);
  EXPECT_FALSE(matched2c);
  EXPECT_FALSE(matched3c);
  EXPECT_FALSE(matched4c);
  EXPECT_TRUE(matched5c);
  EXPECT_FALSE(matched6c);
  EXPECT_FALSE(matched7c);
  EXPECT_FALSE(matched8c);
  EXPECT_FALSE(matched9c);
  EXPECT_FALSE(matched10c);
  EXPECT_FALSE(matchedJc);
  EXPECT_FALSE(matchedQc);
  EXPECT_FALSE(matchedKc);

  EXPECT_TRUE(matchedAb);
  EXPECT_TRUE(matched2b);
  EXPECT_TRUE(matched3b);
  EXPECT_FALSE(matched4b);
  EXPECT_FALSE(matched5b);
  EXPECT_TRUE(matched6b);
  EXPECT_TRUE(matched7b);
  EXPECT_TRUE(matched8b);
  EXPECT_TRUE(matched9b);
  EXPECT_FALSE(matched10b);
  EXPECT_FALSE(matchedJb);
  EXPECT_FALSE(matchedQb);
  EXPECT_TRUE(matchedKb);
}
