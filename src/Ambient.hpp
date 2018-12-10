#ifndef AMBIENT_H
#define AMBIENT_H

// Ambient class. Contain the grid and handle environment container
class Ambient{
private:
  const int m_latticeSize;
  void setPatch(int numberResources, int nResourceDistribution, int numberInitialVariety);
  std::vector<VarietyData> defineInitialVarieties(int numberResources, int numberInitialVariety);
public:
  Patch* grid;
  Ambient(int t_latticeSize, int t_nResourceDistribution, int t_numberResources, int t_numberInitialVariety);
  ~Ambient();
  void computeAllFitness(void);
  int countSpecie(void);
};

// Ambient constructor. initialize parameters, create grid set it's varieties and resources
  Ambient::Ambient(int t_latticeSize, int t_nResourceDistribution, int t_numberResources, int t_numberInitialVariety)
  : m_latticeSize(t_latticeSize)
{
  grid = new Patch [m_latticeSize*m_latticeSize];
  setPatch(t_numberResources, t_nResourceDistribution, t_numberInitialVariety);
}

// Ambient destructor. Free grid memory and set it to nullptr
Ambient::~Ambient(){
  delete[] grid;
  grid = nullptr;
}

// Set the resources along each Patch. 3 options based on the nResourceDistribution, if it is 1 the same resource
// is using in the hole grid, if it is m_latticeSize^2 each Patch have different resources, and if it is a multiple
// of m_latticeSize a division is made to allocate the resource equally. Raises erros if none of that options is true.
void Ambient::setPatch(int numberResources, int nResourceDistribution, int numberInitialVariety){
  std::vector<VarietyData> varietyAvailable = defineInitialVarieties(numberResources, numberInitialVariety);
  std::uniform_int_distribution<long> uniIntSP(0,numberInitialVariety-1);

  if(nResourceDistribution == m_latticeSize*m_latticeSize){
    std::vector<float> resources(numberResources);
    for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
      for(int j = 0; j < numberResources; ++j)
        resources[j] = uniFLOAT(rand64);
      grid[i].initializePatch(resources, varietyAvailable[uniIntSP(rand64)]);
    }
  }
  else if(nResourceDistribution == 1){
    std::vector<float> resources(numberResources);
    for(int i = 0; i < numberResources; ++i)
      resources[i] = uniFLOAT(rand64);

    for(int i = 0; i < m_latticeSize*m_latticeSize; ++i)
      grid[i].initializePatch(resources, varietyAvailable[uniIntSP(rand64)]);
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
        grid[lin*m_latticeSize+col].initializePatch(resources[idx], varietyAvailable[uniIntSP(rand64)]);
    }
  }
  else{
    cout << "ERROR in Ambient::setGlobalResource: " << nResourceDistribution << " is not multiple of " << m_latticeSize << "." << endl;
    exit(-1);
  }
}

std::vector<VarietyData> Ambient::defineInitialVarieties(int numberResources, int numberInitialVariety){
  std::vector<VarietyData> varietyAvailable (numberInitialVariety);

  for(int i = 0; i < numberInitialVariety; ++i){
    for(int j = 0; j < numberResources; ++j){
      varietyAvailable[i].halfSaturation.push_back(gauss(rand64));
      while(!(varietyAvailable[i].halfSaturation[j] > 0 && varietyAvailable[i].halfSaturation[j] < 1))
        varietyAvailable[i].halfSaturation[j] = gauss(rand64);
    }
    varietyAvailable[i].appearence = uniFLOAT(rand64);
  }
  return varietyAvailable;
}

// Compute fitness for all Patch
void Ambient::computeAllFitness(void){
  for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
    grid[i].computeLocalFitness();
  }
}

// Count the number of different varieties in the grid, return a int to that number
int Ambient::countSpecie(void){
  std::vector<int> varietyList {grid[0].variety.varietyNumber};
  for(int i = 1; i < m_latticeSize*m_latticeSize; ++i){
    int varNumber = grid[i].variety.varietyNumber;
    vector<int>::iterator vectorIterator = std::lower_bound(varietyList.begin(), varietyList.end(), varNumber);
    if(*vectorIterator != varNumber)
      varietyList.insert(vectorIterator, varNumber);
  }
  return varietyList.size();
}


#endif
