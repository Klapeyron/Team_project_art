#include "ArtificialIntelligence.hpp"

void ArtificialIntelligence::onUpdate(TableSnapshot const& tableSnapshot)
{
	if(tableSnapshot.myMove)
	{
		for (auto karta : tableSnapshot.playerCards)
			cards.insert(karta);
		AddFindSeqs();
		AddFindGroups();
		if (DecisionToTakeTheCard(tableSnapshot.stackCard))
		{
			gameControl.pickCardFromStack();
		}
		else
		{
			gameControl.pickCardFromHiddenStack();
                }
	}
	else
		RemoveCardFromDeck(tableSnapshot.stackCard);
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

void ArtificialIntelligence::AddFindGroups()
{
	groups = FindGroups(cards);
}

std::vector < std::vector < Card > > ArtificialIntelligence::FindGroups(const std::multiset < Card, Comparation >& source)
{
	std::vector < std::vector < Card > > groupsTmp;
	std::vector< Card > tmp;
	for (std::multiset< Card >::iterator it = source.begin(); it != source.end(); ++it)
	{
		std::vector< Card >::iterator it2;
		for (it2 = tmp.begin(); it2 != tmp.end(); ++it2)
			if(it2->getFigure() != it->getFigure())
				break;
			if(it2 != tmp.end())
			{
				if(tmp.size() >= minSeqLen)
					groupsTmp.push_back(tmp);
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
		groupsTmp.push_back(tmp);
	return groupsTmp;
}

bool ArtificialIntelligence::FindCardInGroups(const Card& card, std::vector < std::vector < Card > > groups)
{
	for (std::vector < std::vector < Card > >::iterator it1 = groups.begin(); it1 != groups.end(); ++it1)
		for (std::vector< Card >::iterator it = it1->begin(); it != it1->end(); ++it)
			if(*it == card)
				return true;
	return false;
}

void ArtificialIntelligence::AddFindSeqs()
{
	sequences = FindSeqs(cards);
}

std::vector < std::multiset < Card, Comparation > > ArtificialIntelligence::FindSeqs(const std::multiset < Card, Comparation >& source)
{
	std::vector < std::multiset < Card, Comparation > > tmp;
	for(int color = 1; color <= static_cast<int>(Card_Color::DIAMOND); ++color)
	{
		std::vector < std::multiset < Card, Comparation > > tmp2 = FindOneColorSeq(static_cast<Card_Color> (color), source);
		tmp.insert(tmp.end(), tmp2.begin(), tmp2.end());
	}
	return tmp;
}

std::vector < std::multiset < Card, Comparation > > ArtificialIntelligence::FindOneColorSeq(Card_Color color, const std::multiset < Card, Comparation >& source)
{
	std::vector < std::multiset < Card, Comparation > > tmp;
	int i = 0;
	std::multiset < Card, Comparation > sequence;
	//szukanie startu
	for (std::multiset < Card, Comparation >::iterator it = source.begin(); it != source.end(); ++it)
		if(it->getColor() == color)
		{
			i = static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure());
			break;
		}
		//szukanie sekwencji
		for (std::multiset < Card, Comparation >::iterator it = source.begin(); it != source.end(); ++it)
		{
			if(it->getColor() == color)
			{
				if(static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure()) == i)
					sequence.insert(Card(it->getFigure(), it->getColor()));
				else
				{
					if(sequence.size() >= minSeqLen)
						tmp.push_back(sequence);
					sequence.clear();
					--it;
				}
				if(++i > static_cast<int> (Card_Figure::KING)) break;
			}
		}
		if(sequence.size() >= minSeqLen)
			tmp.push_back(sequence);
		return tmp;
}

bool ArtificialIntelligence::FindCardInSeq(const Card& card, const std::multiset < Card, Comparation >& sequence)
{
	std::multiset < Card, Comparation >::const_iterator it = sequence.begin();
	for ( ; it != sequence.end(); ++it)
		if(*it == card)
			return true;
		return false;
}

bool ArtificialIntelligence::FindCardInSeqs(const Card& card, const std::vector < std::multiset < Card, Comparation > >& sequences)
{
	for(std::vector < std::multiset < Card, Comparation > >::const_iterator it = sequences.begin(); it != sequences.end(); ++it)
		if(FindCardInSeq(card, *it))
			return true;
		return false;
}

bool ArtificialIntelligence::DecisionToTakeTheCard(const Card& card)
{
	for (std::vector < std::multiset < Card, Comparation > >::iterator it1 = sequences.begin(); it1 != sequences.end(); ++it1)
	{
		if (it1->begin()->getFigure() > Card_Figure::A)//sprawdzanie końca
		{
			Card suitableCardBegin = Card(static_cast<Card_Figure>(static_cast<int>(it1->begin()->getFigure()) - 1), it1->begin()->getColor());
			if (suitableCardBegin == card)
				return true;
		}
		if (it1->end()->getFigure() < Card_Figure::KING)//sprawdzanie początku
		{
                  Card suitableCardEnd = Card(static_cast<Card_Figure>(static_cast<int>(std::prev(it1->end())->getFigure()) + 1), std::prev(it1->end())->getColor());
			if (suitableCardEnd == card)
				return true;
		}
	}
	for (std::vector < std::vector < Card > >::iterator it1 = groups.begin(); it1 != groups.end(); ++it1)
	{
		if (it1->begin()->getFigure() == card.getFigure())
		{
			std::vector < Card >::const_iterator it2 = it1->begin();
			for(; it2 != it1->end(); ++it2)
				if(it2->getColor()==card.getColor())
					break;
				if(it2==it1->end())
					return true;
		}
	}
	std::multiset < Card, Comparation > tmpCards;
	for (std::multiset < Card, Comparation >::iterator it = cards.begin(); it != cards.end(); ++it)
	{
		if(!FindCardInSeqs(*it, sequences) && !FindCardInGroups(*it, groups))
			tmpCards.insert(*it);
	}
	tmpCards.insert(card);
	if(!FindSeqs(tmpCards).empty() or !FindGroups(tmpCards).empty())
		return true;
	
	return false;
}

void ArtificialIntelligence::RemoveCardFromDeck(const Card& card)
{
	for(std::list < Card >::iterator it = cardDeck.begin(); it != cardDeck.end(); ++it)
		if(*it == card)
                {
			cardDeck.erase(it);
                        break;
                }
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
	cards.insert(Card(Card_Figure::SEVEN, Card_Color::DIAMOND));
	cards.insert(Card(Card_Figure::SEVEN, Card_Color::HEART));
}

void ArtificialIntelligence::ShowCards()
{
	std::cout << "Cards:" << std::endl;
	for (std::multiset< Card >::iterator it = cards.begin(); it != cards.end(); ++it)
	{
		std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(it->getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(it->getColor()) << std::endl;
	}
}
