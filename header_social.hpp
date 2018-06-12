#include <iostream> // inputs and outputs
#include <random>  // random number
#include <fstream> // handling files
#include <vector>
#include <ctime> // count time of run
#include <string>
#include <algorithm>
#include "EasyBMP.cpp" // BITMAP
using namespace std;

/* Model Parameters */
int LATTICESIZE = 49; //  size of the lattice (ambient parameter)
float MUTATIONPROB = 0.0001; // mutation probability (ambient parameter)
int NSPECIE = 50; // number of species (ambient parameter)
int NRESOURCEDIST = 1; // number of different resource vector in grid (ambient parameter)
float ALPHA = 1.0;
const int NMAXSPECIE = 100;
const int NRESOURCE = 3;// number of resources (ambient/patch parameter)
int NPROPRIETY = LATTICESIZE;
const int MAXTIME = 10000; // maximum time (model-social parameter)
const int TIMEINTERVAL = 100; // tic interval in time (model-social parameter)
const int NRUN = 15;  // number of runs to average (model-social parameter)


/* Imported functions */
static std::random_device rd;
static std::mt19937_64 rand64(rd());
static std::uniform_int_distribution<long> uniIntNSP(0,NMAXSPECIE-1);
static std::uniform_int_distribution<long> uniIntSP(1,NSPECIE);
static std::uniform_int_distribution<long> uniIntne(0,4 - 1);
static std::uniform_real_distribution<double> uniFLOAT(0.0,1.0);
static std::normal_distribution<double> gauss(1.0,0.1);
static std::normal_distribution<double> gaussResistence(0.5,0.15);

/* Builted class and functions */
class patch;
class ambient;
class propriety;
class variety;

#include "variety.hpp"
#include "patch.hpp"
#include "propriety.hpp"
#include "ambient.hpp"
