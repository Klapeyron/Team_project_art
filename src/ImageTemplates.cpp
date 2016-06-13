#include "ImageTemplates.hpp"

ImageTemplates::ImageTemplates(std::string const& templatesDir) :templatesDirectory(templatesDir),
                                                               leftUpperCorner(cv::imread(templatesDirectory + leftUpperCornerFileName)),
                                                               rightUpperCorner(cv::imread(templatesDirectory + rightUpperCornerFileName)),
                                                               myTurn(cv::imread(templatesDirectory + myTurnFileName)),
                                                               enemyCardTaken(cv::imread(templatesDirectory + enemyCardTakenFileName)),
                                                               pasButton(cv::imread(templatesDirectory + pasButtonFileName)),
                                                               okButton(cv::imread(templatesDirectory + okButtonFileName)),
                                                               stukamButton(cv::imread(templatesDirectory + stukamButtonFileName)),
                                                               pasDisabledButton(cv::imread(templatesDirectory + pasDisabledButtonFileName)),                                                                   okDisabledButton(cv::imread(templatesDirectory + okDisabledButtonFileName)),
                                                               stukamDisabledButton(cv::imread(templatesDirectory + stukamDisabledButtonFileName)),
                                                               startButton(cv::imread(templatesDirectory + startButtonFileName)),
                                                               blueBackground(cv::imread(templatesDirectory + blueBackgroundFileName))
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
