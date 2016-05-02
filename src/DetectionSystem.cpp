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

  // cv::namedWindow( "image", CV_WINDOW_AUTOSIZE );
  
  // cv::imshow( "image", greenField );
  // cv::waitKey();

  // cv::imshow( "image", upperCards );
  // cv::waitKey();

  // cv::imshow( "image", lowerCards );
  // cv::waitKey();

  // cv::imshow( "image", enemyCards );
  // cv::waitKey();

  // cv::imshow( "image", middle );
  // cv::waitKey();

  // cv::imshow( "image", stack );
  // cv::waitKey();
  // TODO: detect stack card

  for(auto it = redCardTemplates.begin(); it != redCardTemplates.end(); ++it)
  {
    bool cardMatched = false;
    Position position;
    std::tie(cardMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(upperCards, *it);
    if(cardMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(redCardTemplates.begin(),it) + 1);
      tableSnapshot.playerCards.emplace_back(figure, Card_Color::HEART);
    }
  }

  for(auto it = blackCardTemplates.begin(); it != blackCardTemplates.end(); ++it)
  {
    bool cardMatched = false;
    Position position;
    std::tie(cardMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(upperCards, *it);
    if(cardMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(blackCardTemplates.begin(),it) + 1);
      tableSnapshot.playerCards.emplace_back(figure, Card_Color::CLUB);
    }
  }

  for(auto it = redCardTemplates.begin(); it != redCardTemplates.end(); ++it)
  {
    bool cardMatched = false;
    Position position;
    std::tie(cardMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(lowerCards, *it);
    if(cardMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(redCardTemplates.begin(),it) + 1);
      tableSnapshot.playerCards.emplace_back(figure, Card_Color::DIAMOND);
    }
  }

  for(auto it = blackCardTemplates.begin(); it != blackCardTemplates.end(); ++it)
  {
    bool cardMatched = false;
    Position position;
    std::tie(cardMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(lowerCards, *it);
    if(cardMatched)
    {
      auto figure = static_cast<Card_Figure>(std::distance(blackCardTemplates.begin(),it) + 1);
      tableSnapshot.playerCards.emplace_back(figure, Card_Color::SPADE);
    }
  }

  // TODO: stack card recognize
  // tableSnapshot.stackCard = Card(Card_Figure::JACK,Card_Color::SPADE);
  
  bool myTurnMatched = false;
  std::tie(myTurnMatched, std::ignore) = ImageAnalyzer::containsImageTemplate(middle, myTurn);
  tableSnapshot.myMove = myTurnMatched;
  
  TableSubject::notify(tableSnapshot);
  TableSubject::waitForUnfinishedJobs();
}
