 #include "GameControl.hpp"
Position GameControl::getPositionOfCard(const Card & card)
{
  auto it = find(tableSnapshot.playerCards.cbegin(),tableSnapshot.playerCards.cend(),card);
  Position pos(it->getX(),it->getY());
  return pos;
}
void GameControl::pressPass()
{
  std::cout << "pressPass" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::PAS_BUTTON].second);
}
void GameControl::pressKnock()
{
  std::cout << "pressKnock" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].second);
}
void GameControl::onUpdate(TableSnapshot const& snapshot)
{
  tableSnapshot = snapshot;
}

void GameControl::pickCardFromHiddenStack()
{
  std::cout << "pickCardFromHiddenStack" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::HIDDEN_STACK].second);
}

void GameControl:: pickCardFromStack()
{
  std::cout << "pickCardFromStack" << std::endl;
  Position pos(tableSnapshot.stackCard.getX(),tableSnapshot.stackCard.getY());
  setPosition(pos);
}

void GameControl:: throwMyCard(Card const &card)
{
  std::cout << "throwMyCard " << card << std::endl;
  setPosition(getPositionOfCard(card));
}

void GameControl:: endGame()
{
  std::cout << "endGame" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].second);
}

void GameControl::touchCard(const Card &card)
{
  std::cout << "touchCard " << card << std::endl;
  setPosition(getPositionOfCard(card));
}

void GameControl::pressOK()
{
  std::cout << "pressOk" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::OK_BUTTON].second);
}
void GameControl::pressOkDisabled()
{
  std::cout << "pressOkDisabled" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::OK_DISABLED_BUTTON].second);
}
void GameControl::pressPassDisabled()
{
  std::cout << "pressPassDisabled" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::PAS_DISABLED_BUTTON].second);
}
void GameControl::pressKnockDisabled()
{
  std::cout << "pressKnockKnockDisabled" << std::endl;
  setPosition(tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].second);
}
void GameControl::setPosition(const Position & position)
{
  std::string x = std::to_string(position.getX());
  std::string y = std::to_string(position.getY());
  std::string br = " ";
  std::string cmd = "xdotool mousemove "+x+br+y;
  system(cmd.c_str());
  system("xdotool click 1");
}
