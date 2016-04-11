#pragma once

#include "gtest/gtest.h"

MATCHER_P(ContainsThisSnapshot, value, "") {
  for(auto const& card : value.playerCards)
    if( std::find(arg.playerCards.begin(), arg.playerCards.end(), card) == arg.playerCards.end() )
      return false;

  auto stackCardIsNone = value.stackCard == Card(Card_Figure::None, Card_Color::None);
  if(arg.stackCard != value.stackCard and not stackCardIsNone)
    return false;
  return true;
}
