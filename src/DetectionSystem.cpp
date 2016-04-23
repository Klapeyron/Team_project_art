#include "DetectionSystem.hpp"

void DetectionSystem::run()
{
  TableSnapshot tableSnapshot;
  tableSnapshot.playerCards.push_back(Card(Card_Figure::A,Card_Color::SPADE));
  TableSubject::notify(tableSnapshot);
  TableSubject::processInBackground();
}
