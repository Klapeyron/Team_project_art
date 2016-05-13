#include "ArtificialIntelligence.hpp"

void ArtificialIntelligence::onUpdate(TableSnapshot const& tableSnapshot)
{
	++turnNumber;
	if(lastTurn == TypeOfTurn::BEGIN)
		RandomCardsForOpponent(tableSnapshot);
	
	if(tableSnapshot.myMove)//from tableSnapshot use: playerCards, stackCard
	{
		Difference difference;
		if(firstTime)
			CopyMyCardFromTableSnapshot(tableSnapshot);
		else
		{
			difference = ComputeDifference(tableSnapshot.playerCards);
			for (const AICard& karta : difference.added)
				cards.insert(karta);
			for (const AICard& karta : difference.removed)
				cards.erase(karta);
			if (difference.added.size() && difference.removed.size())
				std::cerr << "AI: Jednocześnie wzięto: " << difference.added.size() << " i wydano: " << difference.removed.size() << " kart-ę/y!" << " nr ruchu: " << turnNumber << std::endl;
			if(lastTurn != TypeOfTurn::OPP_FOLD && lastTurn == TypeOfTurn::OPP_TAKE)
			{
				if (find (difference.removed.cbegin(), difference.removed.cend(), lastPut) == difference.removed.cend())
					std::cerr << "AI: Gracz nie wydał odpowiedniej karty: " << lastPut << "! nr ruchu: " << turnNumber << std::endl;
				if (difference.removed.size() != 1)
				{
					std::cerr << "AI: Gracz wydał: " << difference.removed.size() << " kart(ę/y) zamiast jednej!" << " nr ruchu: " << turnNumber << std::endl;
					for(const Card& card : difference.removed)
						std::cerr << DIM_GREEN << card << COLOR_RESET;
				}
			}
		}
		if(lastTurn != TypeOfTurn::OPP_FOLD && lastTurn == TypeOfTurn::OPP_TAKE)
		{
			if (lastStackCard == tableSnapshot.stackCard)
				std::cerr << "AI: Przeciwnik nie wydał karty! Karta na stosie jest ta sama co poprzednio: " << lastStackCard << "! nr ruchu: " << turnNumber << std::endl;
			else
			{
				std::list < AIOppCard >::const_iterator cit = std::find(opponentCards.cbegin(), opponentCards.cend(), AIOppCard(tableSnapshot.stackCard, true));
				if (cit != opponentCards.cend())
					opponentCards.erase(cit);
				else
				{
					std::list < AIOppCard >::const_iterator cit = std::find_if(opponentCards.begin(), opponentCards.end(), [](const AIOppCard& card){ return !card.getIsReal(); });
					if (cit != opponentCards.cend())
						opponentCards.erase(cit);
					else
						std::cerr << "AI: Nie usunięto karty przeciwnika!" << " nr ruchu: " << turnNumber << std::endl;
				}
			}
		}
		
		CreateSeqsGroupsUnusedCards();
		if (!difference.added.size())
		{
			if (lastTurn == TypeOfTurn::MY_TAKE)
				std::cerr << "AI: Dwa razy pod rząd taki sam ruch gracza: " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			else
				if (lastTurn != TypeOfTurn::OPP_FOLD && lastTurn != TypeOfTurn::BEGIN && lastTurn != TypeOfTurn::OPP_TAKE)
					std::cerr << "AI: Naruszenie spójności gry: MY_TAKE po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			if (tableSnapshot.playerCards.size() != PLAYER_CARDS_COUNT)
			{
				std::cerr << "AI: Nieprawidłowa ilość kart gracza: " << tableSnapshot.playerCards.size() << " zamiast: 10! nr ruchu: " << turnNumber << std::endl;
				for(const Card& card : tableSnapshot.playerCards)
					std::cerr << card;
			}
			if (DecisionToTakeTheCard(tableSnapshot.stackCard))
			{
				gameControl.pickCardFromStack();
				lastTaken = tableSnapshot.stackCard;
				if(firstTime)
					lastTurn = TypeOfTurn::MY_TAKE;
			}
			else
			{
				gameControl.pickCardFromHiddenStack();
				RemoveCardFromDeck(tableSnapshot.stackCard);
				lastTaken = AICard(Card_Figure::None, Card_Color::None);
			}
			if(!firstTime)
				lastTurn = TypeOfTurn::MY_TAKE;
		}
		else //jeśli wzięto kartę
		{
			if (lastTurn == TypeOfTurn::MY_PUT)
				std::cerr << "AI: Dwa razy pod rząd taki sam ruch gracza: " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			else
				if (lastTurn != TypeOfTurn::MY_TAKE)
					std::cerr << "AI: Naruszenie spójności gry: MY_PUT po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			if (lastTaken != Card(Card_Figure::None, Card_Color::None) && find (difference.added.cbegin(), difference.added.cend(), lastTaken) == difference.added.cend())
				std::cerr << "AI: Nie znaleziono pobranej karty: " << lastTaken << "! nr ruchu: " << turnNumber << std::endl;
			if (difference.added.size() != 1)
			{
				std::cerr << "AI: Gracz pobrał: " << difference.added.size() << " kart(ę/y) zamiast jednej!" << " nr ruchu: " << turnNumber << std::endl;
				for(const Card& card : difference.added)
					std::cerr << DIM_BLUE << card << COLOR_RESET;
			}
			else
				std::cerr << "AI: Gracz pobrał: " << DIM_BLUE << (Card)*difference.added.begin() << COLOR_RESET << ". nr ruchu: " << turnNumber << std::endl;
			if (tableSnapshot.playerCards.size() != PLAYER_CARDS_COUNT + 1)
			{
				std::cerr << "AI: Nieprawidłowa ilość kart gracza: " << tableSnapshot.playerCards.size() << " zamiast: 11! nr ruchu: " << turnNumber << std::endl;
				for(const Card& card : tableSnapshot.playerCards)
					std::cerr << card;
			}
			if (ungruppedCards.size())
			{
				CheckUsefulnessForOpponent();
				std::multiset < AICard, Comparation > ungruppedCardsTmp(CreateSeqsGroupsUnusedCards(ungruppedCards, MIN_SEQ_LEN - 1, MIN_GROUP_LEN - 1));
				AICard cardToPut;
				if (!ungruppedCardsTmp.empty())
				{
					cardToPut = *std::min_element(ungruppedCardsTmp.begin(), ungruppedCardsTmp.end(), UsefulnessForOpponentComparation);
					if (cardToPut.getUsefulnessForOpponent() < 30.0)
					{
						std::multiset < AICard, Comparation >::const_reverse_iterator crit = std::find_if(ungruppedCardsTmp.crbegin(), ungruppedCardsTmp.crend(), [](const AICard& card){ return card.getUsefulnessForOpponent() < 45.0; });// znajdowanie karty o największej figurze z przydatnością mniejszą od 45.0 %
						if(crit != ungruppedCardsTmp.crend())
							cardToPut = *crit;
					}
				}
				else
				{
					cardToPut = *std::min_element(ungruppedCards.begin(), ungruppedCards.end(), UsefulnessForOpponentComparation);
					if (cardToPut.getUsefulnessForOpponent() < 20.0)
					{
						std::multiset < AICard, Comparation >::const_reverse_iterator crit = std::find_if(ungruppedCards.crbegin(), ungruppedCards.crend(), [](const AICard& card){ return card.getUsefulnessForOpponent() < 40.0; });// znajdowanie karty o największej figurze z przydatnością mniejszą od 40.0 %
						if(crit != ungruppedCards.crend())
							cardToPut = *crit;
					}
				}
				gameControl.throwMyCard(lastPut = cardToPut);
				lastStackCard = lastPut;
				if(ungruppedCards.size() == 1)
					EndGame(tableSnapshot);
			}
			else
				if (firstTime)
					EndGame(tableSnapshot);
				else
					std::cerr << "AI: Brak niepogrupowanych kart! Nie wykonano ruchu!" << " nr ruchu: " << turnNumber << std::endl;
			lastTurn = TypeOfTurn::MY_PUT;
		}
		firstTime = false;
	}
	else//from tableSnapshot use: opponentTookCardFromHiddenStack, stackCard
	{
		if (tableSnapshot.opponentTookCardFromHiddenStack) //jeśli wziął kartę
		{
			if (lastStackCard != tableSnapshot.stackCard)
			{
				if(lastStackCard == Card(Card_Figure::None, Card_Color::None))
					std::cerr << "AI: Dodano pustą kartę do kart przeciwnika!" << " nr ruchu: " << turnNumber << std::endl;
				opponentCards.emplace_back(lastStackCard, true);
				lastStackCard = tableSnapshot.stackCard;
			}
			else
				RemoveCardFromDeck(tableSnapshot.stackCard);
		
			if (lastTurn == TypeOfTurn::OPP_TAKE)
				std::cerr << "AI: Dwa razy pod rząd taki sam ruch przeciwnika!" << " nr ruchu: " << turnNumber << std::endl;
			else
				if (lastTurn != TypeOfTurn::OPP_FOLD && lastTurn != TypeOfTurn::MY_PUT)
					std::cerr << "AI: Naruszenie spójności gry: OPP_TAKE po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			lastTurn = TypeOfTurn::OPP_TAKE;
		}
		else
			if (lastTurn == TypeOfTurn::BEGIN)
			{
				lastStackCard = tableSnapshot.stackCard;
				lastTurn = TypeOfTurn::OPP_FOLD;
			}
			else
				if (lastPut != tableSnapshot.stackCard)
					std::cerr << "AI: Gracz nie wydał odpowiedniej karty lub dwa razy ruch przeciwnika przed pobraniem karty!" << " nr ruchu: " << turnNumber << std::endl;
				else
					std::cerr << "AI: Gracz wydał: " << DIM_GREEN << lastPut << COLOR_RESET << ". nr ruchu: " << turnNumber << std::endl;
	}
}

void ArtificialIntelligence::RandomCardsForOpponent(TableSnapshot const& tableSnapshot)
{
	std::vector < Card > cardDeck;
	for (const auto& figure : Card_Figure())
		for (const auto& color : Card_Color())
			cardDeck.emplace_back(figure, color);
	cardDeck.erase(std::find(cardDeck.cbegin(), cardDeck.cend(), tableSnapshot.stackCard));
	for (const Card& card : tableSnapshot.playerCards)
		cardDeck.erase(std::find(cardDeck.cbegin(), cardDeck.cend(), card));
	while(opponentCards.size() < PLAYER_CARDS_COUNT)
	{
		Card card(cardDeck[rand() % cardDeck.size()]);
		if(std::find(opponentCards.cbegin(), opponentCards.cend(), card) == opponentCards.cend())
			opponentCards.emplace_back(card, false);
	}
}

void ArtificialIntelligence::CopyMyCardFromTableSnapshot(TableSnapshot const& tableSnapshot)
{
	cards.clear();
	for (const Card& karta : tableSnapshot.playerCards)
		cards.emplace(karta, newCardID++);
	RemoveCardsFromDeck();
}

void ArtificialIntelligence::EndGame(TableSnapshot const& tableSnapshot) const
{
	gameControl.endGame();
	gameControl.throwMyCard(*std::min_element(ungruppedCards.begin(), ungruppedCards.end(), UsefulnessForOpponentComparation));
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
		ungCard.Usefulness() = (50.0 / (COLORS_COUNT - 1)) * CalculateFactorForGroup(ungCard.getFigure());
		ungCard.Usefulness() += (50.0 / 12.0) * CalculateFactorForSequence(ungCard);
	}
	ungruppedCards.clear();
	ungruppedCards.insert(ungruppedCardsTmp.begin(), ungruppedCardsTmp.end());
}

double ArtificialIntelligence::CalculateFactorForGroup(const Card_Figure& figure) const
{
	int count = 0;
	double weight = 0.0;
	for (const auto& color : Card_Color())
	{
		std::list < AIOppCard >::const_iterator it = find (opponentCards.cbegin(), opponentCards.cend(), AICard (figure, color));
		if (it != opponentCards.cend())
		{
			++count;
			if(it->getIsReal())
				weight += 1.0;
			else
				weight += 0.5;
		}
	}
	if(count > COLORS_COUNT - 1)
	{
		weight = 0.0;
		std::cerr << "AI: Ilość kart dla jednej figury nie powinna był większa niż: " << COLORS_COUNT << std::endl;
	}
	return weight;
}

int ArtificialIntelligence::CalculateFactorForSequence(const AICard& card) const
{
	int minDistance = 13;
	bool isTargetCardReal = false;
	Card minCard;
	int minDistance2 = 13;
	bool isTargetCard2Real = false;
	double weight = 0.0;
	int minDistance3 = 13;
	bool isTargetCard3Real = false;
	for (const auto& oppCard : opponentCards)
		if (oppCard.getColor() == card.getColor())
		{
			int distance = abs(static_cast<int>(card.getFigure()) - static_cast<int>(oppCard.getFigure()));
			if (distance < minDistance)
			{
				minDistance3 = minDistance2;
				isTargetCard3Real = isTargetCard2Real;
				
				minDistance2 = minDistance;
				isTargetCard2Real = isTargetCardReal;
				
				minDistance = distance;
				isTargetCardReal = oppCard.getIsReal();
				minCard = oppCard;
			}
			else
				if (distance < minDistance2)
				{
					minDistance3 = minDistance2;
					isTargetCard3Real = isTargetCard2Real;
					
					minDistance2 = distance;
					isTargetCard2Real = oppCard.getIsReal();
				}
				else
					if (distance < minDistance3)
					{
						minDistance3 = distance;
						isTargetCard3Real = oppCard.getIsReal();
					}
		}
	if (minDistance <= 0) std::cerr << "AI: Nieprawidłowy dystans figur." << " Karta: " << card << " - " << minCard << ". Dystans: " << minDistance << std::endl;
	if (minDistance2 <= 0) std::cerr << "AI: Nieprawidłowy dystans figur." << " Karta: " << card << ". Dystans2: " << minDistance2 << std::endl;
	if (minDistance3 <= 0) std::cerr << "AI: Nieprawidłowy dystans figur." << " Karta: " << card << ". Dystans3: " << minDistance3 << std::endl;
	if(minDistance > 0)
		weight += (13.0 - minDistance) / 6.0 * ((double)isTargetCardReal + 1.0);
	if (minDistance2 > 0)
		weight += (13.0 - minDistance2) / 6.0 * ((double)isTargetCard2Real + 1.0);
	if (minDistance3 > 0)
		weight += (13.0 - minDistance3) / 6.0 * ((double)isTargetCard3Real + 1.0);
	return weight;
}

Difference ArtificialIntelligence::ComputeDifference(std::vector<Card> playerCards) const
{
	Difference difference;
	for (const Card& card : cards)
	{
		std::vector<Card>::const_iterator itf = find (playerCards.cbegin(), playerCards.cend(), card);
		if (itf == playerCards.cend())
			difference.removed.push_back(card);
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

std::multiset < AICard, Comparation > ArtificialIntelligence::CreateSeqsGroupsUnusedCards(std::multiset < AICard, Comparation > sourceCards, const int& minSequenceLen, const int& minGroupLen) const
{
	std::multiset < AICard, Comparation > cardsForSeqs(sourceCards), cardsForGroups(sourceCards);
	SequencesOfCards sequences;
	GroupsOfCards groups;
	int count = sequences.CreateSequences(cardsForSeqs, minSequenceLen) + groups.CreateGroups(cardsForGroups, minGroupLen);
	return ReturnUnusedCards(cardsForSeqs, cardsForGroups);
}

bool ArtificialIntelligence::DecisionToTakeTheCard(const AICard& card) const
{
	if (sequences.CheckFitting(card) || groups.CheckFitting(card))
		return true;
	std::multiset < AICard, Comparation > unusedCards(ungruppedCards);
	unusedCards.insert(card);
	Sequence seq;
	Group group;
	if (seq.CreateSequence(unusedCards) || group.CreateGroup(unusedCards))
		return true;
	return false;
}

int ArtificialIntelligence::RemoveCardsFromDeck()
{
	int count = 0;
	for (const AICard& card : cards)
	{
		if (RemoveCardFromDeck(card))
			++count;
	}
	return count;
}

bool ArtificialIntelligence::RemoveCardFromDeck(const Card& card)
{
	for (std::list < Card >::iterator it = cardDeck.begin(); it != cardDeck.end(); ++it)
		if (*it == card)
		{
			cardDeck.erase(it);
			return true;
		}
	return false;
}

int ArtificialIntelligence::CreateCards()
{
	cards.emplace(Card_Figure::THREE, Card_Color::SPADE);
	cards.emplace(Card_Figure::THREE, Card_Color::CLUB);
	cards.emplace(Card_Figure::SEVEN, Card_Color::SPADE);
	cards.emplace(Card_Figure::FOUR, Card_Color::SPADE);
	cards.emplace(Card_Figure::TWO, Card_Color::SPADE);
	cards.emplace(Card_Figure::KING, Card_Color::SPADE);
	cards.emplace(Card_Figure::NINE, Card_Color::SPADE);
	cards.emplace(Card_Figure::EIGHT, Card_Color::SPADE);
	cards.emplace(Card_Figure::FIVE, Card_Color::HEART);
	cards.emplace(Card_Figure::FIVE, Card_Color::DIAMOND);
	cards.emplace(Card_Figure::FIVE, Card_Color::CLUB);
	cards.emplace(Card_Figure::FOUR, Card_Color::HEART);
	cards.emplace(Card_Figure::QUEEN, Card_Color::DIAMOND);
	cards.emplace(Card_Figure::THREE, Card_Color::HEART);
	cards.emplace(Card_Figure::FIVE, Card_Color::SPADE);
	cards.emplace(Card_Figure::FIVE, Card_Color::SPADE);
	cards.emplace(Card_Figure::KING, Card_Color::DIAMOND);
	cards.emplace(Card_Figure::SEVEN, Card_Color::DIAMOND);
	cards.emplace(Card_Figure::JACK, Card_Color::DIAMOND);
	cards.emplace(Card_Figure::SEVEN, Card_Color::HEART);
	
	return cards.size();
}

int ArtificialIntelligence::ShowCards() const
{
	std::cout << "Cards:" << std::endl;
	for (const AICard& card : cards)
		std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(card.getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(card.getColor()) << std::endl;
	return cards.size();
}

int ArtificialIntelligence::ShowCardDeck() const
{
	std::cout << "AICard Deck:" << std::endl;
	for (const AICard& card : cardDeck)
		std::cout << "f: " << static_cast<std::underlying_type<Card_Figure>::type>(card.getFigure()) << " c: " << static_cast<std::underlying_type<Card_Color>::type>(card.getColor()) << std::endl;
	return cardDeck.size();
}

int ArtificialIntelligence::GetOpponentKnownCardsNumber() const
{
	int count = 0;
	for (const AIOppCard& card : opponentCards)
	{
		if (card.getIsReal())
			++count;
	}
	return count;
}
