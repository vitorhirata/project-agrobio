#ifndef PARAMETER_H
#define PARAMETER_H

/*
Class responsible for dealing with the model's parameters. Including parameter
initialization, variating parameters and writting parameters to file.
*/

class Parameter{
public:
  int latticeSize;
  int numberInitialVariety; // Initial number of varieties in the community
  int numberInitialVarietyHD; // Initial number of varieties per household
  int numberResources; // Number of existing resources
  int numberHabitat; // Ambient heterogeneity
  int maxTime; // Number of agricultural cycles (iterations)
  int timeInterval; // Time interval in which metrics are computed
  int numberHousehold; // Number of households
  int nRun; // Number of simulations runned
  int networkType; // Algorithm used to create network, 0 is Erdos-Renyi, 1 is Watts-Strogatz and 2 is Barabasi-Albert
  int mSF; // Number of initial connected nodes of the Barabasi-Albert model
  int kWT; // Number of connections of the initial regular network of the Watts-Strogatz model
  float betaWT; // Rewiring probability of the Watts-Strogatz model
  float probabilyConnectionER; // Probability of connection of the Erdos-Renyi model
  float outsideTradeLimit; // Base exchange probability
  int selectionStrength; // Intensity of selection
  float alpha; // Importance of productivity on variety score
  float probabilityNewVar; // Total probability of new variety
  float percentageNewRandomVar; // Percentage of new variety randomly initialized
  float crossingDeviation; // Traits matting deviation
  float deathProbability; // Base death probability
  int household_size();
  int number_patches();
  static std::vector<char> keys;
  Parameter();
  void set_parameter(char parameter_key, float value);
  void write_to_file(fstream& arquivo);
  static std::vector<float> get_parameter_variation(char parameter_key);
};

// Parameter constructor, defines the default values used in the model
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
  , outsideTradeLimit(0.2)
  , selectionStrength(20)
  , alpha(0.6)
  , probabilityNewVar(0.25)
  , percentageNewRandomVar(0.025)
  , crossingDeviation(0.005)
  , deathProbability(0.35) {}



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
      break;
    case 'm':
      break;
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
        0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
        0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0
      };
    case 'L':
      return std::vector<float> {16, 25, 36, 49, 64};
    case 'H':
      return std::vector<float> {1, 2, 3, 4, 5, 6, 7, 8};
    case 'v':
      return std::vector<float> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    case 'V':
      return std::vector<float> {
        5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90,
        95, 100
      };
    case 'a':
      return std::vector<float> {
        0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
        0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0
      };
    case 'R':
      return std::vector<float> {0, 1, 2};
    case 'N':
      return std::vector<float> {
        0, 0.01, 0.02, 0.03, 0.04, 0.05, 0.06, 0.07, 0.08, 0.09, 0.1
      };
    case 'b':
      return std::vector<float> {
        0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5,
        0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0
      };
    case 'r':
      return std::vector<float> {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    case 'S':
      return std::vector<float> {1, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50};
    case 'Q':
      return std::vector<float> {
        0.025, 0.05, 0.075, 0.1, 0.125, 0.15, 0.175, 0.20, 0.225, 0.25, 0.275,
        0.3, 0.325, 0.35, 0.375, 0.4
      };
    case 'C':
      return std::vector<float> {
        0, 0.0025, 0.005, 0.0075, 0.01, 0.0125, 0.015, 0.0175, 0.02,
        0.0225, 0.025, 0.0275, 0.3
      };
    case 'M':
      return std::vector<float> {
        0, 0.025, 0.05, 0.075, 0.1, 0.125, 0.15, 0.175, 0.2, 0.225, 0.25,
        0.275, 0.3, 0.325, 0.35, 0.375, 0.4, 0.425, 0.45, 0.475, 0.5
      };
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

int Parameter::household_size(){
  int household_size = latticeSize / sqrt(numberHousehold);
  household_size = pow(household_size, 2);
  return household_size;
}

int Parameter::number_patches(){
  return latticeSize * latticeSize;
}

std::vector<char> Parameter::keys =
  {'T', 'L', 'H', 'v', 'V', 'a', 'R', 'N', 'b', 'r', 'S', 'Q', 'C', 'M'};

#endif
