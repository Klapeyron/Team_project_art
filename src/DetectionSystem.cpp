#include "DetectionSystem.hpp"

DetectionSystem::DetectionSystem(std::string const& imageFilePath) :tableImageFilePath(imageFilePath),
                                                                    leftUpperCorner(cv::imread(templatesDirectory + leftUpperCornerFileName)),
                                                                    rightUpperCorner(cv::imread(templatesDirectory + rightUpperCornerFileName)),
                                                                    myTurn(cv::imread(templatesDirectory + myTurnFileName)),
                                                                    previousTableSnapshot()
{
  for(auto i = 0u; i < redTemplateFileNames.size(); i++)
    redCardTemplates[i] = cv::imread(templatesDirectory + redTemplateFileNames[i]);

  for(auto i = 0u; i < blackTemplateFileNames.size(); i++)
    blackCardTemplates[i] = cv::imread(templatesDirectory + blackTemplateFileNames[i]);

  for(auto i = 0u; i < redStackTemplateFileNames.size(); i++)
    redStackCardTemplates[i] = cv::imread(templatesDirectory + redStackTemplateFileNames[i]);

  for(auto i = 0u; i < blackStackTemplateFileNames.size(); i++)
    blackStackCardTemplates[i] = cv::imread(templatesDirectory + blackStackTemplateFileNames[i]);

  for(auto i = 0u; i < stackColorFileNames.size(); i++)
    stackColorTemplates[i] = cv::imread(templatesDirectory + stackColorFileNames[i]);
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

  auto stackCardHandle = std::async(std::launch::async, [&]()
                                    {
                                      return findStackCard(stack);
                                    });

  auto upperCardsHandle = std::async(std::launch::async, [&]()
                                     {
                                       auto heartCards = getCardsFromSelectedArea(redCardTemplates, upperCards, Card_Color::HEART);
                                       auto clubCards = getCardsFromSelectedArea(blackCardTemplates, upperCards, Card_Color::CLUB);
                                       std::copy(clubCards.begin(), clubCards.end(), std::back_inserter(heartCards));
                                       return heartCards;
                                     });

  auto lowerCardsHandle = std::async(std::launch::async, [&]()
                                     {
                                       auto diamondCards = getCardsFromSelectedArea(redCardTemplates, lowerCards, Card_Color::DIAMOND);
                                       auto spadeCards = getCardsFromSelectedArea(blackCardTemplates, lowerCards, Card_Color::SPADE);
                                       std::copy(spadeCards.begin(), spadeCards.end(), std::back_inserter(diamondCards));
                                       return diamondCards;
                                     });
  
  auto foundUpperCards = upperCardsHandle.get();
  auto foundLowerCards = lowerCardsHandle.get();
  tableSnapshot.stackCard = stackCardHandle.get();
  
  std::copy(foundUpperCards.begin(), foundUpperCards.end(), std::back_inserter(tableSnapshot.playerCards));
  std::copy(foundLowerCards.begin(), foundLowerCards.end(), std::back_inserter(tableSnapshot.playerCards));

  bool myTurnMatched = false;
  std::tie(myTurnMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(middle, myTurn);
  tableSnapshot.myMove = myTurnMatched;

  if(previousTableSnapshot == tableSnapshot)
    return;
  previousTableSnapshot = tableSnapshot;
  
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

Card DetectionSystem::findStackCard(Image const& stackArea)
{
  Card_Color color = Card_Color::None;
  Position stackCardPosition;

  for(auto it = stackColorTemplates.begin(); it != stackColorTemplates.end(); ++it)
  {
    bool colorMatched = false;
    std::tie(colorMatched, stackCardPosition) = ImageAnalyzer::containsImageTemplate(stackArea, *it);
    if(colorMatched)
      color = static_cast<Card_Color>(std::distance(stackColorTemplates.begin(),it) + 1);
  }

  for(auto it = blackStackCardTemplates.begin(); it != blackStackCardTemplates.end(); ++it)
  {
    bool colorMatched = false;
    std::tie(colorMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(stackArea, *it);
    if(colorMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(blackStackCardTemplates.begin(),it) + 1);
      return Card(figure,color,stackCardPosition);
    }
  }

  for(auto it = redStackCardTemplates.begin(); it != redStackCardTemplates.end(); ++it)
  {
    bool colorMatched = false;
    std::tie(colorMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(stackArea, *it);
    if(colorMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(redStackCardTemplates.begin(),it) + 1);
      return Card(figure,color,stackCardPosition);
    }
  }
  return Card(Card_Figure::None, Card_Color::None);
}
