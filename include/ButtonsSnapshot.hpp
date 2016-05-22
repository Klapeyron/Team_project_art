#pragma once

#include <map>
#include <string>

#include "Position.hpp"

enum class ButtonsConstants : unsigned {OK_BUTTON,
    OK_DISABLED_BUTTON,
    PAS_BUTTON,
    PAS_DISABLED_BUTTON,
    KNOCK_KNOCK_BUTTON,
    KNOCK_KNOCK_DISABLED_BUTTON,
    START_BUTTON,
    HIDDEN_STACK
      };

struct ButtonsSnapshot
{
  std::map<ButtonsConstants, std::pair<bool, Position>> buttons;
};
