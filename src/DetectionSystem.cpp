#include "DetectionSystem.hpp"

void DetectionSystem::processSnapshot(std::string const& imageFilePath)
{
  TableSnapshot tableSnapshot;
  tableSnapshot.playerCards.push_back(Card(Card_Figure::A,Card_Color::SPADE));
  TableSubject::notify(tableSnapshot);
}
