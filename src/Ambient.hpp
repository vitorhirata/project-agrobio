#ifndef AMBIENT_H
#define AMBIENT_H

// Ambient class. Contain the grid and handle environment container
class Ambient{
private:
  const int m_latticeSize;
  const int m_numberMaxVariety;
  const Variety* m_variety;
  void setGlobalResource(int numberResources, int nResourceDistribution);
public:
  Patch* grid;
  Ambient(int t_latticeSize, int t_numberInitialVariety, int t_numberMaxVariety, int t_nResourceDistribution, int t_numberResources, Variety* t_variety);
  ~Ambient();
  void computeAllFitness(void);
  int countSpecie(void);
};

// Ambient constructor. initialize parameters, create grid set it's varieties and resources
  Ambient::Ambient(int t_latticeSize, int t_numberInitialVariety, int t_numberMaxVariety, int t_nResourceDistribution, int t_numberResources, Variety* t_variety)
  : m_latticeSize(t_latticeSize)
  , m_numberMaxVariety(t_numberMaxVariety)
  , m_variety(t_variety)
{
  std::uniform_int_distribution<long> uniIntSP(0,t_numberInitialVariety-1);
  grid = new Patch [m_latticeSize*m_latticeSize];
  for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
    grid[i].setVariety(uniIntSP(rand64));
  }
  setGlobalResource(t_numberResources, t_nResourceDistribution);
}

// Ambient destructor. Free grid memory and set it to nullptr
Ambient::~Ambient(){
  delete[] grid;
  grid = nullptr;
}

// Set the resources along each Patch. 3 options based on the nResourceDistribution, if it is 1 the same resource
// is using in the hole grid, if it is m_latticeSize^2 each Patch have different resources, and if it is a multiple
// of m_latticeSize a division is made to allocate the resource equally. Raises erros if none of that options is true.
void Ambient::setGlobalResource(int numberResources, int nResourceDistribution){
  if(nResourceDistribution == m_latticeSize*m_latticeSize){
    std::vector<float> resources(numberResources);
    for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
      for(int j = 0; j < numberResources; ++j)
        resources[j] = uniFLOAT(rand64);
      grid[i].setLocalResource(resources);
    }
  }
  else if(nResourceDistribution == 1){
    std::vector<float> resources(numberResources);
    for(int i = 0; i < numberResources; ++i)
      resources[i] = uniFLOAT(rand64);

    for(int i = 0; i < m_latticeSize*m_latticeSize; ++i)
      grid[i].setLocalResource(resources);
  }
  else if(m_latticeSize % nResourceDistribution == 0){
    std::vector<vector<float>> resources;
    for(int i = 0; i < nResourceDistribution; ++i){
      std::vector<float> temp(numberResources);
      for(int j = 0; j < numberResources; ++j)
        temp[j] = uniFLOAT(rand64);
      resources.push_back(temp);
    }

    int size = m_latticeSize / nResourceDistribution;
    for(int lin = 0; lin < m_latticeSize; ++lin){
      int idx = lin / size;
      for(int col = 0; col < m_latticeSize; ++col)
        grid[lin*m_latticeSize+col].setLocalResource(resources[idx]);
    }
  }
  else{
    cout << "ERROR in Ambient::setGlobalResource: " << nResourceDistribution << " is not multiple of " << m_latticeSize << "." << endl;
    exit(-1);
  }
}

// Compute fitness for all Patch
void Ambient::computeAllFitness(void){
  for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
    grid[i].computeLocalFitness(m_variety);
  }
}

// Count the number of different varieties in the grid, return a int to that number
int Ambient::countSpecie(void){
  std::vector<bool> varietyAvailability(m_numberMaxVariety, false);
  for(int i = 0; i < m_latticeSize*m_latticeSize; ++i)
    varietyAvailability[grid[i].plantedVariety] = true;

  int sum = 0;
  for (int i = 0; i < m_numberMaxVariety; ++i)
    sum += varietyAvailability[i];
  return sum;
}


#endif
