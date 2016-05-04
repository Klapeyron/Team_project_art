#include "DetectionSystem.hpp"

DetectionSystem::DetectionSystem(std::string const& imageFilePath) :tableImageFilePath(imageFilePath),
                                                                    ImageTemplates("../templates/"),
                                                                    previousTableSnapshot(),
                                                                    leftUpperPosition() {}


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
      foundCards.emplace_back(figure, colorOfCardsInArea, position);
    }
  }
  return foundCards;
}

Card DetectionSystem::findStackCard(Image const& stackArea)
{
  Card_Color color = Card_Color::None;
  Position stackCardPosition;

  for(auto it = ImageTemplates::stackColorTemplates.begin(); it != ImageTemplates::stackColorTemplates.end(); ++it)
  {
    bool colorMatched = false;
    std::tie(colorMatched, stackCardPosition) = ImageAnalyzer::containsImageTemplate(stackArea, *it);
    if(colorMatched)
      color = static_cast<Card_Color>(std::distance(ImageTemplates::stackColorTemplates.begin(),it) + 1);
  }

  for(auto it = ImageTemplates::blackStackCardTemplates.begin(); it != ImageTemplates::blackStackCardTemplates.end(); ++it)
  {
    bool colorMatched = false;
    std::tie(colorMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(stackArea, *it);
    if(colorMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(ImageTemplates::blackStackCardTemplates.begin(),it) + 1);
      return Card(figure,color,stackCardPosition);
    }
  }

  for(auto it = ImageTemplates::redStackCardTemplates.begin(); it != ImageTemplates::redStackCardTemplates.end(); ++it)
  {
    bool colorMatched = false;
    std::tie(colorMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(stackArea, *it);
    if(colorMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(ImageTemplates::redStackCardTemplates.begin(),it) + 1);
      return Card(figure,color,stackCardPosition);
    }
  }
  return Card(Card_Figure::None, Card_Color::None);
}

void DetectionSystem::processTable()
{
  Image tableImage = cv::imread(tableImageFilePath);

  auto isGreenFieldLeftUpperPositionDetected = leftUpperPosition.getX() != 0u and leftUpperPosition.getY() != 0u;
  if( not isGreenFieldLeftUpperPositionDetected)
    std::tie(std::ignore,leftUpperPosition) = ImageAnalyzer::containsImageTemplate(tableImage, ImageTemplates::leftUpperCorner);

  auto greenField = AreaOfInterestCutter::cutGreenField(tableImage, leftUpperPosition);
  auto upperCards = AreaOfInterestCutter::cutUpperCards(greenField);
  auto lowerCards = AreaOfInterestCutter::cutLowerCards(greenField);
  auto enemyCards = AreaOfInterestCutter::cutEnemyCards(greenField);
  auto middle = AreaOfInterestCutter::cutMiddlePart(greenField);
  auto stack = AreaOfInterestCutter::cutStackPart(greenField);

  TableSnapshot tableSnapshot;

  auto stackCardHandle = std::async(std::launch::async, [&]()
                                    {
                                      return findStackCard(stack);
                                    });

  auto upperCardsHandle = std::async(std::launch::async, [&]()
                                     {
                                       auto heartCards = getCardsFromSelectedArea(ImageTemplates::redCardTemplates, upperCards, Card_Color::HEART);
                                       auto clubCards = getCardsFromSelectedArea(ImageTemplates::blackCardTemplates, upperCards, Card_Color::CLUB);
                                       std::move(clubCards.begin(), clubCards.end(), std::back_inserter(heartCards));
                                       return heartCards;
                                     });

  auto lowerCardsHandle = std::async(std::launch::async, [&]()
                                     {
                                       auto diamondCards = getCardsFromSelectedArea(ImageTemplates::redCardTemplates, lowerCards, Card_Color::DIAMOND);
                                       auto spadeCards = getCardsFromSelectedArea(ImageTemplates::blackCardTemplates, lowerCards, Card_Color::SPADE);
                                       std::move(spadeCards.begin(), spadeCards.end(), std::back_inserter(diamondCards));
                                       return diamondCards;
                                     });

  auto foundUpperCards = upperCardsHandle.get();
  auto foundLowerCards = lowerCardsHandle.get();
  tableSnapshot.stackCard = stackCardHandle.get();

  std::move(foundUpperCards.begin(), foundUpperCards.end(), std::back_inserter(tableSnapshot.playerCards));
  std::move(foundLowerCards.begin(), foundLowerCards.end(), std::back_inserter(tableSnapshot.playerCards));

  std::tie(tableSnapshot.myMove, std::ignore) = ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::myTurn);
  std::tie(tableSnapshot.opponentTookCardFromHiddenStack, std::ignore) = ImageAnalyzer::containsImageTemplate(enemyCards, ImageTemplates::enemyCardTaken);

  std::tie(tableSnapshot.buttons[ButtonsConstants::OK_BUTTON].first, tableSnapshot.buttons[ButtonsConstants::OK_BUTTON].second) =
    ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::okButton);
  std::tie(tableSnapshot.buttons[ButtonsConstants::PAS_BUTTON].first, tableSnapshot.buttons[ButtonsConstants::PAS_BUTTON].second) =
    ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::pasButton);
  std::tie(tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].first, tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].second) =
    ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::stukamButton);
  std::tie(tableSnapshot.buttons[ButtonsConstants::START_BUTTON].first, tableSnapshot.buttons[ButtonsConstants::START_BUTTON].second) =
    ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::startButton);

  if(previousTableSnapshot == tableSnapshot)
    return;
  previousTableSnapshot = tableSnapshot;
  TableSubject::notify(tableSnapshot);
}
