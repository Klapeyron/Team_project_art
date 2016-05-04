#pragma once

#include "gtest/gtest.h"

MATCHER_P(ContainsThisSnapshot, value, "")
{
  return value == arg;
}
