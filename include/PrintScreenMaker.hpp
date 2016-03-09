#pragma once

#include <stdio.h>
#include <string>
#include <fstream>

class PrintScreenMaker {
  std::string windowID;
 public:
  void detectWindowByMouseClickOnIt();
  void makePrintScreen();
};
