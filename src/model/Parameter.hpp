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
  static std::vector<char> keys;
  Parameter();
  void set_parameter(char parameter_key, float value);
  void write_to_file(fstream& arquivo);
  static std::vector<float> get_parameter_variation(char parameter_key);
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



void Parameter::set_parameter(char parameter_key, float value){
  switch (parameter_key){
    case 'T':
      outsideTradeLimit = value;
      break;
    case 'L':
      if(round(sqrt(value)) * round(sqrt(value)) != value){
        cout << "ERROR: the latticeSize should be a perfect square.";
        cout << endl;
        exit(-1);
      }
      latticeSize = value;
      numberHousehold = value;
      break;
    case 'H':
      numberHabitat = value;
      break;
    case 'v':
      if(value > numberInitialVariety){
        cout << "ERROR: the number of initial variety per HD should be ";
        cout << "smaller than total number of initial variety." << endl;
        exit(-1);
      }
      numberInitialVarietyHD = value;
      break;
    case 'V':
      numberInitialVariety = value;
      break;
    case 'a':
      alpha = value;
      break;
    case 'R':
      networkType = value;
      break;
    case 'N':
      probabilityNewVar = value;
      break;
    case 'b':
      betaWT = value;
      break;
    case 'r':
      numberResources = value;
      break;
    case 'S':
      selectionStrength = value;
      break;
    case 'Q':
      percentageNewRandomVar = value;
      break;
    case 'C':
      crossingDeviation = value;
      break;
    case 'M':
      deathProbability = value;
    default:
      cout << "ERROR: invalid character parameter key. '";
      cout << parameter_key << "' is not a valid parameter key" << endl;
      exit(-1);
  }
}

std::vector<float> Parameter::get_parameter_variation(char parameter_key){
  switch (parameter_key){
    case 'T':
      return std::vector<float> {
        0, 0.05, 0.1, 0.15, 0.20, 0.225, 0.25, 0.275, 0.3, 0.35, 0.5, 0.8, 1.0};
    case 'L':
      return std::vector<float> {25, 36, 49, 64};
    case 'H':
      return std::vector<float> {
        1, 2, 4, 8, 15, 25, 35, 49, 1000, 2401};
    case 'v':
      return std::vector<float> {
        1, 2, 3, 4, 5, 10};
    case 'V':
      return std::vector<float> {
        10, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    case 'a':
      return std::vector<float> {
        0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    case 'R':
      return std::vector<float> {0, 1, 2};
    case 'N':
      return std::vector<float> {
        0, 0.0001, 0.0005, 0.001, 0.003, 0.005, 0.007, 0.01, 0.05};
    case 'b':
      return std::vector<float> {
        0, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
    case 'r':
      return std::vector<float> {1, 3, 5, 7, 10};
    case 'S':
      return std::vector<float> {1, 3, 5, 10, 15, 20, 30, 50, 100};
    case 'Q':
      return std::vector<float> {0.01, 0.25, 0.05, 0.75, 0.1, 0.2, 0.3};
    case 'C':
      return std::vector<float> {0, 0.005, 0.01, 0.05, 0.08, 0.1};
    case 'M':
      return std::vector<float> {0, 0.01, 0.05, 0.1, 0.16, 0.2, 0.3,
        0.4, 0.5};
    default:
      cout << "ERROR: invalid character parameter key. '";
      cout << parameter_key << "' is not a valid parameter key" << endl;
      exit(-1);
  }
}

void Parameter::write_to_file(fstream& arquivo){
    arquivo << "### PARAMETERS VALUE ###" << endl;
    arquivo << "### LATTICESIZE = " << latticeSize;
    arquivo << ", NVARIETY = " << numberInitialVariety;
    arquivo << ", NVARIETYHD = " << numberInitialVarietyHD;
    arquivo << ", NRESOURCE = " << numberResources;
    arquivo << ", NRESOURCEDIST = " << numberHabitat;
    arquivo << ", NETWORKTYPE = " << networkType;
    arquivo << ", PROBABILITYCONNECTION = " << probabilyConnectionER;
    arquivo << ", KWT = " << kWT;
    arquivo << ", BETAWT = " << betaWT;
    arquivo << ", MSF = " << mSF;
    arquivo << ", OUTSIDETRADELIMIT = " << outsideTradeLimit;
    arquivo << ", SELECTIONSTRENGTH = " << selectionStrength;
    arquivo << ", ALPHA = " << alpha;
    arquivo << ", NHOUSEHOLD = " << numberHousehold;
    arquivo << ", PROBABILITYNEWVAR = " << probabilityNewVar;
    arquivo << ", PERCENTAGENEWRANDVAR = " << percentageNewRandomVar;
    arquivo << ", CROSSINGDEVIATION = " << crossingDeviation;
    arquivo << ", DEATHPROBABILITY = " << deathProbability;
    arquivo << ", MAXTIME = " << maxTime;
    arquivo << ", TIMEINTERVAL = " << timeInterval;
    arquivo << ", NRUN = " << nRun << " ###" << endl << endl;
}

std::vector<char> Parameter::keys =
  {'T', 'L', 'H', 'v', 'V', 'a', 'R', 'N', 'b', 'r', 'S', 'Q', 'C', 'M'};

#endif
