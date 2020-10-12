#ifndef RESULT_H
#define RESULT_H

class Result{
public:
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
  Result(int timeSize = 0, int histogramSize = 0, int HDsize = 0,
      int varQuantSize = 0);
};

// result constructor, default values
Result::Result(int timeSize, int histogramSize, int HDsize, int varQuantSize)
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

#endif
