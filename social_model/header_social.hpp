#include <iostream> // inputs and outputs
#include <random>  // random number
#include <fstream> // handling files
#include <vector>
#include <ctime> // count time of run
#include <boost/dynamic_bitset.hpp> // binary species
using namespace std;

/* Model Parameters */
int LATTICESIZE = 49; //  size of the lattice (ambient parameter)
float DEATHPROB = 0.1; // death probability (ambient parameter)
float MUTATIONPROB = 0.0; // mutation probability (ambient parameter)
int NSPECIE = 100; // number of species (ambient parameter)
int NRESOURCEDIST = 49; // number of different resource vector in grid (model-bio parameter)
const int NSPECIEBYTES = 7; // number of bytes to represent species (patch parameter)
const int NMAXSPECIE = pow(2, NSPECIEBYTES);
const int NRESOURCE = 3;// number of resources (ambient/patch parameter)
const int MAXTIME = 100; // maximum time (model-bio parameter)
const int TIMEINTERVAL = 1; // tic interval in time (model-bio parameter)
const int NRUN = 1;  // number of runs to average (model-bio parameter)
double K[NMAXSPECIE*NRESOURCE]; // vector containing the half saturation constants (patch parameter)
const int NPROPRIETY = 7*7;

/* Imported functions */
static std::random_device rd;
static std::mt19937_64 rand64(rd());
static std::uniform_int_distribution<long> uniIntNSPB(0,NSPECIEBYTES-1);
static std::uniform_int_distribution<long> uniIntSP(1,NSPECIE);
static std::uniform_int_distribution<long> uniIntne(0,4 - 1);
static std::uniform_real_distribution<double> uniFLOAT(0.0,1.0);
static std::normal_distribution<double> gauss(1.0,0.1);

/* Builted class and functions */
class patch;
class ambient;
class propriety;
class variety;

#include "variety.hpp"
#include "patch.hpp"
#include "propriety.hpp"
#include "ambient.hpp"
