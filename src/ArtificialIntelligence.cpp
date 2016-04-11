#include "ArtificialIntelligence.hpp"

void ArtificialIntelligence::onUpdate(TableSnapshot const& tableSnapshot)
{
  for (auto karta : tableSnapshot.playerCards)
    cards.insert(karta);
  FindSeqs();
  FindGroups();
  if (DecisionToTakeTheCard(tableSnapshot.stackCard))
    gameControl.pickCardFromStack();
  else
    gameControl.pickCardFromHiddenStack();
}

void ArtificialIntelligence::ShowGroups()
{
  int l = 1;
  for (std::vector < std::vector < Card > >::iterator it1 = groups.begin(); it1 != groups.end(); ++it1)
  {
    std::cout << "Group: " << l++ << std::endl;
    for (std::vector< Card >::iterator it = it1->begin(); it != it1->end(); ++it)
    {
      std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(it->getColor()) << std::endl;
    }
  }
}

void ArtificialIntelligence::ShowSeqs()
{
  int l = 1;
  for (std::vector < std::multiset < Card, Comparation > >::iterator it1 = sequences.begin(); it1 != sequences.end(); ++it1)
  {
    std::cout << "Seq: " << l++ << std::endl;
    for (std::multiset< Card >::iterator it = it1->begin(); it != it1->end(); ++it)
    {
      std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(it->getColor()) << std::endl;
    }
  }
}

void ArtificialIntelligence::FindGroups()
{
  std::vector< Card > tmp;
  for (std::multiset< Card >::iterator it = cards.begin(); it != cards.end(); ++it)
  {
    std::vector< Card >::iterator it2;
    for (it2 = tmp.begin(); it2 != tmp.end(); ++it2)
    if(it2->getFigure() != it->getFigure())
      break;
    if(it2 != tmp.end())
    {
      if(tmp.size() >= minSeqLen)
      groups.push_back(tmp);
      tmp.clear();
    }
    std::vector< Card >::iterator it3;
    for (it3 = tmp.begin(); it3 != tmp.end(); ++it3)
    if(it3->getColor() == it->getColor())
      break;
    if(it3 == tmp.end())
    tmp.push_back(*it);
  }
  if(tmp.size() >= minSeqLen)
  groups.push_back(tmp);
}

void ArtificialIntelligence::FindSeqs()
{
  for(int color = 1; color <= static_cast<int>(Card_Color::DIAMOND); color++)
    FindSeqOneColor(static_cast<Card_Color> (color));
}

void ArtificialIntelligence::FindSeqOneColor(Card_Color color)
{
  int i = 0;
  std::multiset < Card, Comparation > sequence;
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

bool ArtificialIntelligence::DecisionToTakeTheCard(const Card& card)
{
  for (std::vector < std::multiset < Card, Comparation > >::iterator it1 = sequences.begin(); it1 != sequences.end(); ++it1)
  {
    if (it1->begin()->getFigure() > Card_Figure::A)
    {
      Card suitableCardBegin = Card(static_cast<Card_Figure>(static_cast<int>(it1->begin()->getFigure()) - 1), it1->begin()->getColor());
      if (suitableCardBegin == card)
      return true;
    }
    if (it1->end()->getFigure() < Card_Figure::KING)
    {
      Card suitableCardEnd = Card(static_cast<Card_Figure>(static_cast<int>(it1->end()->getFigure()) + 1), it1->end()->getColor());
      if (suitableCardEnd == card)
      return true;
    }
  }
  for (std::vector < std::vector < Card > >::iterator it1 = groups.begin(); it1 != groups.end(); ++it1)
  {
    if (it1->begin()->getColor() > Card_Color::SPADE)
    {
      Card suitableCardBegin = Card(it1->begin()->getFigure(), static_cast<Card_Color>(static_cast<int>(it1->begin()->getColor()) - 1));
    if (suitableCardBegin == card)
      return true;
    }
    if (it1->end()->getColor() < Card_Color::DIAMOND)
    {
      Card suitableCardBegin = Card(it1->end()->getFigure(), static_cast<Card_Color>(static_cast<int>(it1->end()->getColor()) + 1));
    if (suitableCardBegin == card)
      return true;
    }
  }
  return false;
}

void ArtificialIntelligence::CreateCards()
{
  cards.insert(Card(Card_Figure::THREE, Card_Color::SPADE));
  cards.insert(Card(Card_Figure::THREE, Card_Color::CLUB));
  cards.insert(Card(Card_Figure::SEVEN, Card_Color::SPADE));
  cards.insert(Card(Card_Figure::FOUR, Card_Color::SPADE));
  cards.insert(Card(Card_Figure::TWO, Card_Color::SPADE));
  cards.insert(Card(Card_Figure::KING, Card_Color::SPADE));
  cards.insert(Card(Card_Figure::NINE, Card_Color::SPADE));
  cards.insert(Card(Card_Figure::EIGHT, Card_Color::SPADE));
  cards.insert(Card(Card_Figure::FIVE, Card_Color::HEART));
  cards.insert(Card(Card_Figure::FIVE, Card_Color::DIAMOND));
  cards.insert(Card(Card_Figure::FIVE, Card_Color::CLUB));
  cards.insert(Card(Card_Figure::FOUR, Card_Color::HEART));
  cards.insert(Card(Card_Figure::THREE, Card_Color::HEART));
  cards.insert(Card(Card_Figure::FIVE, Card_Color::SPADE));
  cards.insert(Card(Card_Figure::FIVE, Card_Color::SPADE));
}

void ArtificialIntelligence::ShowCards()
{
  std::cout << "Cards:" << std::endl;
  for (std::multiset< Card >::iterator it = cards.begin(); it != cards.end(); ++it)
  {
    std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(it->getColor()) << std::endl;
  }
}