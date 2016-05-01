#include "DetectionSystem.hpp"

DetectionSystem::DetectionSystem(std::string const& imageFilePath) :tableImageFilePath(imageFilePath)
{
  for(auto i = 0u; i < redTemplateFileNames.size(); i++)
    redCardTemplates[i] = cv::imread(templatesDirectory + redTemplateFileNames[i]);

  for(auto i = 0u; i < blackTemplateFileNames.size(); i++)
    blackCardTemplates[i] = cv::imread(templatesDirectory + blackTemplateFileNames[i]);
}

void DetectionSystem::processTable()
{
  // TODO: cut green field
  // TODO: select roi for upper/lover cards, stack area

  TableSnapshot tableSnapshot;

  // TODO: detect turn
  // TODO: detect stack card

  // TODO: process upper cards
  // TODO: process lower cards

  TableSubject::notify(tableSnapshot);
  TableSubject::waitForUnfinishedJobs();
}
