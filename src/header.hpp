#include <iostream> // inputs and outputs
#include <random>  // random number
#include <fstream> // handling files
#include <ctime>  // count time of run
#include <vector> // using std vectors
#include <algorithm> // allow to use transform

using namespace std;

/* Imported functions */
static std::random_device rd;
static std::mt19937_64 rand64(rd());
static std::uniform_real_distribution<double> uniFLOAT(0.0,1.0);
static std::normal_distribution<double> gauss(1.0,0.2);

/* Builted class and functions */
struct Variety;
class Patch;
class Ambient;
class DomesticUnity;
class Model;

#include "Variety.hpp"
#include "Patch.hpp"
#include "Ambient.hpp"
#include "DomesticUnity.hpp"
#include "Model.hpp"
