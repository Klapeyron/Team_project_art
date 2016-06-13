#include "DetectionSystem.hpp"

DetectionSystem::DetectionSystem(std::string const& imageFilePath) :tableImageFilePath(imageFilePath),
                                                                    ImageTemplates("../templates/"),
                                                                    previousTableSnapshot(),
                                                                    leftUpperPosition() {}

std::vector<Card> DetectionSystem::getCardsFromSelectedArea(FigureTemplatesType const& figureTemplates, Image const& areaImage, Card_Color colorOfCardsInArea)
{
  std::vector<Card> foundCards;
  for(auto it = figureTemplates.begin(); it != figureTemplates.end(); ++it)
  {
    bool figureMatched = false;
    Position position;
    std::tie(figureMatched, position) = ImageAnalyzer::containsImageTemplate(areaImage, *it);
    if(figureMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(figureTemplates.begin(),it) + 1);
      foundCards.emplace_back(figure, colorOfCardsInArea, position);
    }
  }
  return foundCards;
}

Card DetectionSystem::findStackCard(Image const& stackArea)
{
  Position stackCardPosition;

  auto searchForColorInStackArea = [&](ColorTemplatesType const& colorTemplates)
      {
        for(auto it = colorTemplates.begin(); it != colorTemplates.end(); ++it)
        {
          bool colorMatched = false;
          std::tie(colorMatched, stackCardPosition) = ImageAnalyzer::containsImageTemplate(stackArea, *it);
          if(colorMatched)
            return static_cast<Card_Color>(std::distance(colorTemplates.begin(),it) + 1);
        }
        return Card_Color::None;
      };

  auto searchForFigureInStackArea = [&](FigureTemplatesType const& figureTemplates)
      {
        for(auto it = figureTemplates.begin(); it != figureTemplates.end(); ++it)
        {
          bool figureMatched = false;
          std::tie(figureMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(stackArea, *it);
          if(figureMatched)
            return static_cast<Card_Figure>(std::distance(figureTemplates.begin(),it) + 1);
        }
        return Card_Figure::None;
      };

  auto foundColor = searchForColorInStackArea(ImageTemplates::stackColorTemplates);
  auto foundFigure = searchForFigureInStackArea(ImageTemplates::blackStackCardTemplates);

  if(Card_Figure::None == foundFigure)
    foundFigure = searchForFigureInStackArea(ImageTemplates::redStackCardTemplates);

  return Card(foundFigure, foundColor, stackCardPosition);
}

void DetectionSystem::processTable()
{
  Image tableImage = cv::imread(tableImageFilePath);

  auto isGreenFieldLeftUpperPositionDetected = leftUpperPosition != Position();
  if( not isGreenFieldLeftUpperPositionDetected)
    std::tie(std::ignore,leftUpperPosition) = ImageAnalyzer::containsImageTemplate(tableImage, ImageTemplates::leftUpperCorner);

  auto greenField = AreaOfInterestCutter::cutGreenField(tableImage, leftUpperPosition);
  auto upperCards = AreaOfInterestCutter::cutUpperCards(greenField);
  auto lowerCards = AreaOfInterestCutter::cutLowerCards(greenField);
  auto enemyCards = AreaOfInterestCutter::cutEnemyCards(greenField);
  auto middle = AreaOfInterestCutter::cutMiddlePart(greenField);
  auto leftStack = AreaOfInterestCutter::cutLeftStackPart(greenField);
  auto rightStack = AreaOfInterestCutter::cutRightStackPart(greenField);

  Image hiddenStack, stack;
  Position stackPosition, hiddenStackPosition;

  bool hiddenStackIsOnLeftSide;
  std::tie(hiddenStackIsOnLeftSide, std::ignore) = ImageAnalyzer::containsImageTemplate(leftStack, ImageTemplates::blueBackground);

  if(hiddenStackIsOnLeftSide)
  {
    hiddenStackPosition = Position(AreaOfInterestCutter::LeftStackPosition.x,
                                   AreaOfInterestCutter::LeftStackPosition.y);
    hiddenStack = leftStack;
    stackPosition = Position(AreaOfInterestCutter::RightStackPosition.x,
                             AreaOfInterestCutter::RightStackPosition.y);
    stack = rightStack;
  }
  else
  {
    hiddenStackPosition = Position(AreaOfInterestCutter::RightStackPosition.x,
                                   AreaOfInterestCutter::RightStackPosition.y);
    hiddenStack = rightStack;
    stackPosition = Position(AreaOfInterestCutter::LeftStackPosition.x,
                             AreaOfInterestCutter::LeftStackPosition.y);
    stack = leftStack;
  }

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
     for(auto & card : heartCards)
     {
       card.setNewPosition(card.getX() + leftUpperPosition.getX() + AreaOfInterestCutter::UpperCardsPosition.x + 10,
                           card.getY() + leftUpperPosition.getY() + AreaOfInterestCutter::UpperCardsPosition.y + 10);
     }
     return heartCards;
   });

  auto lowerCardsHandle = std::async(std::launch::async, [&]()
   {
     auto diamondCards = getCardsFromSelectedArea(ImageTemplates::redCardTemplates, lowerCards, Card_Color::DIAMOND);
     auto spadeCards = getCardsFromSelectedArea(ImageTemplates::blackCardTemplates, lowerCards, Card_Color::SPADE);
     std::move(spadeCards.begin(), spadeCards.end(), std::back_inserter(diamondCards));
     for(auto & card : diamondCards)
     {
       card.setNewPosition(card.getX() + leftUpperPosition.getX() + AreaOfInterestCutter::LowerCardsPosition.x +10,
                           card.getY() + leftUpperPosition.getY() + AreaOfInterestCutter::LowerCardsPosition.y +10);
     }
     return diamondCards;
   });

  auto foundUpperCards = upperCardsHandle.get();
  auto foundLowerCards = lowerCardsHandle.get();
  tableSnapshot.stackCard = stackCardHandle.get();
  tableSnapshot.stackCard.setNewPosition(tableSnapshot.stackCard.getPosition() + stackPosition + leftUpperPosition);

  std::move(foundUpperCards.begin(), foundUpperCards.end(), std::back_inserter(tableSnapshot.playerCards));
  std::move(foundLowerCards.begin(), foundLowerCards.end(), std::back_inserter(tableSnapshot.playerCards));

  std::tie(tableSnapshot.myMove, std::ignore) = ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::myTurn);
  std::tie(tableSnapshot.enemyTookCard, std::ignore) = ImageAnalyzer::containsImageTemplate(enemyCards, ImageTemplates::enemyCardTaken);

  std::tie(tableSnapshot.buttons[ButtonsConstants::OK_BUTTON].first,
           tableSnapshot.buttons[ButtonsConstants::OK_BUTTON].second) =
      ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::okButton, AreaOfInterestCutter::MiddlePartPosition);

  std::tie(tableSnapshot.buttons[ButtonsConstants::OK_DISABLED_BUTTON].first,
           tableSnapshot.buttons[ButtonsConstants::OK_DISABLED_BUTTON].second) =
      ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::okDisabledButton, AreaOfInterestCutter::MiddlePartPosition);

  std::tie(tableSnapshot.buttons[ButtonsConstants::PAS_BUTTON].first,
           tableSnapshot.buttons[ButtonsConstants::PAS_BUTTON].second) =
      ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::pasButton, AreaOfInterestCutter::MiddlePartPosition);

  std::tie(tableSnapshot.buttons[ButtonsConstants::PAS_DISABLED_BUTTON].first,
           tableSnapshot.buttons[ButtonsConstants::PAS_DISABLED_BUTTON].second) =
      ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::pasDisabledButton, AreaOfInterestCutter::MiddlePartPosition);

  std::tie(tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].first,
           tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_BUTTON].second) =
      ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::stukamButton, AreaOfInterestCutter::MiddlePartPosition);

  std::tie(tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_DISABLED_BUTTON].first,
           tableSnapshot.buttons[ButtonsConstants::KNOCK_KNOCK_DISABLED_BUTTON].second) =
      ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::stukamDisabledButton, AreaOfInterestCutter::MiddlePartPosition);

  std::tie(tableSnapshot.buttons[ButtonsConstants::START_BUTTON].first,
           tableSnapshot.buttons[ButtonsConstants::START_BUTTON].second) =
      ImageAnalyzer::containsImageTemplate(middle, ImageTemplates::startButton, AreaOfInterestCutter::MiddlePartPosition);

  std::tie(tableSnapshot.buttons[ButtonsConstants::HIDDEN_STACK].first,
           tableSnapshot.buttons[ButtonsConstants::HIDDEN_STACK].second) =
      ImageAnalyzer::containsImageTemplate(hiddenStack, ImageTemplates::blueBackground, hiddenStackPosition);

  std::tie(tableSnapshot.enemyEndsGame, std::ignore) = ImageAnalyzer::containsImageTemplate(enemyCards, ImageTemplates::blueBackground);
  tableSnapshot.enemyEndsGame = not tableSnapshot.enemyEndsGame;

  for(auto & button : tableSnapshot.buttons)
    button.second.second.setNewPosition(button.second.second.getPosition() + leftUpperPosition + Position(40, 20));

  if(previousTableSnapshot == tableSnapshot)
    return;
  previousTableSnapshot = tableSnapshot;

  TableSubject::notify(tableSnapshot);
}
