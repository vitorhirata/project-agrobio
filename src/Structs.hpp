#ifndef STRUCTS_H
#define STRUCTS_H

// Variety struct contain all the proprieties of each variety
struct Variety{
  std::vector<float> K;
  float appearence;
};

// Parameters struct contain all the parameters of one model
struct Parameter{
  int latticeSize;
  int numberVariety; // Number of species in initialization
  int numberResources; // Number of existing resources
  int numberResourceDistribution; // Number of different resources in grid
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
  , numberVariety(50)
  , numberResources(3)
  , numberResourceDistribution(1)
  , maxTime(200)
  , timeInterval(5)
  , numberDomesticUnity(49)
  , nRun(500)
  , probabilyConnection(0.15)
  , outsideTradeLimit(0.02)
  , insideTradeLimit(0.001)
  , alpha(0.5) {}
};
#endif
