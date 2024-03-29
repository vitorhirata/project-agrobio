#ifndef RESULT_H
#define RESULT_H

/*
Class responsible for dealing with model's results. It contains the output of
one or multiple model's execution, also handling aggregation of multiple
model's output and calling Metrics functions to save data on Result.
*/

class Result{
public:
  std::vector<int> numberVariety;
  std::vector<float> meanVarietyHD;
  std::vector<float> totalScore;
  std::vector<float> productivityScore;
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
  Result() = default;
  Result(Parameter t_parameter, int timeSize);
  Result(Parameter t_parameter, Household* t_household, Patch* t_grid);
  void sumTemporal(Result* resultToSum);
  void sumDistributions(Result* resultToSum);
  void sumResult(Result* resultToSum);
  void save_timeline();
  void save_final_state();
private:
  Metrics metrics;
  void sumResultElement(std::vector<float>* v1, std::vector<float>* v2);
};

Result::Result(Parameter t_parameter, int timeSize)
  : numberVariety(timeSize, 0)
  , meanVarietyHD(timeSize, 0)
  , totalScore(timeSize, 0)
  , productivityScore(timeSize, 0)
  , simpsonCommunity(timeSize, 0)
  , shannonCommunity(timeSize, 0)
  , bergerParkerCommunity(timeSize, 0)
  , simpsonHD(timeSize, 0)
  , shannonHD(timeSize, 0)
  , bergerParkerHD(timeSize, 0)
  , productivityFrequency(round(1 / 0.05), 0)
  , qualityFrequency(round(1 / 0.05), 0)
  , varietyDistribution(t_parameter.latticeSize, 0)
  , varietyQuantity(17, 0)
  , hdDistribution(t_parameter.latticeSize, 0) {}

Result::Result(Parameter t_parameter, Household* t_household, Patch* t_grid)
  : metrics(t_parameter, t_household, t_grid) {}

void Result::sumTemporal(Result* resultToSum){
  std::transform(numberVariety.begin(),
      numberVariety.end(), (*resultToSum).numberVariety.begin(),
      numberVariety.begin(), std::plus<int>());

  sumResultElement(&meanVarietyHD, &((*resultToSum).meanVarietyHD));
  sumResultElement(&totalScore, &((*resultToSum).totalScore));
  sumResultElement(&productivityScore, &((*resultToSum).productivityScore));
  sumResultElement(&simpsonCommunity, &((*resultToSum).simpsonCommunity));
  sumResultElement(&shannonCommunity, &((*resultToSum).shannonCommunity));
  sumResultElement(&bergerParkerCommunity, &((*resultToSum).bergerParkerCommunity));
  sumResultElement(&simpsonHD, &((*resultToSum).simpsonHD));
  sumResultElement(&shannonHD, &((*resultToSum).shannonHD));
  sumResultElement(&bergerParkerHD, &((*resultToSum).bergerParkerHD));
}

void Result::sumDistributions(Result* resultToSum){
  sumResultElement(&varietyDistribution, &((*resultToSum).varietyDistribution));
  sumResultElement(&varietyQuantity, &((*resultToSum).varietyQuantity));
  sumResultElement(&productivityFrequency, &((*resultToSum).productivityFrequency));
  sumResultElement(&qualityFrequency, &((*resultToSum).qualityFrequency));
  sumResultElement(&hdDistribution, &((*resultToSum).hdDistribution));
}

void Result::sumResult(Result* resultToSum){
  sumTemporal(resultToSum);
  sumDistributions(resultToSum);
}

void Result::sumResultElement(std::vector<float>* v1, std::vector<float>* v2){
  std::transform(v1->begin(), v1->end(), v2->begin(), v1->begin(),
      std::plus<float>());
}

void Result::save_timeline(){
  numberVariety.push_back(metrics.computeVarietyRichness());
  meanVarietyHD.push_back(metrics.computeVarietyMeanProfile());
  std::vector<float> tempScore = metrics.computeScoreAverage();
  totalScore.push_back(tempScore[0]);
  productivityScore.push_back(tempScore[1]);
  bergerParkerCommunity.push_back(metrics.computeBergerParker());
  bergerParkerHD.push_back(metrics.computeBergerParkerHD());
  simpsonCommunity.push_back(metrics.computeSimpson());
  shannonCommunity.push_back(metrics.computeShannon());
  simpsonHD.push_back(metrics.computeAverageSimpson());
  shannonHD.push_back(metrics.computeAverageShannon());
}

void Result::save_final_state(){
  productivityFrequency = metrics.computeProductivityProfile();
  qualityFrequency = metrics.computeQualityProfile();
  hdDistribution = metrics.computeHDprofile();
  varietyDistribution = metrics.computeVarietyProfile();
  varietyQuantity = metrics.computeVarietyQuantity();
}

#endif
