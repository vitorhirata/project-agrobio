#ifndef PARAMETER_H
#define PARAMETER_H

class Parameter{
public:
  int latticeSize;
  int numberInitialVariety; // Number of species in initialization
  int numberInitialVarietyHD; // Number of species in initialization of HD
  int numberResources; // Number of existing resources
  int numberHabitat; // Number of different resources in grid
  int maxTime; // Max time of iteration
  int timeInterval; // Interval in which metrics are counted
  int numberHousehold; // Number of households
  int nRun; // number of rounds
  int networkType; // algorithm used to create network, 0 is ER, 1 is WT 2 is SF
  int mSF; // scale free
  int kWT; // watts-strogatz
  float betaWT; // watts-strogatz
  float probabilyConnectionER; // Probability of connection ER
  float outsideTradeLimit;
  int selectionStrength;
  float alpha;
  float probabilityNewVar;
  float percentageNewRandomVar;
  float crossingDeviation;
  float deathProbability;
  Parameter();
};

// parameter constructor, default values
Parameter::Parameter()
  : latticeSize(49)
  , numberInitialVariety(10)
  , numberInitialVarietyHD(5)
  , numberResources(3)
  , numberHabitat(1)
  , maxTime(3000)
  , timeInterval(30)
  , numberHousehold(49)
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

#endif
