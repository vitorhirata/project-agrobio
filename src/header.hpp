#include <iostream> // inputs and outputs
#include <random>  // random number
#include <fstream> // handling files
#include <ctime>  // count time of run
#include <vector> // using std vectors
#include <algorithm> // allow to use transform
#include <tuple> // allow tuple return
#include "EasyBMP.cpp" // BITMAP
#include <map>
#include <cmath>

using namespace std;

/* Imported functions */
static std::random_device rd;
static std::mt19937_64 rand64(rd());
static std::uniform_real_distribution<double> uniFLOAT(0.0,1.0);
static std::uniform_int_distribution<long> uniIntPlace(0,49-1);
static std::uniform_int_distribution<long> uniIntChangeHdProduction(0,20-1);
static std::normal_distribution<double> gaussPref(0.5,0.1);
static std::normal_distribution<double> gaussRes(0.6,0.3);
static std::normal_distribution<double> gaussK(0.5,0.15);
static std::normal_distribution<double> gaussQ(0.5,0.15);

/* Builted class and functions */
struct VarietyData;
struct Parameter;
struct Result;
class Variety;
class Patch;
class Ambient;
class Household;
class Network;
class Model;
namespace metrics{
  void sumResults(Result* result, Result* resultTemp);
  float computeVarietyMeanProfile(Household* household,
      const int t_numberHousehold, const int t_HDsize);
  std::vector<float> computeHDprofile(Household* household,
      const int t_numberHousehold, const int t_HDsize);
  std::vector<float> computeVarietyProfile(
      Household* household, const int t_numberHousehold);
  std::vector<float> computeVarietyQuantity(Household* household,
      const int t_numberHousehold, const int t_latticeSize);
  float computeBergerParker(Household* household,
      const int t_numberHousehold, const int t_latticeSize);
  float computeBergerParkerHD(Household* household,
      const int t_numberHousehold);
  float computeShannon(Household* household,
      const int t_numberHousehold, const int t_latticeSize);
  float computeSimpson(Household* household,
      const int t_numberHousehold, const int t_latticeSize);
  std::vector<float> computePunctuationAverage(Household* household,
      const int t_numberHousehold);
  std::vector<float> computeProductivityProfile(
      Patch* t_grid, const int t_latticeSize);
  std::vector<float> computeQualityProfile(
      Patch* t_grid, const int t_latticeSize);
  float computeCorrelation(Household* household, int numberHD);
  void printState(int t, Patch* grid, const int t_latticeSize);
  void floatToRGB(int n, int* R, int* G, int* B);
  void printParameters(fstream& arquivo, Parameter parameter);
}
namespace worker{
  void Run_standard(void);
  void Run_plot(void);
  void Run_varParamFixedPoints(char param);
  void Run_varParam(char param, std::vector<float> paramList);
  fstream openFile(
      std::string fileName, std::string varNames, Parameter parameter);
}

#include "model/Structs.hpp"
#include "model/Variety.hpp"
#include "model/Patch.hpp"
#include "model/Ambient.hpp"
#include "model/Household.hpp"
#include "model/Network.hpp"
#include "model/Model.hpp"
#include "model/Metrics.hpp"
#include "Worker.hpp"
