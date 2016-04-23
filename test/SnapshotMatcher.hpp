#pragma once

#include "gtest/gtest.h"

MATCHER_P(ContainsThisSnapshot, value, "") {
  for(auto const& card : value.playerCards)
    if( std::find(arg.playerCards.begin(), arg.playerCards.end(), card) == arg.playerCards.end() )
      return false;

  auto stackCardIsNone = Card(Card_Figure::None, Card_Color::None) == value.stackCard;
  if(arg.stackCard != value.stackCard and not stackCardIsNone)
    return false;

  if(arg.myMove != value.myMove)
    return false;
  return true;
}
