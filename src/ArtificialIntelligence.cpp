#include "ArtificialIntelligence.hpp"

void ArtificialIntelligence::onUpdate(TableSnapshot const& tableSnapshot)
{
	if (lastTurn == TypeOfTurn::END_GAME) return;
	++turnNumber;
	if(lastTurn == TypeOfTurn::BEGIN)
		RandomCardsForOpponent(tableSnapshot);
	
	if(tableSnapshot.myMove)//from tableSnapshot must be updated: playerCards, stackCard
	{
		Difference difference;
		if(myFirstMove)
			CopyMyCardFromTableSnapshot(tableSnapshot);
		else
		{
			difference = ApplyDifference(tableSnapshot);
			if (difference.added.size() && difference.removed.size())
				std::cerr << "AI: Jednocześnie wzięto: " << difference.added.size() << " i wydano: " << difference.removed.size() << " kart-ę/y!" << " nr ruchu: " << turnNumber << std::endl;
			if(lastTurn == TypeOfTurn::OPP_TAKE && myLastTurn != TypeOfTurn::MY_FOLD)
			{
				if (find (difference.removed.cbegin(), difference.removed.cend(), lastPut) == difference.removed.cend())
					std::cerr << "AI: Nie wydano odpowiedniej karty: " << lastPut.getFigure() << " " << lastPut.getColor() << "! nr ruchu: " << turnNumber << std::endl;
				if (difference.removed.size() != 1)
				{
					std::cerr << "AI: Wydano: " << difference.removed.size() << " kart(ę/y) zamiast jednej!" << " nr ruchu: " << turnNumber << std::endl;
					std::cerr << DIM_GREEN << difference.removed << COLOR_RESET;
				}
			}
		}
		if(lastTurn == TypeOfTurn::OPP_TAKE)
		{
			if (lastStackCard == tableSnapshot.stackCard)
				std::cerr << "AI: Przeciwnik nie wydał karty! Karta na stosie jest ta sama co poprzednio: " << lastStackCard.getFigure() << " " << lastStackCard.getColor() << "! nr ruchu: " << turnNumber << std::endl;
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
				std::cerr << "AI: Dwa razy pod rząd taki sam ruch graczaaa: " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			else
				if (lastTurn != TypeOfTurn::OPP_FOLD && lastTurn != TypeOfTurn::BEGIN && lastTurn != TypeOfTurn::OPP_TAKE)
					std::cerr << "AI: Naruszenie spójności gry: MY_TAKE po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
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
					gameControl.pressPass();
					lastTurn = TypeOfTurn::MY_FOLD;
				}
				else
				{
					if (!RemoveCardFromDeck(tableSnapshot.stackCard))
						std::cerr << "AI: Nie znaleziono usuwanej karty: " << tableSnapshot.stackCard.getFigure() << " " << tableSnapshot.stackCard.getColor() <<  " w talii!" << " nr ruchu: " << turnNumber << std::endl;
					gameControl.pickCardFromHiddenStack();
					lastTurn = TypeOfTurn::MY_TAKE;
				}
			}
		}
		else //jeśli wzięto kartę
		{
			WarningGameCorrectness(TypeOfTurn::MY_PUT);
			if (lastTaken != Card(Card_Figure::None, Card_Color::None) && find (difference.added.cbegin(), difference.added.cend(), lastTaken) == difference.added.cend())
				std::cerr << "AI: Nie znaleziono pobranej karty: " << lastTaken.getFigure() << " " << lastTaken.getColor() << "! nr ruchu: " << turnNumber << std::endl;
			if (difference.added.size() != 1)
			{
				std::cerr << "AI: Pobrano: " << difference.added.size() << " kart(ę/y) zamiast jednej!" << " nr ruchu: " << turnNumber << std::endl;
				std::cerr << DIM_BLUE << difference.added << COLOR_RESET;
			}
			else
				std::cerr << "AI: Pobrano: " << DIM_BLUE << difference.added.begin()->getFigure() << " " << difference.added.begin()->getColor() << COLOR_RESET << ". nr ruchu: " << turnNumber << std::endl;
			WarningCardsCount(tableSnapshot, true);
			ReplaceOpponentCard(difference.added);
			ReduceGruppedCards();
// 			ShowSeqs();
// 			ShowGroups();
			lastTurn = TypeOfTurn::MY_PUT;
			if (ungruppedCards.size())
			{
				CheckUsefulnessForOpponent();
				std::multiset < AICard, Comparation > ungruppedCardsTmp(CreateSeqsGroupsUnusedCards(ungruppedCards, MIN_SEQ_LEN - 1, MIN_GROUP_LEN - 1));
				AICard cardToPut;
				if (!ungruppedCardsTmp.empty())
					cardToPut = ChooseCardToPut(ungruppedCardsTmp, 30.0, 40.0);
				else
					cardToPut = ChooseCardToPut(ungruppedCards, 20.0, 35.0);
				gameControl.throwMyCard(lastStackCard = lastPut = cardToPut);
				if (ungruppedCards.size() == 1)
					lastTurn = EndGame(tableSnapshot);
				else
					if (UngruppedCardsPoints() <= END_GAME_POINTS)
						lastTurn = EndGame(tableSnapshot);
			}
			else
				if (myFirstMove)
					lastTurn = EndGame(tableSnapshot);
				else
					std::cerr << "AI: Brak niepogrupowanych kart! Nie wykonano ruchu!" << " nr ruchu: " << turnNumber << std::endl;
		}
		myLastTurn = lastTurn;
		myFirstMove = false;
	}
	else//from tableSnapshot must be updated: opponentTookCardFromHiddenStack, stackCard
	{
		if (tableSnapshot.opponentTookCardFromHiddenStack) //jeśli wziął kartę
		{
			if (lastStackCard != tableSnapshot.stackCard) //jeśli wziął kartę z odkrytego stosu
			{
				if(lastStackCard == Card(Card_Figure::None, Card_Color::None))
					std::cerr << "AI: Dodano pustą kartę do kart przeciwnika!" << " nr ruchu: " << turnNumber << std::endl;
				opponentCards.emplace_back(lastStackCard, true);
				lastStackCard = tableSnapshot.stackCard;
			}
			else
				RemoveCardFromDeck(tableSnapshot.stackCard);
		
			if (lastTurn == TypeOfTurn::OPP_TAKE)
				std::cerr << "AI: Dwa razy pod rząd taki sam ruch przeciwnika: " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
			else
				if (lastTurn != TypeOfTurn::OPP_FOLD && lastTurn != TypeOfTurn::OPP_BEFORE_TAKE)
					std::cerr << "AI: Naruszenie spójności gry: OPP_TAKE po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
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
					std::cerr << "AI: Dwa razy pod rząd ruch przeciwnika przed pobraniem karty!" << " nr ruchu: " << turnNumber << std::endl;
				else
					if (lastPut != tableSnapshot.stackCard)
						std::cerr << "AI: Nie wydano odpowiedniej karty: " << lastPut.getFigure() << " " << lastPut.getColor() << "! Na stosie znajduje się: " << tableSnapshot.stackCard.getFigure() << " " << tableSnapshot.stackCard.getColor() << "! nr ruchu: " << turnNumber << std::endl;
					else
						std::cerr << "AI: Wydano: " << DIM_GREEN << lastPut.getFigure() << " " << lastPut.getColor() << COLOR_RESET << ". nr ruchu: " << turnNumber << std::endl;
				lastTurn = TypeOfTurn::OPP_BEFORE_TAKE;
			}
			lastStackCard = tableSnapshot.stackCard;
		}
	}
}

void ArtificialIntelligence::WarningGameCorrectness(TypeOfTurn const& currentTurn)
{
	if (lastTurn == currentTurn)
		std::cerr << "AI: Dwa razy pod rząd taki sam ruch gracza: " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
	else
		if (lastTurn != static_cast<TypeOfTurn>(static_cast<int>(currentTurn) - 1))
			std::cerr << "AI: Naruszenie spójności gry: " << currentTurn << " po " << lastTurn << "! nr ruchu: " << turnNumber << std::endl;
}

void ArtificialIntelligence::WarningCardsCount(TableSnapshot const& tableSnapshot, const bool& isTaken)
{
	if (tableSnapshot.playerCards.size() != PLAYER_CARDS_COUNT + (isTaken ? 1 : 0))
	{
		std::cerr << "AI: Nieprawidłowa ilość kart gracza: " << tableSnapshot.playerCards.size() << " zamiast: " << PLAYER_CARDS_COUNT + (isTaken ? 1 : 0) << "! nr ruchu: " << turnNumber << std::endl;
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
		std::cerr << "AI: Błąd przy liczeniu punktów dla warunku zakończenia gry" << "! nr ruchu: " << turnNumber << std::endl;
	return points;
}

void ArtificialIntelligence::ReplaceOpponentCard(const auto& addedDifference)
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
						std::cerr << "AI: Nie zamieniono karty przeciwnikowi! " << ". nr ruchu: " << turnNumber << std::endl;
					break;
				}
				else
					std::cerr << "AI: Przeciwnik ma taką samą kartę co gracz: " << *cit << ". nr ruchu: " << turnNumber << std::endl;
	}
}

AICard ArtificialIntelligence::ChooseCardToPut(std::multiset < AICard, Comparation > sourceCards, const double& tryToThrowTheGreatestFigureThreshold, const double& ifTheUsefulnessIsLessThanThreshold) const
{
	AICard cardToPut = *std::min_element(sourceCards.begin(), sourceCards.end(), UsefulnessForOpponentComparation);
	if (cardToPut.getUsefulnessForOpponent() < tryToThrowTheGreatestFigureThreshold)
	{
		std::multiset < AICard, Comparation >::const_reverse_iterator crit = std::find_if(sourceCards.crbegin(), sourceCards.crend(), [& ifTheUsefulnessIsLessThanThreshold](const AICard& card){ return card.getUsefulnessForOpponent() < ifTheUsefulnessIsLessThanThreshold; });// znajdowanie karty o największej figurze z przydatnością mniejszą od 45.0 %
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
		std::cerr << "AI: Nie zastosowano właściwie różnicy kart!" << " nr ruchu: " << turnNumber << std::endl;
// 	ShowCards();
	return difference;
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
	for (const Card& card : tableSnapshot.playerCards)
		cards.emplace(card, newCardID++);
	RemoveCardsFromDeck();
}

ArtificialIntelligence::TypeOfTurn ArtificialIntelligence::EndGame(TableSnapshot const& tableSnapshot) const
{
	gameControl.endGame();
	gameControl.throwMyCard(*std::min_element(ungruppedCards.begin(), ungruppedCards.end(), UsefulnessForOpponentComparation));
	TouchMyCards();
	return TypeOfTurn::END_GAME;
}

void ArtificialIntelligence::TouchMyCards() const
{
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
	for (SequencesOfCards::const_iterator it = sequences.begin(); it != sequences.end();)
	{
		for (Sequence::iterator it2 = it->begin(); it2 != it->end(); ++it2)
			if (it2->getId() > 0)
			{
				std::list< Group >::iterator itf = groups.FindCard(it2->getId());
				if (itf != groups.end())
				{
					std::multiset < AICard, Comparation > sequence(it->ReturnCopy());
					sequence.erase( std::find_if( sequence.begin(), sequence.end(), [& it2](const AICard& card){ return card.getId() == it2->getId(); } ) );
					std::multiset < AICard, Comparation > sequence2(sequence);
					std::multiset < AICard, Comparation > group;
					for (const AICard& card : itf->ReturnCopy())
						group.insert(card);
					group.erase( std::find_if( group.begin(), group.end(), [& it2](const AICard& card){ return card.getId() == it2->getId(); } ) );
					std::multiset < AICard, Comparation > group2(group);
					Sequence seq(sequence);
					Group gr(group);
					if (gr.CheckCorrectness())
					{
						std::cerr << DIM_YELLOW << "AI: Zredukowano grupę: " << group2.begin()->getFigure() << COLOR_RESET << std::endl;
						groups.erase(itf);
						groups.AddGroup(group2);
					}
					else
						if (!seq.CheckCorrectness() && !gr.CheckCorrectness())// nie uwzględnia przypadku gdy dwie grupy zakrywają sekwencję
						{
							int seqPoints = 0, grPoints = 0;
							for (const AICard& card : sequence2)
								seqPoints += static_cast<int>(card.getFigure());
							for (const AICard& card : group2)
								grPoints += static_cast<int>(card.getFigure());
							
							if (!seqPoints || !grPoints)
								std::cerr << "AI: Punkty równe zero!" << std::endl;
							else
								if (seqPoints < grPoints)
								{
									std::cerr << DIM_YELLOW << "AI: Usunięto sekwencję: " << it->begin()->getColor() << COLOR_RESET << std::endl;
									ungruppedCards.insert(it->begin(), it->end());
									it = sequences.erase(it);
									break;
								}
								else
								{
									std::cerr << DIM_YELLOW << "AI: Usunięto grupę: " << itf->begin()->getFigure() << COLOR_RESET << std::endl;
									ungruppedCards.insert(itf->begin(), itf->end());
									groups.erase(itf);
								}
						}
						else
							if (seq.CheckCorrectness() && !gr.CheckCorrectness())
							{
								std::cerr << DIM_YELLOW << "AI: Zredukowano sekwencję: " << it->begin()->getColor() << COLOR_RESET << std::endl;
								it = sequences.erase(it);
								sequences.AddSequence(sequence2);
								break;
							}
				}
			}
			else
				std::cerr << "AI: Redukcja kart - id karty równe zero!" << std::endl;
		++it;
	}
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

// int ArtificialIntelligence::CreateCards()
// {
// 	cards.emplace(Card_Figure::THREE, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::THREE, Card_Color::CLUB);
// 	cards.emplace(Card_Figure::SEVEN, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::FOUR, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::TWO, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::KING, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::NINE, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::EIGHT, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::FIVE, Card_Color::HEART);
// 	cards.emplace(Card_Figure::FIVE, Card_Color::DIAMOND);
// 	cards.emplace(Card_Figure::FIVE, Card_Color::CLUB);
// 	cards.emplace(Card_Figure::FOUR, Card_Color::HEART);
// 	cards.emplace(Card_Figure::QUEEN, Card_Color::DIAMOND);
// 	cards.emplace(Card_Figure::THREE, Card_Color::HEART);
// 	cards.emplace(Card_Figure::FIVE, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::FIVE, Card_Color::SPADE);
// 	cards.emplace(Card_Figure::KING, Card_Color::DIAMOND);
// 	cards.emplace(Card_Figure::SEVEN, Card_Color::DIAMOND);
// 	cards.emplace(Card_Figure::JACK, Card_Color::DIAMOND);
// 	cards.emplace(Card_Figure::SEVEN, Card_Color::HEART);
// 	
// 	return cards.size();
// }

int ArtificialIntelligence::ShowCards() const
{
	std::cout << "Cards:" << std::endl;
	for (const AICard& card : cards)
		std::cout << card;
	return cards.size();
}

int ArtificialIntelligence::ShowCardDeck() const
{
	std::cout << "AICard Deck:" << std::endl;
	for (const Card& card : cardDeck)
		std::cout << card;
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
