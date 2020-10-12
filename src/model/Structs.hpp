#ifndef STRUCTS_H
#define STRUCTS_H

struct Result{
  std::vector<int> numberVariety;
  std::vector<float> meanVarietyHD;
  std::vector<float> totalPunctuation;
  std::vector<float> productivityPunctuation;
  std::vector<float> productivityFrequency;
  std::vector<float> simpsonCommunity;
  std::vector<float> shannonCommunity;
  std::vector<float> bergerParkerCommunity;
  std::vector<float> simpsonHD;
  std::vector<float> shannonHD;
  std::vector<float> bergerParkerHD;
  std::vector<float> qualityFrequency;
  std::vector<float> varietyDistribution;
  std::vector<float> varietyQuantity;
  std::vector<float> hdDistribution;

  Result(int timeSize, int histogramSize, int HDsize, int varQuantSize)
  : numberVariety(timeSize, 0)
  , meanVarietyHD(timeSize, 0)
  , totalPunctuation(timeSize, 0)
  , productivityPunctuation(timeSize, 0)
  , simpsonCommunity(timeSize, 0)
  , shannonCommunity(timeSize, 0)
  , bergerParkerCommunity(timeSize, 0)
  , simpsonHD(timeSize, 0)
  , shannonHD(timeSize, 0)
  , bergerParkerHD(timeSize, 0)
  , productivityFrequency(histogramSize, 0)
  , qualityFrequency(histogramSize, 0)
  , varietyDistribution(HDsize, 0)
  , varietyQuantity(varQuantSize, 0)
  , hdDistribution(HDsize, 0) {}
};

// Variety struct contain all the proprieties of each variety
struct VarietyData{
  std::vector<float> halfSaturation;
  float quality;
};

struct HDParameter{
  float alpha;
  float probabilityNewVar;
  float percentageNewRandomVar;
  float outsideTradeLimit;
  int selectionStrength;
  HDParameter(){}
  HDParameter(Parameter parameter)
  : alpha(parameter.alpha)
  , probabilityNewVar(parameter.probabilityNewVar)
  , percentageNewRandomVar(parameter.percentageNewRandomVar)
  , outsideTradeLimit(parameter.outsideTradeLimit)
  , selectionStrength(parameter.selectionStrength) {}
};
#endif
