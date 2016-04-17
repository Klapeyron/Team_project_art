#pragma once

#include <map>
#include <string>

#include "Position.hpp"

const std::string END_GAME_BUTTON = "endgame";
const std::string OK_BUTTON = "ok";
const std::string KNOCK_KNOCK_BUTTON = "knock";
const std::string STACK_BUTTON = "stack";
const std::string HIDDEN_STACK_BUTTON = "hidden_stack";

class ButtonsSnapshot
{
  std::map<std::string,Position> buttons;
 public:
  static Position getButtonPosition(std::string const& buttonName);
};
