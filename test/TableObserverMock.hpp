#pragma once

#include "gmock/gmock.h"
#include "TableObserver.hpp"

class TableObserverMock :public TableObserver
{
 public:
  MOCK_METHOD0(pickCardFromStack,void());
  MOCK_METHOD1(onUpdate,void(TableSnapshot const&));
};

inline std::ostream& operator<<(::std::ostream& os, const Card_Color& color) {
  if(Card_Color::SPADE == color) return os << "SPADE";
  if(Card_Color::HEART == color) return os << "HEART";
  if(Card_Color::CLUB == color) return os << "CLUB";
  if(Card_Color::DIAMOND == color) return os << "DIAMOND";
  if(Card_Color::None == color) return os << "None";
}

inline std::ostream& operator<<(::std::ostream& os, const Card_Figure& figure) {
  if(Card_Figure::A == figure) return os << "A";
  if(Card_Figure::JACK == figure) return os << "JACK";
  if(Card_Figure::QUEEN == figure) return os << "QUEEN";
  if(Card_Figure::KING == figure) return os << "KING";
  if(Card_Figure::None == figure) return os << "None";
  return os << static_cast<int> (figure);
}

inline std::ostream& operator<<(::std::ostream& os, const Card& card) {
  return os << card.getFigure() << " " << card.getColor() << std::endl;
}

inline std::ostream& operator<<(::std::ostream& os, const TableSnapshot& snapshot) {
  os << "Player cards: ";
  for(auto const& playerCard : snapshot.playerCards)
    os << playerCard << " | ";
  return os << "stackCard: " << snapshot.stackCard << " | " << "myMove: " << snapshot.myMove;
}
