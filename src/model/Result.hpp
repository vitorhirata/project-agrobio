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
  void sumResult(Result* resultToSum);
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

void Result::sumResult(Result* resultToSum){
  std::transform(numberVariety.begin(),
      numberVariety.end(), (*resultToSum).numberVariety.begin(),
      numberVariety.begin(), std::plus<float>());
  std::transform(meanVarietyHD.begin(),
      meanVarietyHD.end(), (*resultToSum).meanVarietyHD.begin(),
      meanVarietyHD.begin(), std::plus<float>());
  std::transform(totalPunctuation.begin(),
      totalPunctuation.end(),
      (*resultToSum).totalPunctuation.begin(),
      totalPunctuation.begin(), std::plus<float>());
  std::transform(productivityPunctuation.begin(),
      productivityPunctuation.end(),
      (*resultToSum).productivityPunctuation.begin(),
      productivityPunctuation.begin(), std::plus<float>());
  std::transform(simpsonCommunity.begin(),
      simpsonCommunity.end(),
      (*resultToSum).simpsonCommunity.begin(),
      simpsonCommunity.begin(), std::plus<float>());
  std::transform(shannonCommunity.begin(),
      shannonCommunity.end(),
      (*resultToSum).shannonCommunity.begin(),
      shannonCommunity.begin(), std::plus<float>());
  std::transform(bergerParkerCommunity.begin(),
      bergerParkerCommunity.end(),
      (*resultToSum).bergerParkerCommunity.begin(),
      bergerParkerCommunity.begin(), std::plus<float>());
  std::transform(simpsonHD.begin(),
      simpsonHD.end(),
      (*resultToSum).simpsonHD.begin(),
      simpsonHD.begin(), std::plus<float>());
  std::transform(shannonHD.begin(),
      shannonHD.end(),
      (*resultToSum).shannonHD.begin(),
      shannonHD.begin(), std::plus<float>());
  std::transform(varietyDistribution.begin(),
      varietyDistribution.end(),
      (*resultToSum).varietyDistribution.begin(),
      varietyDistribution.begin(), std::plus<float>());
  std::transform(varietyQuantity.begin(),
      varietyQuantity.end(),
      (*resultToSum).varietyQuantity.begin(),
      varietyQuantity.begin(), std::plus<float>());
  std::transform(bergerParkerHD.begin(),
      bergerParkerHD.end(),
      (*resultToSum).bergerParkerHD.begin(),
      bergerParkerHD.begin(), std::plus<float>());
  std::transform(productivityFrequency.begin(),
      productivityFrequency.end(),
      (*resultToSum).productivityFrequency.begin(),
      productivityFrequency.begin(), std::plus<float>());
  std::transform(qualityFrequency.begin(),
      qualityFrequency.end(),
      (*resultToSum).qualityFrequency.begin(),
      qualityFrequency.begin(), std::plus<float>());
  std::transform(hdDistribution.begin(),
      hdDistribution.end(), (*resultToSum).hdDistribution.begin(),
      hdDistribution.begin(), std::plus<float>());
}

#endif
