#include "DetectionSystem.hpp"
#include "ArtificialIntelligence.hpp"
#include "GameControl.hpp"

#include "CardOperators.hpp"
#include <iostream>

class MovePrinter :public IGameControl, public TableObserver
{
  void onUpdate(TableSnapshot const&) {}
  void pickCardFromHiddenStack()
  {
    std::cout << "pickCardFromHiddenStack" << std::endl;
  };
  void pickCardFromStack()
  {
    std::cout << "pickCardFromStack" << std::endl;
  };
  void throwMyCard(Card const& card)
  {
    std::cout << "throwMyCard " << card << std::endl;
  };
  void endGame()
  {
    std::cout << "endGame" << std::endl;
  };
  void touchCard(Card const& card)
  {
    std::cout << "touchCard " << card << std::endl;
  };
  void pressOK()
  {
    std::cout << "pressOk" << std::endl;
  };
};

int main()
{
    DetectionSystem detectionSystem(printScreenFileName);
    // GameControl gameControl;
    MovePrinter gameControl;
    detectionSystem.registerOberver(gameControl);

    ArtificialIntelligence artificialIntelligence(gameControl);
    detectionSystem.registerOberver(artificialIntelligence);

    detectionSystem.detectWindowByMouseClickOnIt();
    while(true)
    {
      detectionSystem.makePrintScreen();
      detectionSystem.processTable();
    }
}
