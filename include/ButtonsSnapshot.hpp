#pragma once

#include <map>
#include <string>

#include "Position.hpp"

const std::string END_GAME_BUTTON = "endgame";
const std::string OK_BUTTON = "ok";
const std::string KNOCK_KNOCK_BUTTON = "knock";

class ButtonsSnapshot
{
  std::map<std::string,Position> buttons;
 public:
  Position getButtonPosition(std::string const& buttonName);
};
