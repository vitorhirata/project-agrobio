#include <iostream> // inputs and outputs
#include <random>  // random number
#include <fstream> // handling files
#include <algorithm> // made up algorithms
#include <vector>
#include <ctime> // count time of run
#include <boost/dynamic_bitset.hpp> // binary species
using namespace std;

/* Model Parameters */
int L=50; // 50  size of the lattice
float u = 0.1; // death probability
float m = 0.0; // mutation probability
const int nResource = 10; // number of different resource vector in grid
const int n = 3;// number of resources
const int nK = 7; // number of bytes to represent species
const int k = 100; // number of species
const int T = 100000; // maximum time
const int tic = 1000; // tic interval in time
const int nRun = 10;  // number of runs to average
double K[k*n]; // vector containing the half saturation constants

/* Imported functions */
static std::random_device rd;
static std::mt19937_64 rand64(rd());
static std::uniform_int_distribution<long> uniIntnK(0,nK-1);
static std::uniform_int_distribution<long> uniIntk(1,k);
static std::uniform_int_distribution<long> uniIntne(0,4 - 1);
static std::uniform_real_distribution<double> uniFLOAT(0.0,1.0);
static std::normal_distribution<double> gauss(1.0,0.1);

/* Builted class and functions */
class patch;
class ambient;

#include "patch.hpp"
#include "ambient.hpp"
