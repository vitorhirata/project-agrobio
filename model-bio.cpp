#include <iostream> // inputs and outputs
#include <random>  // random number
#include <cmath>  // math operation
#include <fstream> // handling files
#include <algorithm> // made up algorithms
#include <ctime> // count time of run
#include <boost/dynamic_bitset.hpp> // binary species
#include <vector>
using namespace std;

int L=50; // 50  size of the lattice
float u = 0.1; // death probability
float m = 0.0; // mutation probability
const int n = 3;// number of resources
const int k = 100; // number of species
const int nK = 7; // number of bytes to represent species
const int T = 100000; // maximum time
const int tic = 1000; // tic interval in time
const int nRun = 2;  // number of runs to average
class patch;
double K[k*n]; // vector containing the half saturation constants

static std::random_device rd;
static std::mt19937_64 rand64(rd());
static std::uniform_int_distribution<long> uniIntkb(0,nK-1);
static std::uniform_int_distribution<long> uniIntk(1,k);
static std::uniform_int_distribution<long> uniIntne(0,4 - 1);
static std::uniform_real_distribution<double> uniFLOAT(0.0,1.0);
static std::normal_distribution<double> gauss(1.0,0.1);


// Each patch equivales to one produceble site, and contain up to one plantation an the resources present in the soil.
class patch{
  std::vector<float> resource; // each number is the amount of the i'th resource
  public:
    boost::dynamic_bitset<> specie; // binary array
    void kill(void);
    double fitness(void);
    void mutate(void);
    patch(void);
    ~patch();
};

// Constructor, initialize resouce with n-sized vector and uniform distribution [0,1], and specie as an int [0,k], convert to an array of binary (size nK bytes).
patch::patch(void){

  for(int i=0;i<n;i++)
    resource.push_back(uniFLOAT(rand64));
  specie = boost::dynamic_bitset<>(nK,uniIntk(rand64));
}

// Destructor, free the memory of resource and specie.
patch::~patch(void){
  std::vector<float>().swap(resource);
  specie = boost::dynamic_bitset<>(0);
}

// Uses XOR to make specie an empty array of binaries. No output.
void patch::kill(void){
  specie ^= specie;
}

// Return the fitness of the population living in the site. Computed using the Monod equation.s
double patch::fitness(void){
  double min = resource[0]/(K[(specie.to_ulong()-1)*n]+resource[0]);
  for (int i=1;i<n;i++){
    double temp = resource[i]/(K[(specie.to_ulong()-1)*n+i]+resource[i]);
    if (temp < min)
      min = temp;
  }
  return min;
} //pode ser melhorado, o fitness de uma especie em um grid vai ser sempre igual (na real isso vale so se o recurso for constante)

// Cause a mutation, selecting randomly one 'gene' and changing it
void patch::mutate(void){
  int temp = uniIntkb(rand64);
  specie[temp] = !specie[temp];
}



bool haveNeighbor(patch grid[], int x, int y, int* x_N, int* y_N);
void iterate(patch grid[]);
int countSpecie(patch grid[]);
int Run_standart(void);
int Run_varParam(char param, std::vector<float> paramList);

// Receives the grid and iterate one time, passing over all the sites of the grid.
void iterate(patch grid[]){
  int x, y, x_Neigh, y_Neigh;
  std::vector<int> x_list(L), y_list(L);

  // Initialize the list over which the program will pass updating the program.
  for(int i=0;i<L;i++)
    x_list[i]=y_list[i]=i;
  std::random_shuffle(x_list.begin(),x_list.end());
  std::random_shuffle(y_list.begin(),y_list.end());


  for(int i=0;i<L;i++)
    for(int j=0;j<L;j++){
      x = x_list[i];
      y = y_list[i];
      if(grid[x*L+y].specie.any()){
        if(uniFLOAT(rand64) < u) // death probability
          grid[x*L+y].kill();
        else
          if(haveNeighbor(grid, x, y, &x_Neigh,&y_Neigh) && uniFLOAT(rand64) < grid[x*L+y].fitness()){
            grid[x_Neigh*L+y_Neigh].specie = grid[x*L+y].specie;
            if(uniFLOAT(rand64) < m)
              grid[x_Neigh*L+y_Neigh].mutate();
          }
      }
    }
}

// Receives the grid, one site (x,y), and a pair of pointers (*x_N, *y_N) where the result will be put. Return false if there aren't empty neighboor and true if there is at least one empy neighboor. Sampling one random site if there are more than one.
bool haveNeighbor(patch grid[], int x, int y, int* x_N, int* y_N){
  int xP = x+1, xM = x-1, yP = y+1, yM = y-1;
  std::vector<int> PossibleX(4), PossibleY(4);
  int nPossible = 0;

  if(x == L - 1)
		xP = 0;
	else if(x == 0)
		xM = L-1;
	if(y == L - 1)
		yP = 0;
	else if(y == 0)
		yM = L-1;


  PossibleX[nPossible] = xP;
  PossibleY[nPossible] = y;
  nPossible += !grid[xP*L+y].specie.any();

  PossibleX[nPossible] = xM;
  PossibleY[nPossible] = y;
  nPossible += !grid[xM*L+y].specie.any();

  PossibleX[nPossible] = x;
  PossibleY[nPossible] = yP;
  nPossible += !grid[x*L+yP].specie.any();

  PossibleX[nPossible] = x;
  PossibleY[nPossible] = yM;
  nPossible += !grid[x*L+yM].specie.any();

  if (nPossible==0)
    return false;
  if (nPossible==1){
    *x_N = PossibleX[0];
    *y_N = PossibleY[0];
    return true;
  }
  else{
    uniIntne.param(std::uniform_int_distribution<long>::param_type(0, nPossible-1));
    int nChoosen = uniIntne(rand64);
    *x_N = PossibleX[nChoosen];
    *y_N = PossibleY[nChoosen];
    return true;
  }
}

// Cont the number of observed species at the actual time, on the grid.
int countSpecie(patch grid[]){
  int nDifferentS=0;
  std::vector<bool> alreadyExist(k+1,false);

  for(int i=0;i<L;i++)
    for(int j=0;j<L;j++){
      int sp=grid[i*L+j].specie.to_ulong();
      if(!alreadyExist[sp]){
        alreadyExist[sp] = true;
        nDifferentS++;
      }
    }
  if(alreadyExist[0] == true)
    return nDifferentS-1;
  else
    return nDifferentS;
}

// Run the standart model, saving a txt with the evolution of the number of species.
int Run_standart(void){
  std::vector<int> result(T/tic,0);
  int i, j;
  fstream arquivo;
  arquivo.open("standarts.txt",ios::out);

  // Rodo nRun rodadas
  for (int run=0; run < nRun; run++){
    patch* grid;
    grid = new (nothrow) patch[L*L];
    if (grid == nullptr){
      cout << "Erro na alocacao de grid" << endl;
      return -1;
    }
    for (i=0;i<k;i++)
      for (j=0;j<n;j++)
        K[i*n+j] = gauss(rand64);

    clock_t tStart = clock();
    for (int t=0;t<T;t++){
      iterate(grid);
      if (t % tic == 0)
        result[t/tic] += countSpecie(grid);
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    delete[] grid;
  }

  for (int t=0;t<T/tic;t++)
    arquivo << t*tic << "; " << result[t]/nRun << endl;
  arquivo.close();
  return 0;
}

// Run the model for some parameter varying, saving a txt with the evolution of the number of species for each parameter value.
// at the moment working only for m variation
int Run_varParam(char param, std::vector<float> paramList){
  std::vector<int> result((T/tic)*paramList.size(),0);
  fstream arquivo;
  arquivo.open(std::string ("varParam_")+param+".txt",ios::out);
  int i, j, idxParam;

  if (param != 'm' && param != 'u' && param != 'L'){
    cout << "Invalid parameter, only m, u and L can vary." << endl;
    return -1;
  }

  for (idxParam=0; idxParam < paramList.size(); idxParam++){
    switch (param){
      case 'm':
        m=paramList[idxParam];
        break;
      case 'u':
        u=paramList[idxParam];
        break;
      case 'L':
        L=paramList[idxParam];
        break;
    }

  // Run nRun rounds
    clock_t tStart = clock();
    for (int run=0; run < nRun; run++){
      patch* grid;
      grid = new (nothrow) patch[L*L];
      if (grid == nullptr){
        cout << "Erro na alocacao de grid" << endl;
        return -1;
      }
      for (i=0;i<k;i++)
        for (j=0;j<n;j++)
          K[i*n+j] = gauss(rand64);

      for (int t=0;t<T;t++){
        iterate(grid);
        if (t % tic == 0)
          result[(T/tic)*idxParam+(t/tic)] += countSpecie(grid);
      }
      delete[] grid;
    }
    cout << "Finish " << param << " = " << paramList[idxParam] << ". Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  }

  for (int t=0;t<T/tic;t++){
    arquivo << t*tic;
    for (idxParam=0; idxParam < paramList.size(); idxParam++)
        arquivo << "; " << result[(T/tic)*idxParam+t]/nRun;
    arquivo << endl;
  }
  arquivo.close();
  return 0;
}

int main(){
  //Run_standart();
  Run_varParam('u', {0.01,0.1});
  return 0;
}
