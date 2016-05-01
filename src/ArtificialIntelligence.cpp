#include "ArtificialIntelligence.hpp"

void ArtificialIntelligence::onUpdate(TableSnapshot const& tableSnapshot)
{
	++turnNumber;
	if(tableSnapshot.myMove)//from TableSnapshot use: tableSnapshot.playerCards, stackCard
	{
		if(firstTime)
		{
			cards.clear();
			for (const Card& karta : tableSnapshot.playerCards)
				cards.insert(AICard (karta, newCardID++));
			RemoveCardsFromDeck();
		}
		else
		{
			if (tableSnapshot.myMove == lastTurn) std::cerr << "AI: Dwa razy pod rząd ruch gracza!" << " nr ruchu: " << turnNumber << std::endl;
			Difference difference = ComputeDifference(tableSnapshot.playerCards);
			if (find (difference.removed.cbegin(), difference.removed.cend(), lastPut) == difference.removed.cend())
				std::cerr << "AI: Nie wydano odpowiedniej karty!" << " nr ruchu: " << turnNumber << std::endl;
			if(difference.removed.size() != 1)
				std::cerr << "AI: Wydano: " << difference.removed.size() << " zamiast jednej!" << " nr ruchu: " << turnNumber << std::endl;
			if (lastTaken != Card(Card_Figure::None, Card_Color::None) && find (difference.added.cbegin(), difference.added.cend(), lastTaken) == difference.added.cend())
				std::cerr << "AI: Nie znaleziono pobranej karty!" << " nr ruchu: " << turnNumber << std::endl;
			if(difference.added.size() != 1)
				std::cerr << "AI: Pobrano: " << difference.added.size() << " zamiast jednej!" << " nr ruchu: " << turnNumber << std::endl;
			for (const Card& karta : difference.added)
				cards.insert(karta);
		}
		CreateSeqsGroupsUnusedCards();
		if (DecisionToTakeTheCard(tableSnapshot.stackCard))
		{
			gameControl.pickCardFromStack();
			lastTaken = tableSnapshot.stackCard;
		}
		else
		{
			gameControl.pickCardFromHiddenStack();
			lastTaken = AICard(Card_Figure::None, Card_Color::None);
		}
		if(ungruppedCards.size())
		{
			CheckUsefulnessForOpponent();
			gameControl.throwMyCard(lastPut = *std::min_element(ungruppedCards.begin(), ungruppedCards.end(), UsefulnessForOpponentComparation));
			lastStackCard = lastPut;
			if(ungruppedCards.size() == 1)
				EndGame(tableSnapshot);
		}
		else
			if(firstTime)
				EndGame(tableSnapshot);
			else
				std::cerr << "AI: Brak niepogrupowanych kart! Nie wykonano ruchu!" << " nr ruchu: " << turnNumber << std::endl;
	}
	else//from TableSnapshot use: opponentTookCardFromHiddenStack, stackCard
	{
		if(lastStackCard == tableSnapshot.stackCard)
			std::cerr << "AI: Gracz nie wydał karty! Karta na stosie jest ta sama co poprzednio." << " nr ruchu: " << turnNumber << std::endl;
		if (tableSnapshot.opponentTookCardFromHiddenStack)
			opponentCards.push_back(lastStackCard);
		RemoveCardFromDeck(tableSnapshot.stackCard);
		lastStackCard = tableSnapshot.stackCard;
		if (tableSnapshot.myMove == lastTurn) std::cerr << "AI: Dwa razy pod rząd ruch przeciwnika!" << " nr ruchu: " << turnNumber << std::endl;
	}
	lastTurn = tableSnapshot.myMove;
	firstTime = false;
}

void ArtificialIntelligence::EndGame(TableSnapshot const& tableSnapshot)
{
	gameControl.endGame();
	for(const Sequence& seq : sequences)
		gameControl.touchCard(*seq.begin());
	for(const Group& group : groups)
		gameControl.touchCard(*group.begin());
}

void ArtificialIntelligence::CheckUsefulnessForOpponent()
{
	std::vector < AICard > ungruppedCardsTmp(ungruppedCards.begin(), ungruppedCards.end());
	for (AICard& ungCard : ungruppedCardsTmp)
	{
		int count = ColorCount(ungCard.getFigure()) % COLORS_COUNT;
		ungCard.Usefulness() = (50.0 / COLORS_COUNT / ILOSC_TALII) * count;
		int distance = DistanceFromFigure(ungCard);
		if (distance < 0) std::cerr << "AI: Błąd przy liczeniu dystansu." << " Dystans: " << distance << std::endl;
		ungCard.Usefulness() += (50.0 / 12.0) * (13.0 - distance);
	}
	ungruppedCards.clear();
	ungruppedCards.insert(ungruppedCardsTmp.begin(), ungruppedCardsTmp.end());
}

int ArtificialIntelligence::ColorCount(const Card_Figure& figure)
{
	int count = 0;
	for (const auto& color : Card_Color())
		if(find (opponentCards.cbegin(), opponentCards.cend(), AICard (figure, color)) != opponentCards.cend())
			++count;
	return count;
}

int ArtificialIntelligence::DistanceFromFigure(const AICard& card)
{
	int minDistance = 13;
	for (const auto& oppCard : opponentCards)
		if (oppCard.getColor() == card.getColor())
		{
			int distance = abs(static_cast<int>(card.getFigure()) - static_cast<int>(oppCard.getFigure()));
			if(distance < minDistance)
				minDistance = distance;
		}
	return minDistance;
}

Difference ArtificialIntelligence::ComputeDifference(std::vector<Card> playerCards)
{
	Difference difference;
	for (const Card& karta : cards)
	{
		std::vector<Card>::const_iterator itf = find (playerCards.cbegin(), playerCards.cend(), karta);
		if (itf == playerCards.cend())
			difference.removed.push_back(karta);
		else
			playerCards.erase(itf);
	}
	difference.added.insert(difference.added.end(), playerCards.cbegin(), playerCards.cend());
	return difference;
}

int ArtificialIntelligence::CreateSeqsGroupsUnusedCards()
{
	std::multiset < AICard, Comparation > cardsForSeqs(cards), cardsForGroups(cards);
	int count = sequences.CreateSequences(cardsForSeqs) + groups.CreateGroups(cardsForGroups);
	ungruppedCards.clear();
	ungruppedCards = ReturnUnusedCards(cardsForSeqs, cardsForGroups);
	return count;
}

bool ArtificialIntelligence::DecisionToTakeTheCard(const AICard& card)
{
	if (sequences.CheckFitting(card) || groups.CheckFitting(card))
		return true;
	std::multiset < AICard, Comparation > unusedCards(ungruppedCards);
	unusedCards.insert(card);
	Sequence seq;
	Group group;
	if(seq.CreateSequence(unusedCards) || group.CreateGroup(unusedCards))
		return true;
	return false;
}

int ArtificialIntelligence::RemoveCardsFromDeck()
{
	int count = 0;
	for (const AICard& card : cards)
	{
		if(RemoveCardFromDeck(card))
			++count;
	}
	return count;
}

bool ArtificialIntelligence::RemoveCardFromDeck(const AICard& card)
{
	for(std::list < AICard >::iterator it = cardDeck.begin(); it != cardDeck.end(); ++it)
		if(*it == card)
		{
			cardDeck.erase(it);
			return true;
		}
	return false;
}

int ArtificialIntelligence::CreateCards()
{
	cards.insert(AICard(Card_Figure::THREE, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::THREE, Card_Color::CLUB));
	cards.insert(AICard(Card_Figure::SEVEN, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::FOUR, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::TWO, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::KING, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::NINE, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::EIGHT, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::FIVE, Card_Color::HEART));
	cards.insert(AICard(Card_Figure::FIVE, Card_Color::DIAMOND));
	cards.insert(AICard(Card_Figure::FIVE, Card_Color::CLUB));
	cards.insert(AICard(Card_Figure::FOUR, Card_Color::HEART));
	cards.insert(AICard(Card_Figure::QUEEN, Card_Color::DIAMOND));
	cards.insert(AICard(Card_Figure::THREE, Card_Color::HEART));
	cards.insert(AICard(Card_Figure::FIVE, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::FIVE, Card_Color::SPADE));
	cards.insert(AICard(Card_Figure::KING, Card_Color::DIAMOND));
	cards.insert(AICard(Card_Figure::SEVEN, Card_Color::DIAMOND));
	cards.insert(AICard(Card_Figure::JACK, Card_Color::DIAMOND));
	cards.insert(AICard(Card_Figure::SEVEN, Card_Color::HEART));
	
	return cards.size();
}

int ArtificialIntelligence::ShowCards()
{
	std::cout << "Cards:" << std::endl;
	for (const AICard& card : cards)
		std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(card.getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(card.getColor()) << std::endl;
	return cards.size();
}

int ArtificialIntelligence::ShowCardDeck()
{
	std::cout << "AICard Deck:" << std::endl;
	for (const AICard& card : cardDeck)
		std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(card.getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(card.getColor()) << std::endl;
	return cardDeck.size();
}
