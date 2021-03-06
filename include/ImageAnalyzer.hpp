#pragma once

#include <tuple>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "Position.hpp"

const double THRESHOLD = 0.90;
const int MATCH_METHOD = CV_TM_CCOEFF_NORMED;

class ImageAnalyzer {
 public:
  std::tuple<bool, Position> containsImageTemplate(cv::Mat const& image,
                                                   cv::Mat const& templ,
                                                   const cv::Rect referencePoint = cv::Rect(0,0,0,0));
  std::tuple<bool, Position> containsImageTemplate(cv::Mat const& image, cv::Mat const& templ, Position position);
};
