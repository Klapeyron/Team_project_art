#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <type_traits>

#include "IGameControl.hpp"
#include "TableObserver.hpp"

struct Comparation
{
  bool operator()(const Card& a, const Card& b)
  {
    return a.getFigure() < b.getFigure();
  }
};

class ArtificialIntelligence :public TableObserver {
  IGameControl & gameControl;
  const int minSeqLen = 3;
  std::list < Card > cardDeck;//talia kart
  std::multiset < Card, Comparation > cards;//karty do przeszukania
  std::vector < std::multiset < Card, Comparation > > sequences;//znalezione sekwencje
  std::vector < std::vector < Card > > groups;//znalezione grupy
  
 public:
  ArtificialIntelligence(IGameControl & gameControlInterface) :gameControl(gameControlInterface)
  {
    for(int figure = 1; figure <= static_cast<int>(Card_Figure::KING); ++figure)
      for(int color = 1; color <= static_cast<int>(Card_Color::DIAMOND); ++color)
	cardDeck.push_back( Card( static_cast<Card_Figure> (figure), static_cast<Card_Color> (color)));
  }
  void onUpdate(TableSnapshot const&);
  
  std::vector < std::multiset < Card, Comparation > > ReturnSeqs()
  {
    return sequences;
  }
  
  std::vector < std::multiset < Card, Comparation > >& Seqs()
  {
    return sequences;
  }
  
  std::vector < std::vector < Card > > ReturnGroups()
  {
    return groups;
  }
  
  std::vector < std::vector < Card > >& Groups()
  {
    return groups;
  }
  
  void ShowGroups();
  
  void ShowSeqs();
  
  void AddFindGroups();
  
  std::vector < std::vector < Card > > FindGroups(const std::multiset < Card, Comparation >& source);
  
  bool FindCardInGroups(const Card& card, std::vector < std::vector < Card > > groups);
  
  void AddFindSeqs();
  
  std::vector < std::multiset < Card, Comparation > > FindSeqs(const std::multiset < Card, Comparation >& source);
  
  std::vector < std::multiset < Card, Comparation > > FindOneColorSeq(Card_Color color, const std::multiset < Card, Comparation >& source);
  
  bool FindCardInSeq(const Card& card, const std::multiset < Card, Comparation >& sequence);
  
  bool FindCardInSeqs(const Card& card, const std::vector < std::multiset < Card, Comparation > >& sequences);
  
  bool DecisionToTakeTheCard(const Card& card);
  
  void RemoveCardFromDeck(const Card& card);
  
  void CreateCards();
  
  void ShowCards();
};
