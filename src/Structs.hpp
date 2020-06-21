#ifndef STRUCTS_H
#define STRUCTS_H

// Parameters struct contain all the parameters of one model
struct Parameter{
  int latticeSize;
  int numberInitialVariety; // Number of species in initialization
  int numberInitialVarietyDU; // Number of species in initialization of DU
  int numberResources; // Number of existing resources
  int numberHabitat; // Number of different resources in grid
  int maxTime; // Max time of iteration
  int timeInterval; // Interval in which metrics are counted
  int numberDomesticUnity; // Number of domestic unities
  int nRun; // number of rounds
  int networkType;
  int mSF;
  int kWT;
  float betaWT;
  float probabilyConnectionER; // Probability of connection
  float outsideTradeLimit;
  int selectionStrength;
  float alpha;
  float probabilityNewVar;
  float percentageNewRandomVar;
  float crossingDeviation;
  float deathProbability;
  Parameter()
  : latticeSize(49)
  , numberInitialVariety(10)
  , numberInitialVarietyDU(5)
  , numberResources(3)
  , numberHabitat(1)
  , maxTime(3000)
  , timeInterval(30)
  , numberDomesticUnity(49)
  , nRun(200)
  , networkType(1)
  , mSF(2)
  , kWT(4)
  , betaWT(0.2)
  , probabilyConnectionER(0.15)
  , outsideTradeLimit(0.32)
  , selectionStrength(10)
  , alpha(0.6)
  , probabilityNewVar(0.06)
  , percentageNewRandomVar(0.1)
  , crossingDeviation(0.005)
  , deathProbability(0.2) {}
};

struct Result{
  std::vector<int> numberVariety;
  std::vector<float> meanVarietyDU;
  std::vector<float> totalPunctuation;
  std::vector<float> productivityPunctuation;
  std::vector<float> productivityFrequency;
  std::vector<float> simpsonCommunity;
  std::vector<float> shannonCommunity;
  std::vector<float> bergerParkerCommunity;
  std::vector<float> simpsonDU;
  std::vector<float> shannonDU;
  std::vector<float> bergerParkerDU;
  std::vector<float> qualityFrequency;
  std::vector<float> varietyDistribution;
  std::vector<float> varietyQuantity;
  std::vector<float> duDistribution;

  Result(int timeSize, int histogramSize, int DUsize, int varQuantSize)
  : numberVariety(timeSize, 0)
  , meanVarietyDU(timeSize, 0)
  , totalPunctuation(timeSize, 0)
  , productivityPunctuation(timeSize, 0)
  , simpsonCommunity(timeSize, 0)
  , shannonCommunity(timeSize, 0)
  , bergerParkerCommunity(timeSize, 0)
  , simpsonDU(timeSize, 0)
  , shannonDU(timeSize, 0)
  , bergerParkerDU(timeSize, 0)
  , productivityFrequency(histogramSize, 0)
  , qualityFrequency(histogramSize, 0)
  , varietyDistribution(DUsize, 0)
  , varietyQuantity(varQuantSize, 0)
  , duDistribution(DUsize, 0) {}
};

// Variety struct contain all the proprieties of each variety
struct VarietyData{
  std::vector<float> halfSaturation;
  float quality;
};

struct DUParameter{
  float alpha;
  float probabilityNewVar;
  float percentageNewRandomVar;
  float outsideTradeLimit;
  int selectionStrength;
  DUParameter(){}
  DUParameter(Parameter parameter)
  : alpha(parameter.alpha)
  , probabilityNewVar(parameter.probabilityNewVar)
  , percentageNewRandomVar(parameter.percentageNewRandomVar)
  , outsideTradeLimit(parameter.outsideTradeLimit)
  , selectionStrength(parameter.selectionStrength) {}
};
#endif
