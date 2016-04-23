#include <string>
#include <opencv/cv.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#include "DetectionSystem.hpp"
#include "GameControl.hpp"
#include "ArtificialIntelligence.hpp"

int main(int argc, char *argv[])
{
  DetectionSystem ds("../test/DetectionSystemImages/tableSnapshot1366x768.tiff");
  GameControl gc;
  ArtificialIntelligence ai(gc);
  ds.registerOberver(ai);
  return 0;
}


// int main(int argc, char *argv[])
// {
//   const double THRESHOLD = 0.87;
//   const std::string IMAGE_TEMPLATES_ADRESS = "zdjecia/foto/";
//   const std::string PRT_SCR_IMAGE_NAME = "foto.png";
//   const std::string PRT_SCR_APPLICATION = "scrot";
//   const int MATCH_METHOD = CV_TM_CCOEFF_NORMED;
//   const int MAX_TRACKBAR = 15;
  
//   cv::Mat result,sourceImage,templ;
//   double minVal, maxVal;

//   sourceImage = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);
//   templ = cv::imread(argv[2], CV_LOAD_IMAGE_COLOR);
  
//   std::string image_window = "myLowerCards";
//   // std::string result_window = "enemyCards";
  
//   cv::namedWindow( image_window, CV_WINDOW_AUTOSIZE );
//   // cv::namedWindow( result_window, CV_WINDOW_AUTOSIZE );
  
//   // cv::imshow( result_window, templ );
  
//   // cv::waitKey(0);
//   std::vector<double> resss;
//   // for (int i = 0; i < 100; ++i) {
//   cv::Mat temp;
//   // cv::resize(templ, temp, cv::Size(), 0.711458, 0.711111);
//   cv::resize(templ, temp, cv::Size(), 0.711458, 0.711111);
//   templ = temp;
//   // cv::imshow( image_window, templ );
//   //   cv::waitKey(0);
//   cv::matchTemplate(sourceImage,templ,result,MATCH_METHOD);
//   cv::minMaxLoc(result, &minVal, &maxVal);
//   // }
//   std::cout << maxVal << std::endl;
//   // auto elem = std::max_element(resss.begin(),resss.end());
//   // std::cout << *elem << std::endl;
// }


