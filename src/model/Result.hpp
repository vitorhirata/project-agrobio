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
private:
  void sumResultElement(std::vector<float>* v1, std::vector<float>* v2);
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
      numberVariety.begin(), std::plus<int>());

  sumResultElement(&meanVarietyHD, &((*resultToSum).meanVarietyHD));
  sumResultElement(&totalPunctuation, &((*resultToSum).totalPunctuation));
  sumResultElement(&productivityPunctuation, &((*resultToSum).productivityPunctuation));
  sumResultElement(&simpsonCommunity, &((*resultToSum).simpsonCommunity));
  sumResultElement(&shannonCommunity, &((*resultToSum).shannonCommunity));
  sumResultElement(&bergerParkerCommunity, &((*resultToSum).bergerParkerCommunity));
  sumResultElement(&simpsonHD, &((*resultToSum).simpsonHD));
  sumResultElement(&shannonHD, &((*resultToSum).shannonHD));
  sumResultElement(&varietyDistribution, &((*resultToSum).varietyDistribution));
  sumResultElement(&varietyQuantity, &((*resultToSum).varietyQuantity));
  sumResultElement(&bergerParkerHD, &((*resultToSum).bergerParkerHD));
  sumResultElement(&productivityFrequency, &((*resultToSum).productivityFrequency));
  sumResultElement(&qualityFrequency, &((*resultToSum).qualityFrequency));
  sumResultElement(&hdDistribution, &((*resultToSum).hdDistribution));
}

void Result::sumResultElement(std::vector<float>* v1, std::vector<float>* v2){
  std::transform(v1->begin(), v1->end(), v2->begin(), v1->begin(),
      std::plus<float>());
}


#endif
