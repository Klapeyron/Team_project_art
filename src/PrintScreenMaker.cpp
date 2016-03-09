#include "PrintScreenMaker.hpp"

namespace {
const std::string printScreenFileName = "prtScr.tiff";
}

void PrintScreenMaker::detectWindowByMouseClickOnIt() {
  std::system("xwininfo | grep 'Window id' | cut -d ':' -f3 | cut -d ' ' -f2 > windowID.txt");
  std::ifstream fileStream {"windowID.txt"};
  std::getline(fileStream,windowID);
  std::system("rm -rf windowID.txt");
}

void PrintScreenMaker::makePrintScreen() {
  auto command = "import -silent -window " + windowID + " " + printScreenFileName;
  std::system(command.c_str());
}
