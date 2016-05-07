#pragma once

#include <map>
#include <string>

#include "Position.hpp"

enum class ButtonsConstants : unsigned {OK_BUTTON, PAS_BUTTON, KNOCK_KNOCK_BUTTON, START_BUTTON,HIDDEN_STACK,STACK};

struct ButtonsSnapshot
{
  std::map<ButtonsConstants, std::pair<bool, Position>> buttons;
};
