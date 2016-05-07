#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <type_traits>

#include "IGameControl.hpp"
#include "TableObserver.hpp"

#define MIN_SEQ_LEN (3)
#define MIN_GROUP_LEN (3)
#define COLORS_COUNT (4)
#define PLAYER_CARDS_COUNT (10)

#define COLOR_RESET "\e[0m"
#define BLACK "\e[0;30m"
#define GRAY "\e[1;30m"
#define RED "\e[0;31m"
#define DIM_RED "\e[1;31m"
#define GREEN "\e[0;32m"
#define DIM_GREEN "\e[1;32m"
#define YELLOW "\e[0;33m"
#define DIM_YELLOW "\e[1;33m"
#define BLUE "\e[0;34m"
#define DIM_BLUE "\e[1;34m"
#define PURPLE "\e[0;35m"
#define DIM_PURPLE "\e[1;35m"
#define CYAN "\e[0;36m"
#define DIM_CYAN "\e[1;36m"
#define WHITE "\e[0;37m"
#define DIM_WHITE "\e[1;37m"

class AICard: public Card
{
	int id;
	double usefulnessForOpponent;
	
	friend std::ostream& operator<< ( std::ostream& wyjscie, const AICard& card )
	{
		wyjscie << "Figure: ";
		switch(card.getFigure())
		{
			case Card_Figure::None : wyjscie << DIM_RED << "None" << COLOR_RESET;
								break;
			case Card_Figure::A : wyjscie << "A";
								break;
			case Card_Figure::TWO : wyjscie << "TWO";
								break;
			case Card_Figure::THREE : wyjscie << "THREE";
								break;
			case Card_Figure::FOUR : wyjscie << "FOUR";
								break;
			case Card_Figure::FIVE : wyjscie << "FIVE";
								break;
			case Card_Figure::SIX : wyjscie << "SIX";
								break;
			case Card_Figure::SEVEN : wyjscie << "SEVEN";
								break;
			case Card_Figure::EIGHT : wyjscie << "EIGHT";
								break;
			case Card_Figure::NINE : wyjscie << "NINE";
								break;
			case Card_Figure::TEN : wyjscie << "TEN";
								break;
			case Card_Figure::JACK : wyjscie << "JACK";
								break;
			case Card_Figure::QUEEN : wyjscie << "QUEEN";
								break;
			case Card_Figure::KING : wyjscie << "KING";
								break;
			default : wyjscie << "Different";
		}
		wyjscie << ", Color: ";
		switch(card.getColor())
		{
			case Card_Color::None : wyjscie << DIM_RED << "None" << COLOR_RESET;
									break;
			case Card_Color::SPADE : wyjscie << "SPADE";
									break;
			case Card_Color::HEART : wyjscie << "HEART";
									break;
			case Card_Color::CLUB : wyjscie << "CLUB";
									break;
			case Card_Color::DIAMOND : wyjscie << "DIAMOND";
									break;
			default : wyjscie << DIM_RED << "Different" << COLOR_RESET;
		}
		wyjscie << ", id: " << card.getId() << ", use: " << card.getUsefulnessForOpponent() << std::endl;
		return wyjscie;
	}
	
public:
	AICard(Card_Figure figure = Card_Figure::None, Card_Color color = Card_Color::None, int id = 0, double usefulness = 0.0) : Card(figure, color), id(id), usefulnessForOpponent(usefulness) {}
	
	AICard(const Card& card, const int& id = 0, const double& usefulness = 0.0) : Card(card.getFigure(), card.getColor()), id(id), usefulnessForOpponent(usefulness) {}
	
	int getId() const { return id; }
	
	double& Usefulness() { return usefulnessForOpponent; }
	
	double getUsefulnessForOpponent() const { return usefulnessForOpponent; }
};

class AIOppCard: public Card
{
	bool isReal;
	
	friend std::ostream& operator<< ( std::ostream& wyjscie, const AIOppCard& card )
	{
		wyjscie << "Figure: ";
		switch(card.getFigure())
		{
			case Card_Figure::None : wyjscie << DIM_RED << "None" << COLOR_RESET;
			break;
			case Card_Figure::A : wyjscie << "A";
			break;
			case Card_Figure::TWO : wyjscie << "TWO";
			break;
			case Card_Figure::THREE : wyjscie << "THREE";
			break;
			case Card_Figure::FOUR : wyjscie << "FOUR";
			break;
			case Card_Figure::FIVE : wyjscie << "FIVE";
			break;
			case Card_Figure::SIX : wyjscie << "SIX";
			break;
			case Card_Figure::SEVEN : wyjscie << "SEVEN";
			break;
			case Card_Figure::EIGHT : wyjscie << "EIGHT";
			break;
			case Card_Figure::NINE : wyjscie << "NINE";
			break;
			case Card_Figure::TEN : wyjscie << "TEN";
			break;
			case Card_Figure::JACK : wyjscie << "JACK";
			break;
			case Card_Figure::QUEEN : wyjscie << "QUEEN";
			break;
			case Card_Figure::KING : wyjscie << "KING";
			break;
			default : wyjscie << "Different";
		}
		wyjscie << ", Color: ";
		switch(card.getColor())
		{
			case Card_Color::None : wyjscie << DIM_RED << "None" << COLOR_RESET;
			break;
			case Card_Color::SPADE : wyjscie << "SPADE";
			break;
			case Card_Color::HEART : wyjscie << "HEART";
			break;
			case Card_Color::CLUB : wyjscie << "CLUB";
			break;
			case Card_Color::DIAMOND : wyjscie << "DIAMOND";
			break;
			default : wyjscie << DIM_RED << "Different" << COLOR_RESET;
		}
		wyjscie << ", real: " << card.getIsReal() << std::endl;
		return wyjscie;
	}
	
public:
	AIOppCard(Card_Figure figure = Card_Figure::None, Card_Color color = Card_Color::None, bool isReal = true) : Card(figure, color), isReal(isReal) {}
	
	AIOppCard(const Card& card, const bool& isReal) : Card(card.getFigure(), card.getColor()), isReal(isReal) {}
	
	int getIsReal() const { return isReal; }
};

struct Difference
{
	std::list< AICard > added;
	std::list< AICard > removed;
};

struct Comparation
{
  bool operator()(const AICard& a, const AICard& b)
  {
    return a.getFigure() < b.getFigure();
  }
};

class Sequence
{
	std::multiset < AICard, Comparation > sequence;
	
	friend std::ostream& operator<< ( std::ostream& wyjscie, const Sequence& sequence )
	{
		for(const AICard& card : sequence)
			wyjscie << card;
		return wyjscie;
	}
	
public:
	Sequence() {}
	
	Sequence(std::multiset < AICard, Comparation >& cards, const int& minSequenceLen = MIN_SEQ_LEN)
	{
		CreateSequence(cards, minSequenceLen);
	}
	
	bool CreateSequence(std::multiset < AICard, Comparation >& cards, const int& minSequenceLen = MIN_SEQ_LEN)
	{
		std::multiset < AICard, Comparation > sequence;
		std::multiset < AICard, Comparation >::iterator nextCheckingIt = cards.begin();
		Card_Color color = Card_Color::None;
		Card_Figure figure = Card_Figure::None;
		for (std::multiset < AICard, Comparation >::iterator it = cards.begin(); it != cards.end(); ++it)
		{
			//inicjalizacja
			if (color == Card_Color::None && figure == Card_Figure::None)
			{
				color = it->getColor();
				figure = it->getFigure();
				nextCheckingIt = it;
			}
			//sprawdzanie
			if (it->getColor() == color)
				if(it->getFigure() == figure)
				{
					if(nextCheckingIt == std::prev(it))//pomijanie sąsiedniej pasującej karty do sekwencji, przy ponowynym przeszukaniu
						nextCheckingIt = it;
					sequence.insert(*it);
					if(++figure > Card_Figure::KING) color = Card_Color::None;//przejdź do dodawania sekwencji
				}
				else
					if(sequence.empty() || *it != *std::prev(sequence.end()))//pomiń taką samą kartę
						color = Card_Color::None;//przejdź do dodawania sekwencji
						
			//dodawanie sekwencji
			if (color == Card_Color::None || it == std::prev(cards.end()))
				if(sequence.size() >= minSequenceLen)//dodawanie sekwencji
				{
					this->sequence = sequence;//dodaj sekwencję
					for (const AICard& card : sequence)//usuń sekwencję z kart
					{
						std::multiset < AICard, Comparation >::const_iterator itf = std::find(cards.cbegin(), cards.cend(), card);
						if (itf != cards.cend())
							cards.erase(itf);
					}
					return true;
				}
				else//reset
				{
					if (figure > Card_Figure::KING) break;//optymalizacja wyszukiwania
					sequence.clear();
					color = Card_Color::None;
					figure = Card_Figure::None;
					it = nextCheckingIt;
				}
		}
		return false;
	}
	
	bool AddCard(const AICard& card)
	{
		bool fits = false;
		if (fits = CheckFitting(card))
			sequence.insert(card);
		return fits;
	}
	
	bool CheckFitting(const AICard& card) const
	{
		if (sequence.empty()) return false;
		if (sequence.cbegin()->getFigure() > Card_Figure::A)//sprawdzanie na początku
			if (AICard(--sequence.cbegin()->getFigure(), sequence.cbegin()->getColor()) == card)
				return true;
		if (std::prev(sequence.cend())->getFigure() < Card_Figure::KING)//sprawdzanie na końcu
			if (AICard(++std::prev(sequence.cend())->getFigure(), std::prev(sequence.cend())->getColor()) == card)
				return true;
		return false;
	}
	
	bool FindCard(const AICard& card) const
	{
		if (!sequence.empty() && find (sequence.cbegin(), sequence.cend(), card) != sequence.cend())
			return true;
		return false;
	}
	
	bool CheckCorrectness(const int& minSequenceLen = MIN_SEQ_LEN) const
	{
		if (sequence.empty() || sequence.size() < minSequenceLen) return false;
		Card_Color color = sequence.cbegin()->getColor();
		Card_Figure figure = sequence.cbegin()->getFigure();
		for(const AICard& card : sequence)
		{
			if (figure > Card_Figure::KING) return false;
			if (card.getColor() == color && card.getFigure() == figure)
				++figure;
		}
		return true;
	}
	
	std::multiset < AICard, Comparation >::iterator begin()
	{
		return sequence.begin();
	}
	
	std::multiset < AICard, Comparation >::iterator end()
	{
		return sequence.end();
	}
	
	std::multiset < AICard, Comparation >::const_iterator begin() const
	{
		return sequence.cbegin();
	}
	
	std::multiset < AICard, Comparation >::const_iterator end() const
	{
		return sequence.cend();
	}
	
	std::multiset < AICard, Comparation >::size_type size() const noexcept
	{
		return sequence.size();
	}
};

// struct Comparation2
// {
// 	bool operator()(const Sequence& a, const Sequence& b)
// 	{
// 		return a.size() < b.size();
// 	}
// };

class SequencesOfCards
{
	std::list< Sequence > sequences;
	
	friend std::ostream& operator<< ( std::ostream& wyjscie, const SequencesOfCards& sequences )
	{
		int l = 0;
		for(const Sequence& seq : sequences)
			wyjscie << "Sequence: " << ++l << std::endl << seq;
		return wyjscie;
	}
	
public:
	SequencesOfCards() {}
	
	SequencesOfCards(const std::list< Sequence >& sequences, const int& minSequenceLen = MIN_SEQ_LEN)
	{
		for(const Sequence& seq : sequences)
			if(seq.CheckCorrectness(minSequenceLen))
				this->sequences.push_back(seq);
	}
	
	SequencesOfCards(std::multiset < AICard, Comparation >& cards, const int& minSequenceLen = MIN_SEQ_LEN)
	{
		CreateSequences(cards, minSequenceLen);
	}
	
	std::list< Sequence >::size_type CreateSequences(std::multiset < AICard, Comparation >& cards, const int& minSequenceLen = MIN_SEQ_LEN)
	{
		sequences.clear();
		for(int i = 0; i < cards.size() / minSequenceLen; ++i)
		{
			Sequence seq;
			if(seq.CreateSequence(cards, minSequenceLen))
				this->sequences.push_back(seq);
			else
				break;
		}
		return this->sequences.size();
	}
	
	bool CheckFitting(const AICard& card) const
	{
		for(const Sequence& seq : sequences)
			if(seq.CheckFitting(card))
				return true;
		return false;
	}
	
	bool AddCard(const AICard& card)
	{
		for(Sequence& seq : sequences)
			if(seq.AddCard(card))
				return true;
		return false;
	}
	
	bool FindCard(const AICard& card) const
	{
		for(const Sequence& seq : sequences)
			if(seq.FindCard(card))
				return true;
		return false;
	}
	
	std::list< Sequence >::iterator begin()
	{
		return sequences.begin();
	}
	
	std::list< Sequence >::iterator end()
	{
		return sequences.end();
	}
	
	std::list< Sequence >::const_iterator begin() const
	{
		return sequences.cbegin();
	}
	
	std::list< Sequence >::const_iterator end() const
	{
		return sequences.cend();
	}
	
	std::list< Sequence >::size_type size() const noexcept
	{
		return sequences.size();
	}
};

struct GroupComparation
{
	bool operator()(const AICard& a, const AICard& b)
	{
		return a.getColor() < b.getColor();
	}
};

class Group
{
	std::multiset < AICard, GroupComparation > group;
	
	friend std::ostream& operator<< ( std::ostream& wyjscie, const Group& group )
	{
		for(const AICard& card : group)
			wyjscie << card;
		return wyjscie;
	}
	
public:
	Group() {}
	
	Group(std::multiset < AICard, Comparation >& cards, const int& minGroupLen = MIN_GROUP_LEN)
	{
		CreateGroup(cards, minGroupLen);
	}
	
	bool CreateGroup(std::multiset < AICard, Comparation >& cards, const int& minGroupLen = MIN_GROUP_LEN)
	{
		std::multiset < AICard, GroupComparation > group;
		std::multiset < AICard, GroupComparation >::iterator nextCheckingIt = group.begin();
		Card_Figure figure = Card_Figure::None;
		for (std::multiset < AICard, Comparation >::iterator it = cards.begin(); it != cards.end(); ++it)
		{
			//inicjalizacja
			if (figure == Card_Figure::None)
			{
				figure = it->getFigure();
				nextCheckingIt = it;
				group.insert(*it);
			}
			//sprawdzanie
			if (it->getFigure() == figure)
			{
				if (find (group.cbegin(), group.cend(), *it) == group.cend())
				{
					if (nextCheckingIt == std::prev(it))//pomijanie sąsiedniej pasującej karty do grupy, przy ponowynym przeszukaniu
						nextCheckingIt = it;
					group.insert(*it);
					if(group.size() >= COLORS_COUNT) figure = Card_Figure::None;
				}
			}
			else
				figure = Card_Figure::None;
			//dodawanie grupy
			if (figure == Card_Figure::None || it == std::prev(cards.end()))
				if (group.size() >= minGroupLen)//dodawanie grupy
				{
					this->group = group;//dodaj grupę
					for (const AICard& card : group)//usuń grupę z kart
					{
						std::multiset < AICard, Comparation >::iterator itf = std::find(cards.cbegin(), cards.cend(), card);
						if(itf != cards.cend())
							cards.erase(itf);
					}
					return true;
				}
				else//reset
				{
					group.clear();
					figure = Card_Figure::None;
					it = nextCheckingIt;
				}
		}
		return false;
	}
	
	bool AddCard(const AICard& card)
	{
		bool fits = false;
		if(fits = CheckFitting(card))
			group.insert(card);
		return fits;
	}
	
	bool CheckFitting(const AICard& card) const
	{
		for (const AICard& cardg : group)
			if (cardg.getFigure() == card.getFigure() && cardg.getColor() != card.getColor())
			return true;
		return false;
	}
	
	bool FindCard(const AICard& card) const
	{
		if (!group.empty() && find (group.cbegin(), group.cend(), card) != group.cend())
			return true;
		return false;
	}
	
	bool CheckCorrectness(const int& minGroupLen = MIN_GROUP_LEN) const
	{
		if (group.empty() || group.size() < minGroupLen) return false;
		Card_Figure figure = group.cbegin()->getFigure();
		for (const AICard& card : group)
			if (card.getFigure() != figure) return false;
		return true;
	}
	
	std::multiset < AICard, GroupComparation >::iterator begin()
	{
		return group.begin();
	}
	
	std::multiset < AICard, GroupComparation >::iterator end()
	{
		return group.end();
	}
	
	std::multiset < AICard, GroupComparation >::const_iterator begin() const
	{
		return group.cbegin();
	}
	
	std::multiset < AICard, GroupComparation >::const_iterator end() const
	{
		return group.cend();
	}
	
	std::multiset < AICard, GroupComparation >::size_type size() const noexcept
	{
		return group.size();
	}
};

class GroupsOfCards
{
	std::list< Group > groups;
	
	friend std::ostream& operator<< ( std::ostream& wyjscie, const GroupsOfCards& groups )
	{
		int l = 0;
		for (const Group& group : groups)
			wyjscie << "Group: " << ++l << std::endl << group << std::endl;
		return wyjscie;
	}
	
public:
	GroupsOfCards() {}
	
	GroupsOfCards(const std::list< Group >& groups, const int& minGroupLen = MIN_GROUP_LEN)
	{
		for (const Group& group : groups)
			if (group.CheckCorrectness(minGroupLen))
				this->groups.push_back(group);
	}
	
	GroupsOfCards(std::multiset < AICard, Comparation >& cards, const int& minGroupLen = MIN_GROUP_LEN)
	{
		CreateGroups(cards, minGroupLen);
	}
	
	std::list< Group >::size_type CreateGroups(std::multiset < AICard, Comparation >& cards, const int& minGroupLen = MIN_GROUP_LEN)
	{
		groups.clear();
		for (int i = 0; i < cards.size() / minGroupLen; ++i)
		{
			Group group;
			if(group.CreateGroup(cards, minGroupLen))
				this->groups.push_back(group);
			else
				break;
		}
		return this->groups.size();
	}
	
	bool CheckFitting(const AICard& card) const
	{
		for (const Group& group : groups)
			if(group.CheckFitting(card))
				return true;
		return false;
	}
	
	bool AddCard(const AICard& card)
	{
		for (Group& group : groups)
			if(group.AddCard(card))
				return true;
		return false;
	}
	
	bool FindCard(const AICard& card) const
	{
		for (const Group& group : groups)
			if(group.FindCard(card))
				return true;
		return false;
	}
	
	std::list< Group >::iterator begin()
	{
		return groups.begin();
	}
	
	std::list< Group >::iterator end()
	{
		return groups.end();
	}
	
	std::list< Group >::const_iterator begin() const
	{
		return groups.cbegin();
	}
	
	std::list< Group >::const_iterator end() const
	{
		return groups.cend();
	}
	
	std::list< Group >::size_type size() const noexcept
	{
		return groups.size();
	}
};

static struct UsefulnessForOpponentComparation
{
	bool operator()(const AICard& a, const AICard& b)
	{
		return a.getUsefulnessForOpponent() < b.getUsefulnessForOpponent();
	}
} UsefulnessForOpponentComparation;

class ArtificialIntelligence :public TableObserver
{
  IGameControl & gameControl;
  enum class TypeOfTurn {BEGIN, MY_TAKE, MY_PUT, OPP_TAKE};
  TypeOfTurn lastTurn;
//   StackType lastAction;
  Card lastStackCard, lastTaken, lastPut;
  std::multiset < AICard, Comparation > ungruppedCards;//niepogrupowane karty
  std::list < Card > cardDeck;//talia kart
  std::multiset < AICard, Comparation > cards;//karty do przeszukania
  SequencesOfCards sequences;//znalezione sekwencje
  GroupsOfCards groups;//znalezione grupy
  std::list < AIOppCard > opponentCards;//karty przeciwnika
  
	bool firstTime = true;
	int newCardID = 0, turnNumber = 0;
  
 public:
	 ArtificialIntelligence(IGameControl & gameControlInterface) : gameControl(gameControlInterface), lastTurn(TypeOfTurn::BEGIN), lastTaken(Card(Card_Figure::None, Card_Color::None)), lastPut(Card(Card_Figure::None, Card_Color::None))
	{
		for (const auto& figure : Card_Figure())
			for (const auto& color : Card_Color())
				cardDeck.emplace_back(figure, color);
		srand (time (NULL));
//     for(int figure = 1; figure <= static_cast<int>(Card_Figure::KING); ++figure)
//       for(int color = 1; color <= static_cast<int>(Card_Color::DIAMOND); ++color)
// 		  cardDeck.push_back( AICard( static_cast<Card_Figure> (figure), static_cast<Card_Color> (color)));
	}
  
	void onUpdate(TableSnapshot const&);
	
	void RandomCardsForOpponent(TableSnapshot const& tableSnapshot);
	
	void CopyMyCardFromTableSnapshot(TableSnapshot const& tableSnapshot);
	
	void EndGame(TableSnapshot const& tableSnapshot) const;
	
	std::multiset < AICard, Comparation > ReturnUnusedCards(const std::multiset < AICard, Comparation >& unusedForSeqs, const std::multiset < AICard, Comparation >& unusedForGroups) const
	{
		std::multiset < AICard, Comparation > unusedCards;
		if (!unusedForGroups.empty())
			for (const AICard& card : unusedForSeqs)
				if (find (unusedForGroups.cbegin(), unusedForGroups.cend(), card) != unusedForGroups.cend())
					unusedCards.insert(card);
		return unusedCards;
	}
	
	void CheckUsefulnessForOpponent();
	
	double CalculateFactorForGroup(const Card_Figure& figure) const;
	
	int CalculateFactorForSequence(const AICard& card) const;
	
	Difference ComputeDifference(std::vector<Card> playerCards) const;
	
	int CreateSeqsGroupsUnusedCards();
	
	std::multiset < AICard, Comparation > CreateSeqsGroupsUnusedCards(std::multiset < AICard, Comparation > sourceCards, const int& minSequenceLen = MIN_SEQ_LEN, const int& minGroupLen = MIN_GROUP_LEN) const;
	
	bool DecisionToTakeTheCard(const AICard& card) const;
	
	int RemoveCardsFromDeck();
	
	bool RemoveCardFromDeck(const Card& card);
	
	int CreateCards();
	
	void ShowGroups() const
	{
		std::cout << groups;
	}
	
	void ShowSeqs() const
	{
		std::cout << sequences;
	}
	
	int ShowCards() const;
	
	int ShowCardDeck() const;
	
	int GetOpponentKnownCardsNumber() const;
};
