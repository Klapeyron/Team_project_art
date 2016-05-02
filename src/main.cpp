#include "DetectionSystem.hpp"
#include "ArtificialIntelligence.hpp"
#include "GameControl.hpp"
int main()
{
  DetectionSystem detectionSystem(printScreenFileName);
  GameControl gameControl;
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
