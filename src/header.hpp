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
static std::normal_distribution<double> gauss(0.5,0.1);

/* Builted class and functions */
struct VarietyData;
struct Parameter;
struct Result;
class Variety;
class Patch;
class Ambient;
class DomesticUnity;
class Network;
class Model;
namespace metrics{
  void sumResults(Result* result, Result* resultTemp);
  float computeVarietyMeanProfile(DomesticUnity* domesticUnity, const int t_numberDomesticUnity, const int t_DUsize);
  std::vector<float> computeVarietyProfile(DomesticUnity* domesticUnity, const int t_numberDomesticUnity, const int t_DUsize);
  std::vector<float> computePunctuationAverage(DomesticUnity* domesticUnity, const int t_numberDomesticUnity);
  std::vector<float> computeFitnessProfile(Patch* t_grid, const int t_latticeSize);
  std::vector<float> computeAppearenceProfile(Patch* t_grid, const int t_latticeSize);
  void printState(int t, Patch* grid, const int t_latticeSize);
  void floatToRGB(int n, int* R, int* G, int* B);
  void printParameters(fstream& arquivo, Parameter parameter);
}
namespace worker{
  void Run_standard(void);
  void Run_plot(void);
  void Run_varParamFixedPoints(char param);
  void Run_varParam(char param, std::vector<float> paramList);
  fstream openFile(std::string fileName, std::string varNames, Parameter parameter);
}

#include "Structs.hpp"
#include "Variety.hpp"
#include "Patch.hpp"
#include "Ambient.hpp"
#include "DomesticUnity.hpp"
#include "Network.hpp"
#include "Model.hpp"
#include "Metrics.hpp"
#include "Worker.hpp"
