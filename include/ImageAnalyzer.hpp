#pragma once

#include <opencv/cv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "SizeConverter.hpp"

const double THRESHOLD = 0.87;
const int MATCH_METHOD = CV_TM_CCOEFF_NORMED;
using Image = cv::Mat;

class ImageAnalyzer {
 public:
  bool containsImageTemplate(Image const& image, Image const& templ);
};
