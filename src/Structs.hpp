#ifndef STRUCTS_H
#define STRUCTS_H

// Parameters struct contain all the parameters of one model
struct Parameter{
  int latticeSize;
  int numberInitialVariety; // Number of species in initialization
  int numberResources; // Number of existing resources
  int numberHabitat; // Number of different resources in grid
  int maxTime; // Max time of iteration
  int timeInterval; // Interval in which metrics are counted
  int numberDomesticUnity; // Number of domestic unities
  int nRun; // number of rounds
  float probabilyConnection; // Probability of connection
  float outsideTradeLimit;
  float insideTradeLimit;
  float alpha;
  Parameter()
  : latticeSize(49)
  , numberInitialVariety(50)
  , numberResources(3)
  , numberHabitat(1)
  , maxTime(200)
  , timeInterval(5)
  , numberDomesticUnity(49)
  , nRun(500)
  , probabilyConnection(0.15)
  , outsideTradeLimit(0.02)
  , insideTradeLimit(0.001)
  , alpha(0.5) {}
};

struct Result{
  std::vector<int> numberVariety;
  std::vector<float> meanVarietyDU;
  std::vector<float> fitnessFrequency;
  std::vector<float> appearenceFrequency;
  std::vector<float> varietyDistribution;
  Result(int nVarSize, int histogramSize, int DUsize)
  : numberVariety(nVarSize, 0)
  , meanVarietyDU(nVarSize, 0)
  , fitnessFrequency(histogramSize, 0)
  , appearenceFrequency(histogramSize, 0)
  , varietyDistribution(DUsize, 0) {}
};

// Variety struct contain all the proprieties of each variety
struct Variety{
  std::vector<float> K;
  float appearence;
};
#endif
