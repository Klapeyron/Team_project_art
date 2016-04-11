#pragma once

#include <map>
#include <string>

#include "Position.hpp"

class ButtonsSnapshot
{
  std::map<std::string,Position> buttons;
 public:
  Position getButtonPosition(std::string const& buttonName);
};
