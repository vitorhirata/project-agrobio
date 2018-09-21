#include <iostream> // inputs and outputs
#include <random>  // random number
#include <fstream> // handling files
#include <ctime>  // count time of run
#include <vector> // using std vectors
#include <algorithm> // allow to use transform
#include <tuple> // allow tuple return
#include "EasyBMP.cpp" // BITMAP

using namespace std;

/* Imported functions */
static std::random_device rd;
static std::mt19937_64 rand64(rd());
static std::uniform_real_distribution<double> uniFLOAT(0.0,1.0);

/* Builted class and functions */
struct Variety;
struct Parameter;
class Patch;
class Ambient;
class DomesticUnity;
class Model;
namespace metrics{
  std::vector<float> computeVarietyProfile(Patch* t_grid, Variety* variety, const int t_latticeSize, const int t_numberVariety);
  void printState(int t, Patch* grid, const int t_latticeSize);
  void floatToRGB(int n, int* R, int* G, int* B);
  void printParameters(fstream& arquivo, Parameter parameter);
}
namespace worker{
  void Run_standard(void);
  void Run_plot(void);
  void Run_varParamFixedPoints(char param);
  void Run_varParam(char param, std::vector<float> paramList);
}

#include "Structs.hpp"
#include "Patch.hpp"
#include "Ambient.hpp"
#include "DomesticUnity.hpp"
#include "Model.hpp"
#include "Metrics.hpp"
#include "Worker.hpp"
