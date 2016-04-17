 #include "GameControl.hpp"


void GameControl::pickCardFromHiddenStack()
{
  makeMove(ButtonsSnapshot::getButtonPosition(HIDDEN_STACK_BUTTON));
}

void GameControl:: pickCardFromStack()
{
  makeMove(ButtonsSnapshot::getButtonPosition(STACK_BUTTON));
}

void GameControl:: throwMyCard(const Card &card)
{
  Position pos(card.getX(),card.getY());
  makeMove(pos);
}

void GameControl:: endGame()
{
   makeMove(ButtonsSnapshot::getButtonPosition(END_GAME_BUTTON));
}

void GameControl::touchCard(const Card &card)
{
  Position pos(card.getX(),card.getY());
  makeMove(pos);
}

void GameControl::pressOK()
{
  makeMove(ButtonsSnapshot::getButtonPosition(OK_BUTTON));
}

void GameControl::makeMove(const Position & position)
{
 
}
