#include "DetectionSystem.hpp"
#include "ArtificialIntelligence.hpp"
#include "GameControl.hpp"

#include "CardOperators.hpp"

int main()
{
    DetectionSystem detectionSystem(printScreenFileName);
    detectionSystem.detectWindowByMouseClickOnIt();
    GameControl gameControl;

    detectionSystem.registerOberver(gameControl);

    ArtificialIntelligence artificialIntelligence(gameControl, true);
    detectionSystem.registerOberver(artificialIntelligence);

    while(true)
    {
      detectionSystem.makePrintScreen();
      detectionSystem.processTable();
    }
}
