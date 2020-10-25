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
class Parameter;
struct VarietyData;
struct HDParameter;
class Variety;
class Patch;
class Ambient;
class Household;
class WriteState;
class Metrics;
class Result;
class Network;
class Model;
class Data;
class ModelRunner;
bool is_valid_key(char parameter_key);

#include "model/Parameter.hpp"
#include "model_helper/Structs.hpp"
#include "model/Variety.hpp"
#include "model/Patch.hpp"
#include "model/Ambient.hpp"
#include "model/Household.hpp"
#include "model_helper/WriteState.hpp"
#include "model_helper/Metrics.hpp"
#include "model_helper/Result.hpp"
#include "model/Network.hpp"
#include "model/Model.hpp"
#include "Data.hpp"
#include "ModelRunner.hpp"
