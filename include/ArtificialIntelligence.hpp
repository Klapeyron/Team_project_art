#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <type_traits>

#include "IGameControl.hpp"
#include "TableObserver.hpp"

struct Porownanie
{
  bool operator()(const Card& a, const Card& b)
  {
    return a.getFigure() < b.getFigure();
  }
};

class ArtificialIntelligence :public TableObserver {
  IGameControl & gameControl;
  const int minSeqLen = 3;
  std::multiset < Card, Porownanie > cards;//karty do przeszukania
  std::vector < std::multiset < Card, Porownanie > > sequences;//znalezione sekwencje
  
 public:
  ArtificialIntelligence(IGameControl & gameControlInterface) :gameControl(gameControlInterface) {}
  void onUpdate(TableSnapshot const&);
  
  std::vector < std::multiset < Card, Porownanie > > ReturnSeqs()
  {
    return sequences;
  }
  
  std::vector < std::multiset < Card, Porownanie > >& Seqs()
  {
    return sequences;
  }
  
  void ShowSeqs()
  {
    int l = 1;
    for (std::vector < std::multiset < Card, Porownanie > >::iterator it1 = sequences.begin(); it1 != sequences.end(); ++it1)
    {
      std::cout << "Seq: " << l++ << std::endl;
      for (std::multiset< Card >::iterator it = it1->begin(); it != it1->end(); ++it)
      {
	std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(it->getColor()) << std::endl;
      }
    }
  }
  
  void FindAllSeqs()
  {
    FindSeqOneColor(Card_Color::SPADE);
    FindSeqOneColor(Card_Color::HEART);
    FindSeqOneColor(Card_Color::CLUB);
    FindSeqOneColor(Card_Color::DIAMOND);
  }
  
  void FindSeqOneColor(Card_Color color)
  {
    int i = 0;
    std::multiset < Card, Porownanie > sequence;
    //szukanie startu
    for (std::multiset< Card >::iterator it = cards.begin(); it != cards.end(); ++it)
      if(it->getColor() == color)
      {
	i = static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure());
	
	break;
      }
    //szukanie sekwencji
    for (std::multiset< Card >::iterator it = cards.begin(); it != cards.end(); ++it)
    {
      if(it->getColor() == color)
      {
	if(static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure()) == i)
	  sequence.insert(Card(it->getFigure(), it->getColor()));
	else
	{
	  if(sequence.size() >= minSeqLen)
	    sequences.push_back(sequence);
	  sequence.clear();
	  --it;
	}
	if(++i > static_cast<int> (Card_Figure::KING)) break;
      }
    }
    if(sequence.size() >= minSeqLen)
	    sequences.push_back(sequence);
  }
  
  void CreateCards()
  {
    cards.insert(Card(Card_Figure::THREE, Card_Color::SPADE));
    cards.insert(Card(Card_Figure::SEVEN, Card_Color::SPADE));
    cards.insert(Card(Card_Figure::FOUR, Card_Color::SPADE));
    cards.insert(Card(Card_Figure::TWO, Card_Color::SPADE));
    cards.insert(Card(Card_Figure::KING, Card_Color::SPADE));
    cards.insert(Card(Card_Figure::NINE, Card_Color::SPADE));
    cards.insert(Card(Card_Figure::EIGHT, Card_Color::SPADE));
    cards.insert(Card(Card_Figure::FIVE, Card_Color::HEART));
    cards.insert(Card(Card_Figure::FOUR, Card_Color::HEART));
    cards.insert(Card(Card_Figure::THREE, Card_Color::HEART));
    cards.insert(Card(Card_Figure::FIVE, Card_Color::SPADE));
  }
  
  void ShowCards()
  {
    std::cout << "Cards:" << std::endl;
    for (std::multiset< Card >::iterator it = cards.begin(); it != cards.end(); ++it)
    {
      std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(it->getColor()) << std::endl;
    }
  }
};
