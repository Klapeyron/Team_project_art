#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <type_traits>
#include <iterator>
#include <sstream>

#include "IGameControl.hpp"
#include "TableObserver.hpp"
#include "CardOperators.hpp"

#define MIN_SEQ_LEN (3)
#define MIN_GROUP_LEN (3)
#define COLORS_COUNT (4)
#define PLAYER_CARDS_COUNT (10)
#define END_GAME_POINTS (11)

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
		wyjscie << card.getFigure() << " " << card.getColor() << ", Id: " << card.getId() << ", Usefulness: " << card.getUsefulnessForOpponent() << "%" << std::endl;
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
		wyjscie << card.getFigure() << " " << card.getColor() << ", Is real: " << card.getIsReal() << std::endl;
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

inline std::ostream& operator<< ( std::ostream& wyjscie, const std::list< AICard >& list )
{
	std::copy(list.begin(), list.end(), std::ostream_iterator< Card >(wyjscie));
	return wyjscie;
}

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
	
	typedef std::multiset < AICard, Comparation >::iterator iterator;
	typedef std::multiset < AICard, Comparation >::const_iterator const_iterator;
	
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
	
	bool FindCard(const int& cardId) const
	{
		if (!sequence.empty() && std::find_if( sequence.begin(), sequence.end(), [& cardId](const AICard& card){ return card.getId() == cardId; } ) != sequence.cend())
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
	
	std::multiset < AICard, Comparation > ReturnCopy() const
	{
		return sequence;
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
		for (int i = 0, maxCount = cards.size() / minSequenceLen; i < maxCount; ++i)
		{
			if (!AddSequence(cards, minSequenceLen))
				break;
		}
		return this->sequences.size();
	}
	
	typedef std::list< Sequence >::iterator iterator;
	typedef std::list< Sequence >::const_iterator const_iterator;
	
	bool CheckFitting(const AICard& card) const
	{
		for (const Sequence& seq : sequences)
			if (seq.CheckFitting(card))
				return true;
		return false;
	}
	
	bool AddSequence(std::multiset < AICard, Comparation >& cards, const int& minSequenceLen = MIN_SEQ_LEN)
	{
		Sequence seq;
		if (seq.CreateSequence(cards, minSequenceLen))
		{
			this->sequences.push_back(seq);
			return true;
		}
		return false;
	}
	
	bool AddCard(const AICard& card)
	{
		for (Sequence& seq : sequences)
			if (seq.AddCard(card))
				return true;
		return false;
	}
	
	bool FindCard(const AICard& card) const
	{
		for (const Sequence& seq : sequences)
			if (seq.FindCard(card))
				return true;
		return false;
	}
	
	std::list< Sequence >::iterator FindCard(const int& cardId)
	{
		for (std::list < Sequence >::iterator it = sequences.begin(); it != sequences.end(); ++it)
			if (it->FindCard(cardId))
				return it;
		return sequences.end();
	}
	
	iterator erase(const_iterator position)
	{
		return sequences.erase(position);
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
	
	void clear() noexcept
	{
		sequences.clear();
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
	
	typedef std::multiset < AICard, GroupComparation >::iterator iterator;
	typedef std::multiset < AICard, GroupComparation >::const_iterator const_iterator;
	
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
	
	bool FindCard(const int& cardId) const
	{
		if (!group.empty() && std::find_if( group.begin(), group.end(), [& cardId](const AICard& card){ return card.getId() == cardId; } ) != group.cend())
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
	
	std::multiset < AICard, GroupComparation > ReturnCopy() const
	{
		return group;
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
		for (int i = 0, maxCount = cards.size() / minGroupLen; i < maxCount; ++i)
		{
			if (!AddGroup(cards, minGroupLen))
				break;
		}
		return this->groups.size();
	}
	
	typedef std::list< Group >::iterator iterator;
	typedef std::list< Group >::const_iterator const_iterator;
	
	bool CheckFitting(const AICard& card) const
	{
		for (const Group& group : groups)
			if (group.CheckFitting(card))
				return true;
		return false;
	}
	
	bool AddGroup(std::multiset < AICard, Comparation >& cards, const int& minGroupLen = MIN_GROUP_LEN)
	{
		Group group;
		if (group.CreateGroup(cards, minGroupLen))
		{
			this->groups.push_back(group);
			return true;
		}
		return false;
	}
	
	bool AddCard(const AICard& card)
	{
		for (Group& group : groups)
			if (group.AddCard(card))
				return true;
		return false;
	}
	
	bool FindCard(const AICard& card) const
	{
		for (const Group& group : groups)
			if (group.FindCard(card))
				return true;
		return false;
	}
	
	std::list< Group >::iterator FindCard(const int& cardId)
	{
		for (std::list < Group >::iterator it = groups.begin(); it != groups.end(); ++it)
			if (it->FindCard(cardId))
				return it;
		return groups.end();
	}
	
	iterator erase(const_iterator position)
	{
		return groups.erase(position);
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
	
	void clear() noexcept
	{
		groups.clear();
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
	enum class TypeOfTurn : int { BEGIN = 0, MY_TAKE, MY_PUT, MY_FOLD, OPP_BEFORE_TAKE, OPP_TAKE, OPP_FOLD, END_GAME };
	friend inline std::ostream& operator<<(::std::ostream& os, const TypeOfTurn& turnType)
	{
		if (turnType == TypeOfTurn::BEGIN) return os << "BEGIN";
		if (turnType == TypeOfTurn::MY_TAKE) return os << "MY_TAKE";
		if (turnType == TypeOfTurn::MY_PUT) return os << "MY_PUT";
		if (turnType == TypeOfTurn::MY_FOLD) return os << "MY_FOLD";
		if (turnType == TypeOfTurn::OPP_BEFORE_TAKE) return os << "OPP_BEFORE_TAKE";
		if (turnType == TypeOfTurn::OPP_TAKE) return os << "OPP_TAKE";
		if (turnType == TypeOfTurn::OPP_FOLD) return os << "OPP_FOLD";
		if (turnType == TypeOfTurn::END_GAME) return os << "END_GAME";
		return os << "None";
	}
	TypeOfTurn lastTurn, myLastTurn;
	Card lastStackCard, lastTaken, lastPut;
	std::multiset < AICard, Comparation > ungruppedCards;//niepogrupowane karty
	std::list < Card > cardDeck;//talia kart
	std::multiset < AICard, Comparation > cards;//karty do przeszukania
	SequencesOfCards sequences;//znalezione sekwencje
	GroupsOfCards groups;//znalezione grupy
	std::list < AIOppCard > opponentCards;//karty przeciwnika
	bool myFirstMove = true, isFulOptionActivated = false;
	int newCardID = 1, turnNumber = 0;
	double usefulnessForOpponentTolerance = 5.0, putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSingleCards = 30.0, theHeaviestCardUsfullnessForOppLowerThenThresholdForSingleCards = 40.0, putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSmallCollection = 20.0, theHeaviestCardUsfullnessForOppLowerThenThresholdForSmallCollection = 35.0;

	void FillTheCardDeck()
	{
		cardDeck.clear();
		for (const Card_Figure& figure : Card_Figure())
			for (const Card_Color& color : Card_Color())
				cardDeck.emplace_back(figure, color);
	}
	
	void ShowSettingsInfo();
	
	void WarningTurnCorrectness(TypeOfTurn const& currentTurn);
	
	void WarningCardsCount(TableSnapshot const& tableSnapshot, const bool& isTaken);
	
	int UngruppedCardsPoints() const;
	
	void ReplaceOpponentCard(const std::list< AICard >& addedDifference);
	
	AICard ChooseCardToBeReallyPut() const;
	
	AICard ChooseCardToPut(std::multiset < AICard, Comparation > sourceCards, const double& tryToThrowTheGreatestFigureThreshold, const double& ifTheUsefulnessIsLessThanThreshold) const;
	
	Difference ApplyDifference(TableSnapshot const& tableSnapshot);
	
	void RandomCardsForOpponent(TableSnapshot const& tableSnapshot);
	
	void CopyMyCardFromTableSnapshot(TableSnapshot const& tableSnapshot);
	
	TypeOfTurn EndGame(TableSnapshot const& tableSnapshot);
	
	Card PossibleToEndGameForNoUngruppedCards() const;
	
	void TouchMyCards() const;
	
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
	
	AICard MinUsefulnessForOpponentCardCollection(const std::vector < AICard >& sourceCards) const;
	
	double CalculateFactorForGroup(const Card_Figure& figure) const;
	
	int CalculateFactorForSequence(const AICard& card) const;
	
	Difference ComputeDifference(std::vector<Card> playerCards);
	
	void ReduceGruppedCards();
	bool CheckCorrectnessOfUnusedCards();
	
	int CreateCollectionsAndUnusedCards();
	
	std::multiset < AICard, Comparation > CreateCollectionsAndUnusedCards(std::multiset < AICard, Comparation > sourceCards, const int& minSequenceLen, const int& minGroupLen, SequencesOfCards& sequences, GroupsOfCards& groups) const;
	
	bool DecisionToTakeTheCard(const AICard& card) const;
	
	int RemoveCardsFromDeck();
	
	bool RemoveCardFromDeck(const Card& card);
	
	int CardCountWithTheSameFigureInCardDeck(const Card& checkedCard) const;
	
public:
	ArtificialIntelligence(IGameControl & gameControlInterface, const bool& fulOptionActivate = false, const double& usefulnessForOpponentTolerance = 5.0, const double& putTheHeaviestCardIfUsefullnessIsLowerThenThFSiC = 30.0, const double& theHeaviestCardUsfullnessLowerThenThFSiC = 40.0, const double& putTheHeaviestCardIfUsefullnessIsLowerThenThFSmC = 20.0, const double& theHeaviestCardUsfullnessLowerThenThFSmC = 35.0) : gameControl(gameControlInterface), isFulOptionActivated(fulOptionActivate)
	{
		GameInit();
		SetUsefulnessForOpponentTolerance(usefulnessForOpponentTolerance);
		SetThForPutTheHeaviestCardIfUsefullnessIsLowerThenFSiC(putTheHeaviestCardIfUsefullnessIsLowerThenThFSiC);
		SetThForTheHeaviestCardUsfullnessLowerThenFSiC(theHeaviestCardUsfullnessLowerThenThFSiC);
		SetThForPutTheHeaviestCardIfUsefullnessIsLowerThenFSmC(putTheHeaviestCardIfUsefullnessIsLowerThenThFSmC);
		SetThForTheHeaviestCardUsfullnessLowerThenFSmC(theHeaviestCardUsfullnessLowerThenThFSmC);
		FillTheCardDeck();
		srand (time (NULL));
		//     	for(int figure = 1; figure <= static_cast<int>(Card_Figure::KING); ++figure)
		// 			for(int color = 1; color <= static_cast<int>(Card_Color::DIAMOND); ++color)
		// 				cardDeck.emplace_back(static_cast<Card_Figure> (figure), static_cast<Card_Color> (color));
	}
	
	void onUpdate(TableSnapshot const&);
	
	void GameInit();
	
	void SetFulOption(const bool& isActive)
	{
		isFulOptionActivated = isActive;
	}
	
	double SetUsefulnessForOpponentTolerance(const double& newUsefulnessForOpponentTolerance)
	{
		if (newUsefulnessForOpponentTolerance >= 0.0 && newUsefulnessForOpponentTolerance <= 100.0)
			usefulnessForOpponentTolerance = newUsefulnessForOpponentTolerance;
		return usefulnessForOpponentTolerance;
	}
	
	double SetThForPutTheHeaviestCardIfUsefullnessIsLowerThenFSiC(const double& putTheHeaviestCardIfUsefullnessIsLowerThenThFSiC)
	{
		if (putTheHeaviestCardIfUsefullnessIsLowerThenThFSiC >= 0.0 && putTheHeaviestCardIfUsefullnessIsLowerThenThFSiC <= 100.0)
			putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSingleCards = putTheHeaviestCardIfUsefullnessIsLowerThenThFSiC;
		return putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSingleCards;
	}
	
	double SetThForTheHeaviestCardUsfullnessLowerThenFSiC(const double& theHeaviestCardUsfullnessLowerThenThFSiC)
	{
		if (theHeaviestCardUsfullnessLowerThenThFSiC >= 0.0 && theHeaviestCardUsfullnessLowerThenThFSiC <= 100.0)
			theHeaviestCardUsfullnessForOppLowerThenThresholdForSingleCards = theHeaviestCardUsfullnessLowerThenThFSiC;
		return theHeaviestCardUsfullnessForOppLowerThenThresholdForSingleCards;
	}
	
	double SetThForPutTheHeaviestCardIfUsefullnessIsLowerThenFSmC(const double& putTheHeaviestCardIfUsefullnessIsLowerThenThFSmC)
	{
		if (putTheHeaviestCardIfUsefullnessIsLowerThenThFSmC >= 0.0 && putTheHeaviestCardIfUsefullnessIsLowerThenThFSmC <= 100.0)
			putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSmallCollection = putTheHeaviestCardIfUsefullnessIsLowerThenThFSmC;
		return putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSmallCollection;
	}
	
	double SetThForTheHeaviestCardUsfullnessLowerThenFSmC(const double& theHeaviestCardUsfullnessLowerThenThFSmC)
	{
		if (theHeaviestCardUsfullnessLowerThenThFSmC >= 0.0 && theHeaviestCardUsfullnessLowerThenThFSmC <= 100.0)
			theHeaviestCardUsfullnessForOppLowerThenThresholdForSmallCollection = theHeaviestCardUsfullnessLowerThenThFSmC;
		return theHeaviestCardUsfullnessForOppLowerThenThresholdForSmallCollection;
	}
	
	void ShowSeqs() const
	{
		std::cout << sequences;
	}
	
	void ShowGroups() const
	{
		std::cout << groups;
	}
	
	int ShowCards() const;
	
	int ShowCardDeck() const;
	
	int GetOpponentKnownCardsNumber() const;
};
