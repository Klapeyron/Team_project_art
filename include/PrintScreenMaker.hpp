#pragma once

#include <stdio.h>
#include <string>
#include <fstream>

namespace
{
const std::string printScreenFileName = "prtScr.tiff";
}

class PrintScreenMaker {
  std::string windowID;
 public:
  void detectWindowByMouseClickOnIt();
  void makePrintScreen();
};
