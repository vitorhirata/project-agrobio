#include <iostream> // inputs and outputs
#include <random>  // random number
#include <fstream> // handling files
#include <ctime>  // count time of run
#include <vector> // using std vectors
#include <algorithm> // allow to use transform
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

#include "Structs.hpp"
#include "Patch.hpp"
#include "Ambient.hpp"
#include "DomesticUnity.hpp"
#include "Metrics.hpp"
#include "Model.hpp"
