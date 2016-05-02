#include "DetectionSystem.hpp"

DetectionSystem::DetectionSystem(std::string const& imageFilePath) :tableImageFilePath(imageFilePath),
                                                                    leftUpperCorner(cv::imread(templatesDirectory + leftUpperCornerFileName)),
                                                                    rightUpperCorner(cv::imread(templatesDirectory + rightUpperCornerFileName)),
                                                                    myTurn(cv::imread(templatesDirectory + myTurnFileName))
{
  for(auto i = 0u; i < redTemplateFileNames.size(); i++)
    redCardTemplates[i] = cv::imread(templatesDirectory + redTemplateFileNames[i]);

  for(auto i = 0u; i < blackTemplateFileNames.size(); i++)
    blackCardTemplates[i] = cv::imread(templatesDirectory + blackTemplateFileNames[i]);
}

Image DetectionSystem::cutGreenField(Image const& tableImage)
{
  bool matched = false;
  std::tie(matched, leftUpperPosition) = ImageAnalyzer::containsImageTemplate(tableImage, leftUpperCorner);
  return tableImage(cv::Rect(leftUpperPosition.getX() + 8, leftUpperPosition.getY() + 6, 714, 597));
}

Image DetectionSystem::cutUpperCards(Image const& greenField)
{
  return greenField(cv::Rect(112, 326, 424, 85));
}

Image DetectionSystem::cutLowerCards(Image const& greenField)
{
  return greenField(cv::Rect(112, 409, 424, 80));
}

Image DetectionSystem::cutEnemyCards(Image const& greenField)
{
  return greenField(cv::Rect(238, 13, 239, 106));
}

Image DetectionSystem::cutMiddlePart(Image const& greenField)
{
  return greenField(cv::Rect(187, 123, 333, 218));
}

Image DetectionSystem::cutStackPart(Image const& greenField)
{
  return greenField(cv::Rect(600, 175, 85, 100));
}

void DetectionSystem::processTable()
{
  Image tableImage = cv::imread(tableImageFilePath);
  auto greenField = cutGreenField(tableImage);
  auto upperCards = cutUpperCards(greenField);
  auto lowerCards = cutLowerCards(greenField);
  auto enemyCards = cutEnemyCards(greenField);
  auto middle = cutMiddlePart(greenField);
  auto stack = cutStackPart(greenField);

  TableSnapshot tableSnapshot;

  auto heartCards = getCardsFromSelectedArea(redCardTemplates, upperCards, Card_Color::HEART);
  auto clubCards = getCardsFromSelectedArea(blackCardTemplates, upperCards, Card_Color::CLUB);
  auto diamondCards = getCardsFromSelectedArea(redCardTemplates, lowerCards, Card_Color::DIAMOND);
  auto spadeCards = getCardsFromSelectedArea(blackCardTemplates, lowerCards, Card_Color::SPADE);

  tableSnapshot.playerCards.insert(tableSnapshot.playerCards.end(), std::make_move_iterator(heartCards.begin()),   std::make_move_iterator(heartCards.end()));
  tableSnapshot.playerCards.insert(tableSnapshot.playerCards.end(), std::make_move_iterator(clubCards.begin()),    std::make_move_iterator(clubCards.end()));
  tableSnapshot.playerCards.insert(tableSnapshot.playerCards.end(), std::make_move_iterator(diamondCards.begin()), std::make_move_iterator(diamondCards.end()));
  tableSnapshot.playerCards.insert(tableSnapshot.playerCards.end(), std::make_move_iterator(spadeCards.begin()),   std::make_move_iterator(spadeCards.end()));
  // TODO: stack card recognize
  // tableSnapshot.stackCard = Card(Card_Figure::JACK,Card_Color::SPADE);
  
  bool myTurnMatched = false;
  std::tie(myTurnMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(middle, myTurn);
  tableSnapshot.myMove = myTurnMatched;
  
  TableSubject::notify(tableSnapshot);
  TableSubject::waitForUnfinishedJobs();
}

std::vector<Card> DetectionSystem::getCardsFromSelectedArea(std::array<Image,13> const& imageTemplates, Image const& areaImage, Card_Color colorOfCardsInArea)
{
  std::vector<Card> foundCards;
  for(auto it = imageTemplates.begin(); it != imageTemplates.end(); ++it)
  {
    bool cardMatched = false;
    Position position;
    std::tie(cardMatched, position) = ImageAnalyzer::containsImageTemplate(areaImage, *it);
    if(cardMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(imageTemplates.begin(),it) + 1);
      position.setNewPosition(position.getX() + leftUpperPosition.getX() + 5, position.getY() + leftUpperPosition.getY() + 5);
      foundCards.emplace_back(figure, colorOfCardsInArea);
    }
  }
  return foundCards;
}
