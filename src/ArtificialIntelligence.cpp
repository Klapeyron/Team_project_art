#include "ArtificialIntelligence.hpp"

void ArtificialIntelligence::onUpdate(TableSnapshot const& tableSnapshot)
{
	if (tableSnapshot.enemyEndsGame && lastTurn != TypeOfTurn::END_GAME)
	{
		TouchMyCards();
		std::cerr << "AI Info: Przeciwnik pierwszy zakończył grę." << " nr ruchu: " << turnNumber << std::endl;
		lastTurn = TypeOfTurn::END_GAME;
	}
	if (lastTurn == TypeOfTurn::END_GAME) return;
	if (lastTurn == TypeOfTurn::BEGIN)
	{
		ShowSettingsInfo();
		RandomCardsForOpponent(tableSnapshot);
	}
	++turnNumber;
	
	if (tableSnapshot.myMove)//from tableSnapshot must be updated: playerCards, stackCard
	{
		Difference difference;
		if (myFirstMove)
			CopyMyCardFromTableSnapshot(tableSnapshot);
		else
		{
			difference = ApplyDifference(tableSnapshot);
			if (difference.added.size() && difference.removed.size())
				std::cerr << "AI Error: Jednocześnie wzięto: " << difference.added.size() << " i wydano: " << difference.removed.size() << " kart-ę/y!" << " nr ruchu: " << turnNumber << std::endl;
			if (lastTurn == TypeOfTurn::OPP_TAKE && myLastTurn != TypeOfTurn::MY_FOLD)
			{
				if (find (difference.removed.cbegin(), difference.removed.cend(), lastPut) == difference.removed.cend())
					std::cerr << "AI Error: Nie wydano odpowiedniej karty: " << lastPut.getFigure() << " " << lastPut.getColor() << "! nr ruchu: " << turnNumber << std::endl;
				if (difference.removed.size() != 1)
				{
					std::cerr << "AI Error: Wydano: " << difference.removed.size() << " kart(ę/y) zamiast jednej!" << " nr ruchu: " << turnNumber << std::endl;
					std::cerr << DIM_GREEN << difference.removed << COLOR_RESET;
				}
			}
		}
		if (lastTurn == TypeOfTurn::OPP_TAKE)
		{
			if (lastStackCard == tableSnapshot.stackCard)
				std::cerr << "AI Error: Przeciwnik nie wydał karty! Karta na stosie jest ta sama co poprzednio: " << lastStackCard.getFigure() << " " << lastStackCard.getColor() << "! nr ruchu: " << turnNumber << std::endl;
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
						std::cerr << "AI Error: Nie usunięto karty przeciwnika!" << " nr ruchu: " << turnNumber << std::endl;
				}
				if (opponentCards.size() != PLAYER_CARDS_COUNT)
					std::cerr << "AI Error: Nieprawidłowa ilość kart przeciwnika: " << opponentCards.size() << " nr ruchu: " << turnNumber << std::endl;
			}
		}
		
		CreateCollectionsAndUnusedCards();
		if (difference.added.empty())
		{
			if (lastTurn == TypeOfTurn::MY_TAKE)
				std::cerr << "AI Error: Dwa razy pod rząd taki sam ruch graczaaa: " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			else
				if (lastTurn != TypeOfTurn::OPP_FOLD && lastTurn != TypeOfTurn::BEGIN && lastTurn != TypeOfTurn::OPP_TAKE)
					std::cerr << "AI Error: Naruszenie spójności gry: MY_TAKE po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			WarningCardsCount(tableSnapshot, false);
			
			if (DecisionToTakeTheCard(tableSnapshot.stackCard))
			{
				gameControl.pickCardFromStack();
				lastTaken = tableSnapshot.stackCard;
				lastTurn = TypeOfTurn::MY_TAKE;
			}
			else
			{
				lastTaken = AICard(Card_Figure::None, Card_Color::None);
				if (myFirstMove)
				{
					if (ungruppedCards.empty())
					{
						gameControl.pickCardFromStack();
						lastTaken = tableSnapshot.stackCard;
						lastTurn = TypeOfTurn::MY_TAKE;
					}
					else
					{
						gameControl.pressPass();
						lastTurn = TypeOfTurn::MY_FOLD;
					}
				}
				else
				{
					gameControl.pickCardFromHiddenStack();
					lastTurn = TypeOfTurn::MY_TAKE;
				}
			}
		}
		else //jeśli wzięto kartę
		{
			WarningTurnCorrectness(TypeOfTurn::MY_PUT);
			if (lastTaken != Card(Card_Figure::None, Card_Color::None) && find (difference.added.cbegin(), difference.added.cend(), lastTaken) == difference.added.cend())
				std::cerr << "AI Error: Nie znaleziono pobranej karty: " << lastTaken.getFigure() << " " << lastTaken.getColor() << "! nr ruchu: " << turnNumber << std::endl;
			if (difference.added.size() != 1)
			{
				std::cerr << "AI Error: Pobrano: " << difference.added.size() << " kart(ę/y) zamiast jednej!" << " nr ruchu: " << turnNumber << std::endl;
				std::cerr << DIM_BLUE << difference.added << COLOR_RESET;
			}
			else
				std::cerr << "AI Info: Pobrano: " << DIM_BLUE << difference.added.begin()->getFigure() << " " << difference.added.begin()->getColor() << COLOR_RESET << ". nr ruchu: " << turnNumber << std::endl;
			WarningCardsCount(tableSnapshot, true);
			
			for (const Card& card : difference.added)
				if (!RemoveCardFromDeck(card) && turnNumber != 2 && turnNumber != 4)
					std::cerr << "AI Error: Nie znaleziono usuwanej karty: " << card.getFigure() << " " << card.getColor() <<  " w talii!" << " nr ruchu: " << turnNumber << std::endl;
			ReplaceOpponentCard(difference.added);
			ReduceGruppedCards();
			ShowSeqs();
			ShowGroups();
			lastTurn = TypeOfTurn::MY_PUT;
			if (ungruppedCards.size())
			{
				CheckUsefulnessForOpponent();
				if (ungruppedCards.size() == 1 || (!isFulOptionActivated && UngruppedCardsPoints() <= END_GAME_POINTS))// warunek końca gry
					lastTurn = EndGame(tableSnapshot);
				else
				{
					const AICard cardToPut = ChooseCardToBeReallyPut();
					gameControl.throwMyCard(lastStackCard = lastPut = (Card)cardToPut);
					if (cardToPut == AICard(Card_Figure::None, Card_Color::None))
						std::cerr << "AI Error: Wydano pustą kartę!" << " nr ruchu: " << turnNumber << std::endl;
				}
			}
			else
			{
				const Card cardToPut = PossibleToEndGameForNoUngruppedCards();
				if (cardToPut == Card(Card_Figure::None, Card_Color::None))
				{
					gameControl.throwMyCard(lastStackCard = lastPut = *std::min_element(cards.begin(), cards.end(), UsefulnessForOpponentComparation));
					std::cerr << "AI Warrning: Wydano kartę i stracono kolekcję!" << " nr ruchu: " << turnNumber << std::endl;
				}
				else
				{
					lastStackCard = lastPut = cardToPut;
					lastTurn = EndGame(tableSnapshot);
				}
				if (turnNumber != 2 && turnNumber != 4)
					std::cerr << "AI Error: Brak niepogrupowanych kart! Gra powinna być zakończona wcześniej!" << " nr ruchu: " << turnNumber << std::endl;
			}
		}
		myLastTurn = lastTurn;
		myFirstMove = false;
	}
	else//from tableSnapshot must be updated: enemyTookCard, stackCard
	{
		if (tableSnapshot.enemyTookCard) //jeśli wziął kartę
		{
			if (lastTurn == TypeOfTurn::OPP_TAKE)
				std::cerr << "AI Error: Dwa razy pod rząd taki sam ruch przeciwnika: " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			else
				if (lastTurn != TypeOfTurn::OPP_FOLD && lastTurn != TypeOfTurn::OPP_BEFORE_TAKE)
					std::cerr << "AI Error: Naruszenie spójności gry: OPP_TAKE po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
				else
					if (lastStackCard == tableSnapshot.stackCard)
						opponentCards.emplace_back(*std::next(cardDeck.begin(), rand() % cardDeck.size()), false);
			if (lastStackCard != tableSnapshot.stackCard) //jeśli wziął kartę z odkrytego stosu
			{
				if(lastStackCard == Card(Card_Figure::None, Card_Color::None))
					std::cerr << "AI Error: Przeciwnik wziął pustą kartę z odkrytego stosu!" << " nr ruchu: " << turnNumber << std::endl;
				else
					opponentCards.emplace_back(lastStackCard, true);
				std::cerr << "AI Info: Przeciwnik pobrał: " << GRAY << lastStackCard.getFigure() << " " << lastStackCard.getColor() << COLOR_RESET << ". nr ruchu: " << turnNumber << std::endl;
				lastStackCard = tableSnapshot.stackCard;
			}
			lastTurn = TypeOfTurn::OPP_TAKE;
		}
		else
		{
			if (lastTurn == TypeOfTurn::BEGIN || lastTurn == TypeOfTurn::MY_FOLD)
			{
// 				lastStackCard = tableSnapshot.stackCard;
				lastTurn = TypeOfTurn::OPP_FOLD;
			}
			else
			{
				if (lastTurn == TypeOfTurn::OPP_BEFORE_TAKE)
					std::cerr << "AI Error: Dwa razy pod rząd ruch przeciwnika przed pobraniem karty!" << " nr ruchu: " << turnNumber << std::endl;
				else
					if (lastPut != tableSnapshot.stackCard)
						std::cerr << "AI Error: Nie wydano odpowiedniej karty: " << lastPut.getFigure() << " " << lastPut.getColor() << "! Na stosie znajduje się: " << tableSnapshot.stackCard.getFigure() << " " << tableSnapshot.stackCard.getColor() << "! nr ruchu: " << turnNumber << std::endl;
					else
						std::cerr << "AI Info: Wydano: " << DIM_GREEN << lastPut.getFigure() << " " << lastPut.getColor() << COLOR_RESET << ". nr ruchu: " << turnNumber << std::endl;
				lastTurn = TypeOfTurn::OPP_BEFORE_TAKE;
			}
			lastStackCard = tableSnapshot.stackCard;
		}
	}
}

void ArtificialIntelligence::ShowSettingsInfo()
{
	std::cerr << "Parametry algorytmu: " << std::endl;
	std::cerr << "Tryb zakończenia gry bez niepogrupowanych kart: " << (isFulOptionActivated ? "\e[0;32mwłączony" : "\e[0;31mwyłączony") << COLOR_RESET << "." << std::endl;
	std::cerr << "Próg przydatności dla przeciwnika decydujący o wydaniu największej figury: " << putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSingleCards << "% (Pojedyncze karty), " << putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSmallCollection << "% (Małe kolekcje)" << std::endl;
	std::cerr << "Próg przydatności dla przeciwnika, gdy wydawana jest największa figura: " << theHeaviestCardUsfullnessForOppLowerThenThresholdForSingleCards << "% (Pojedyncze karty), " << theHeaviestCardUsfullnessForOppLowerThenThresholdForSmallCollection << "% (Małe kolekcje)" << std::endl;
	std::cerr << "Tolerancja progu przy wydawaniu najmniej przydatnej karty dla przeciwnika, gdy ta karta należy do małej grupy i sekwencji: " << usefulnessForOpponentTolerance << "%" << std::endl << std::endl;
}

void ArtificialIntelligence::WarningTurnCorrectness(TypeOfTurn const& currentTurn)
{
	if (lastTurn == currentTurn)
		std::cerr << "AI Error: Dwa razy pod rząd taki sam ruch gracza: " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
	else
		if (lastTurn != static_cast<TypeOfTurn>(static_cast<int>(currentTurn) - 1))
			std::cerr << "AI Error: Naruszenie spójności gry: " << currentTurn << " po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
}

void ArtificialIntelligence::WarningCardsCount(TableSnapshot const& tableSnapshot, const bool& isTaken)
{
	if (tableSnapshot.playerCards.size() != PLAYER_CARDS_COUNT + (isTaken ? 1 : 0))
	{
		std::cerr << "AI Error: Nieprawidłowa ilość kart gracza: " << tableSnapshot.playerCards.size() << " zamiast: " << PLAYER_CARDS_COUNT + (isTaken ? 1 : 0) << "! nr ruchu: " << turnNumber << std::endl;
		for(const Card& card : tableSnapshot.playerCards)
			std::cerr << card;
	}
}

int ArtificialIntelligence::UngruppedCardsPoints() const
{
	int points = 0;
	for (const AICard& ungCard : ungruppedCards)
		points += static_cast<int>(ungCard.getFigure());
	if (points <=0)
		std::cerr << "AI Error: Błąd przy liczeniu punktów dla warunku zakończenia gry. Suma: " << points << "! nr ruchu: " << turnNumber << std::endl;
	return points;
}

void ArtificialIntelligence::ReplaceOpponentCard(const std::list< AICard >& addedDifference)
{
	for (const Card& card : addedDifference)
	{
		for (std::list < AIOppCard >::const_iterator cit = opponentCards.begin(); cit != opponentCards.end(); ++cit)
			if (card == *cit)
				if(!cit->getIsReal())
				{
					opponentCards.erase(cit);
					if (!cardDeck.empty())
					{
						for (const Card& card : cardDeck)
							if (find (opponentCards.cbegin(), opponentCards.cend(), card) == opponentCards.cend())
							{
								opponentCards.emplace_back(card, false);
								break;
							}
					}
					else
						std::cerr << "AI Error: Nie zamieniono karty przeciwnikowi! " << ". nr ruchu: " << turnNumber << std::endl;
					break;
				}
				else
					std::cerr << "AI Error: Przeciwnik ma taką samą kartę co gracz: " << *cit << ". nr ruchu: " << turnNumber << std::endl;
	}
}

AICard ArtificialIntelligence::ChooseCardToBeReallyPut() const
{
	SequencesOfCards sequences2;
	GroupsOfCards groups2;
	std::multiset < AICard, Comparation > ungruppedCardsFromSmallCollection(CreateCollectionsAndUnusedCards(ungruppedCards, MIN_SEQ_LEN - 1, MIN_GROUP_LEN - 1, sequences2, groups2));
	AICard cardToPut;
	if (!ungruppedCardsFromSmallCollection.empty())// dla pojedynczych kart
		cardToPut = ChooseCardToPut(ungruppedCardsFromSmallCollection, putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSingleCards, theHeaviestCardUsfullnessForOppLowerThenThresholdForSingleCards);
	else// dla podwójnych kart
	{
		cardToPut = ChooseCardToPut(ungruppedCards, putTheHeaviestCardIfUsefullnessForOppIsLowerThenThresholdForSmallCollection, theHeaviestCardUsfullnessForOppLowerThenThresholdForSmallCollection);
		//optimalize CardToPut
		std::list < AICard > cardsWithTheSameFigure;
		for (const AICard& card : ungruppedCards)
			if (card.getFigure() == cardToPut.getFigure())
				cardsWithTheSameFigure.push_back(card);
			std::list < AICard >::const_iterator minUseCard = std::min_element(cardsWithTheSameFigure.cbegin(), cardsWithTheSameFigure.cend(), UsefulnessForOpponentComparation);
		if (minUseCard != cardsWithTheSameFigure.cend())
		{
// 			if (GetOpponentKnownCardsNumber() / (double)PLAYER_CARDS_COUNT >= 50.0 && CardCountWithTheSameFigureInCardDeck() >= 1)
			cardToPut = *minUseCard;
			if (sequences2.FindCard(cardToPut) && groups2.FindCard(cardToPut))
			{
				cardsWithTheSameFigure.erase(minUseCard);
				std::list < AICard >::const_iterator minUseCard2 = std::min_element(cardsWithTheSameFigure.cbegin(), cardsWithTheSameFigure.cend(), UsefulnessForOpponentComparation);
				if (minUseCard2 != cardsWithTheSameFigure.cend())
					if (minUseCard2->getUsefulnessForOpponent() - cardToPut.getUsefulnessForOpponent() < usefulnessForOpponentTolerance && !sequences2.FindCard(*minUseCard2) && !groups2.FindCard(*minUseCard2))
						cardToPut = *minUseCard2;
			}
// 			std::cerr << cardToPut << " nr ruchu: " << turnNumber << std::endl;
		}
		else
			std::cerr << "AI Error: Brak minimalnej karty!" << " nr ruchu: " << turnNumber << std::endl;
	}
	return cardToPut;
}

AICard ArtificialIntelligence::ChooseCardToPut(std::multiset < AICard, Comparation > sourceCards, const double& tryToThrowTheGreatestFigureThreshold, const double& ifTheUsefulnessIsLessThanThreshold) const
{
	AICard cardToPut = *std::min_element(sourceCards.begin(), sourceCards.end(), UsefulnessForOpponentComparation);// karta najmniej korzystna dla przeciwnika
	if (cardToPut.getUsefulnessForOpponent() < tryToThrowTheGreatestFigureThreshold)
	{// wybierz kartę o największej figurze o przydatności mniejszej niż próg
		bool b = false;
		if (GetOpponentKnownCardsNumber() / (double)PLAYER_CARDS_COUNT >= 50.0) b = true;
		std::multiset < AICard, Comparation >::const_reverse_iterator crit = std::find_if(sourceCards.crbegin(), sourceCards.crend(), [& ifTheUsefulnessIsLessThanThreshold](const AICard& card){ return card.getUsefulnessForOpponent() < ifTheUsefulnessIsLessThanThreshold; });// znajdowanie karty o największej figurze z przydatnością mniejszą od [próg] %
		if(crit != sourceCards.crend())
			cardToPut = *crit;
	}
	return cardToPut;
}

Difference ArtificialIntelligence::ApplyDifference(TableSnapshot const& tableSnapshot)
{
	Difference difference(ComputeDifference(tableSnapshot.playerCards));
// 	for (const Card& karta : tableSnapshot.playerCards)
// 		std::cout << GRAY<< karta<<COLOR_RESET;
// 	std::cout << DIM_PURPLE<< difference.removed<<COLOR_RESET;
// 	std::cout << CYAN<< difference.added<<COLOR_RESET;
	for (const AICard& card : difference.removed)
		cards.erase( std::find_if( cards.begin(), cards.end(), [& card](const AICard& card2){ return card2.getFigure() == card.getFigure() && card2.getColor() == card.getColor(); } ) ); 
	for (const AICard& card : difference.added)
		cards.emplace(card, newCardID++);
	if (tableSnapshot.playerCards.size() != cards.size())
		std::cerr << "AI Error: Nie zastosowano właściwie różnicy kart!" << " nr ruchu: " << turnNumber << std::endl;
// 	ShowCards();
	return difference;
}

void ArtificialIntelligence::RandomCardsForOpponent(TableSnapshot const& tableSnapshot)
{
	std::vector < Card > cardDeck;
	for (const Card_Figure& figure : Card_Figure())
		for (const Card_Color& color : Card_Color())
			cardDeck.emplace_back(figure, color);
	cardDeck.erase(std::find(cardDeck.cbegin(), cardDeck.cend(), tableSnapshot.stackCard));
	for (const Card& card : tableSnapshot.playerCards)
		cardDeck.erase(std::find(cardDeck.cbegin(), cardDeck.cend(), card));
	while(opponentCards.size() < PLAYER_CARDS_COUNT)
	{
		Card card(cardDeck[rand() % cardDeck.size()]);
		if (std::find(opponentCards.cbegin(), opponentCards.cend(), card) == opponentCards.cend())
			opponentCards.emplace_back(card, false);
	}
}

void ArtificialIntelligence::CopyMyCardFromTableSnapshot(TableSnapshot const& tableSnapshot)
{
	cards.clear();
	for (const Card& card : tableSnapshot.playerCards)
		cards.emplace(card, newCardID++);
	if (!RemoveCardFromDeck(tableSnapshot.stackCard))
		std::cerr << "AI Error: Nie znaleziono usuwanej karty ze stosu w talii kart!" << " nr ruchu: " << turnNumber << std::endl;
	RemoveCardsFromDeck();
}

ArtificialIntelligence::TypeOfTurn ArtificialIntelligence::EndGame(TableSnapshot const& tableSnapshot)
{
	gameControl.endGame();
	if (ungruppedCards.size())
		gameControl.throwMyCard(*ungruppedCards.begin());
	else
	{
		cards.erase(std::find (cards.cbegin(), cards.cend(), lastPut));
		CreateCollectionsAndUnusedCards();
		ReduceGruppedCards();
		gameControl.throwMyCard(lastPut);
	}
	TouchMyCards();
	return TypeOfTurn::END_GAME;
}

Card ArtificialIntelligence::PossibleToEndGameForNoUngruppedCards() const
{
	Card cardToPut(Card_Figure::None, Card_Color::None);
	SequencesOfCards::const_iterator sit = std::find_if(sequences.begin(), sequences.end(), [](const Sequence& seq){ return seq.size() > MIN_SEQ_LEN; });
	if (sit != sequences.end())
		cardToPut = (Card)*sit->begin();
	else
	{
		GroupsOfCards::const_iterator git = std::find_if(groups.begin(), groups.end(), [](const Group& group){ return group.size() > MIN_GROUP_LEN; });
		if (git != groups.end())
			cardToPut = (Card)*git->begin();
	}
	return cardToPut;
}

void ArtificialIntelligence::TouchMyCards() const
{
	for (const Sequence& seq : sequences)
		gameControl.touchCard(*seq.begin());
	for (const Group& group : groups)
		gameControl.touchCard(*group.begin());
	gameControl.pressOK();
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

AICard ArtificialIntelligence::MinUsefulnessForOpponentCardCollection(const std::vector < AICard >& sourceCards) const
{
	std::vector < AICard > ungruppedCardsTmp(sourceCards.begin(), sourceCards.end());
	for (AICard& ungCard : ungruppedCardsTmp)
	{
		ungCard.Usefulness() = (50.0 / (COLORS_COUNT - 1)) * CalculateFactorForGroup(ungCard.getFigure());
		ungCard.Usefulness() += (50.0 / 12.0) * CalculateFactorForSequence(ungCard);
	}
	return *std::min_element(ungruppedCardsTmp.cbegin(), ungruppedCardsTmp.cend(), UsefulnessForOpponentComparation);
}

double ArtificialIntelligence::CalculateFactorForGroup(const Card_Figure& figure) const
{
	int count = 0;
	double weight = 0.0;
	for (const Card_Color& color : Card_Color())
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
		std::cerr << "AI Error: Ilość kart dla jednej figury nie powinna był większa niż: " << COLORS_COUNT << std::endl;
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
	for (const AIOppCard& oppCard : opponentCards)
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
	if (minDistance <= 0) std::cerr << "AI Error: Nieprawidłowy dystans figur." << " Karta: " << card << " - " << minCard << ". Dystans: " << minDistance << std::endl;
	if (minDistance2 <= 0) std::cerr << "AI Error: Nieprawidłowy dystans figur." << " Karta: " << card << ". Dystans2: " << minDistance2 << std::endl;
	if (minDistance3 <= 0) std::cerr << "AI Error: Nieprawidłowy dystans figur." << " Karta: " << card << ". Dystans3: " << minDistance3 << std::endl;
	if(minDistance > 0)
		weight += (13.0 - minDistance) / 6.0 * ((double)isTargetCardReal + 1.0);
	if (minDistance2 > 0)
		weight += (13.0 - minDistance2) / 6.0 * ((double)isTargetCard2Real + 1.0);
	if (minDistance3 > 0)
		weight += (13.0 - minDistance3) / 6.0 * ((double)isTargetCard3Real + 1.0);
	return weight;
}

Difference ArtificialIntelligence::ComputeDifference(std::vector<Card> playerCards)
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

void ArtificialIntelligence::ReduceGruppedCards()
{
	for (SequencesOfCards::const_iterator foundSeq = sequences.begin(); foundSeq != sequences.end();)
	{
		bool isSeqRemoved = false;
		for (Sequence::iterator cardFromSeq = foundSeq->begin(); cardFromSeq != foundSeq->end(); ++cardFromSeq)
			if (cardFromSeq->getId() > 0)
			{
				std::list< Group >::iterator foundGroup = groups.FindCard(cardFromSeq->getId());
				if (foundGroup != groups.end())
				{
					std::multiset < AICard, Comparation > sequence(foundSeq->ReturnCopy());
					sequence.erase( std::find_if( sequence.begin(), sequence.end(), [& cardFromSeq](const AICard& card){ return card.getId() == cardFromSeq->getId(); } ) );
					std::multiset < AICard, Comparation > sequence2(sequence);
					std::multiset < AICard, Comparation > group;
					for (const AICard& card : foundGroup->ReturnCopy())
						group.insert(card);
					group.erase( std::find_if( group.begin(), group.end(), [& cardFromSeq](const AICard& card){ return card.getId() == cardFromSeq->getId(); } ) );
					std::multiset < AICard, Comparation > group2(group);
					Sequence seq(sequence);
					Group gr(group);
					if (gr.CheckCorrectness())
					{
						std::cerr << DIM_YELLOW << "AI Info: Zredukowano grupę: " << group2.begin()->getFigure() << COLOR_RESET << std::endl;
						groups.erase(foundGroup);
						groups.AddGroup(group2);
					}
					else
						if (!seq.CheckCorrectness() && !gr.CheckCorrectness())
						{
							int seqPoints = 0, grPoints = 0;
							for (const AICard& card : sequence2)// strata usunięcie sekwencji
								if (groups.FindCard(card.getId()) == groups.end())
									seqPoints += static_cast<int>(card.getFigure());
							for (const AICard& card : group2)// strata usunięcie grupy
								if (sequences.FindCard(card.getId()) == sequences.end())
									grPoints += static_cast<int>(card.getFigure());
							
							if (seqPoints < 0 || grPoints < 0)
								std::cerr << "AI Error: Nieprawidłowa suma punktów: sekwencja: " << seqPoints << " grupa: " << grPoints << "!" << std::endl;
							else
								if (seqPoints < grPoints)
								{
									std::cerr << DIM_YELLOW << "AI Info: Usunięto sekwencję: " << foundSeq->begin()->getColor() << COLOR_RESET << std::endl;
									for (Sequence::const_iterator cardFromS = foundSeq->begin(); cardFromS != foundSeq->end(); ++cardFromS)
										if (cardFromS != cardFromSeq)// pomiń aktualną kartę
											if (groups.FindCard(cardFromS->getId()) == groups.end())
												ungruppedCards.insert(*cardFromS);
									foundSeq = sequences.erase(foundSeq);
									isSeqRemoved = true;
									break;
								}
								else
								{
									std::cerr << DIM_YELLOW << "AI Info: Usunięto grupę: " << foundGroup->begin()->getFigure() << COLOR_RESET << std::endl;
									for (Group::const_iterator cardFromG = foundGroup->begin(); cardFromG != foundGroup->end(); ++cardFromG)
										if (sequences.FindCard(cardFromG->getId()) == sequences.end())
											ungruppedCards.insert(*cardFromG);
									groups.erase(foundGroup);
								}
						}
						else
							if (seq.CheckCorrectness() && !gr.CheckCorrectness())
							{
								std::multiset < AICard, Comparation > sequence3(sequence);
								Sequence seq2(sequence);// układanie drugiej sekwencji na prawo od podziału
								int seqPoints = 0, grPoints = 0;
								for (const AICard& card : sequence)// strata za zredukowania sekwencji
									seqPoints += static_cast<int>(card.getFigure());
								for (const AICard& card : group2)// strata usunięcie grupy
									grPoints += static_cast<int>(card.getFigure());
								if (seqPoints < grPoints)
								{
									std::cerr << DIM_YELLOW << "AI Info: Zredukowano sekwencję: " << foundSeq->begin()->getColor() << COLOR_RESET << std::endl;
									for (const AICard& card : sequence)
									ungruppedCards.insert(card);
									foundSeq = sequences.erase(foundSeq);// usunięcie całej sekwencji
									sequences.AddSequence(sequence2);// dodanie zredukowanej sekwencji
									if (seq2.CheckCorrectness()) sequences.AddSequence(sequence3);
									isSeqRemoved = true;
									break;
								}
								else
								{
									std::cerr << DIM_YELLOW << "AI Info: Usunięto grupę: " << foundGroup->begin()->getFigure() << COLOR_RESET << std::endl;
									for (Group::const_iterator cardFromG = foundGroup->begin(); cardFromG != foundGroup->end(); ++cardFromG)
										if (sequences.FindCard(cardFromG->getId()) == sequences.end())
											ungruppedCards.insert(*cardFromG);
									groups.erase(foundGroup);
								}
							}
				}
			}
			else
				std::cerr << "AI Error: Redukcja kart - id karty równe zero!" << std::endl;
		if(!isSeqRemoved) ++foundSeq;
	}
	if (!CheckCorrectnessOfUnusedCards())
		std::cerr << "AI Error: Niepogrupowane karty zawierają się w kolekcji kart!" << " nr ruchu: " << turnNumber << std::endl;
}

bool ArtificialIntelligence::CheckCorrectnessOfUnusedCards()
{
	for (const AICard& ungCard : ungruppedCards)
		if (sequences.FindCard(ungCard) || groups.FindCard(ungCard))
			return false;
	return true;
}

int ArtificialIntelligence::CreateCollectionsAndUnusedCards()
{
	std::multiset < AICard, Comparation > cardsForSeqs(cards), cardsForGroups(cards);
	int count = sequences.CreateSequences(cardsForSeqs) + groups.CreateGroups(cardsForGroups);
	ungruppedCards.clear();
	ungruppedCards = ReturnUnusedCards(cardsForSeqs, cardsForGroups);
	return count;
}

std::multiset < AICard, Comparation > ArtificialIntelligence::CreateCollectionsAndUnusedCards(std::multiset < AICard, Comparation > sourceCards, const int& minSequenceLen, const int& minGroupLen, SequencesOfCards& sequences, GroupsOfCards& groups) const
{
	std::multiset < AICard, Comparation > cardsForSeqs(sourceCards), cardsForGroups(sourceCards);
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
		if (RemoveCardFromDeck(card))
			++count;
	return count;
}

bool ArtificialIntelligence::RemoveCardFromDeck(const Card& card)
{
	std::list < Card >::const_iterator cardToErase = std::find(cardDeck.cbegin(), cardDeck.cend(), card);
	if (cardToErase != cardDeck.cend())
	{
		cardDeck.erase(cardToErase);
		return true;
	}
	return false;
}

int ArtificialIntelligence::CardCountWithTheSameFigureInCardDeck(const Card& checkedCard) const
{
	int count = 0;
	for (const Card& card : cardDeck)
		if (card.getFigure() == checkedCard.getFigure())
			++count;
	return count;
}

void ArtificialIntelligence::GameInit()
{
	lastTurn = TypeOfTurn::BEGIN;
	myLastTurn = TypeOfTurn::BEGIN;
	lastStackCard = Card(Card_Figure::None, Card_Color::None);
	lastTaken = Card(Card_Figure::None, Card_Color::None);
	lastPut = Card(Card_Figure::None, Card_Color::None);
	ungruppedCards.clear();
	FillTheCardDeck();
	cards.clear();
	sequences.clear();
	groups.clear();
	opponentCards.clear();
	myFirstMove = true;
	newCardID = 1;
	turnNumber = 0;
}

int ArtificialIntelligence::ShowCards() const
{
	std::cout << "Player cards:" << std::endl;
	for (const AICard& card : cards)
		std::cout << card;
	return cards.size();
}

int ArtificialIntelligence::ShowCardDeck() const
{
	std::cout << "Cards on deck:" << std::endl;
	for (const Card& card : cardDeck)
		std::cout << card;
	return cardDeck.size();
}

int ArtificialIntelligence::GetOpponentKnownCardsNumber() const
{
	int count = 0;
	for (const AIOppCard& card : opponentCards)
		if (card.getIsReal())
			++count;
	return count;
}
