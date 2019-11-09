#ifndef AMBIENT_H
#define AMBIENT_H

// Ambient class. Contain the grid and handle environment container
class Ambient{
private:
  const int m_latticeSize;
  const float m_deathProbability;
  void setPatch(int numberResources, int nResourceDistribution,
      int numberInitialVariety, int numberInitialVarietyDU,
      float t_crossingDeviation);
  std::vector<VarietyData> defineInitialVarieties(
      int numberResources, int numberInitialVariety);
  std::vector<std::vector<int> > defineDUindex(
      std::uniform_int_distribution<long> uniIntSP, int numberVarDU);
  int computeDUnumber(int place);
public:
  Patch* grid;
  Ambient(int t_latticeSize, int t_nResourceDistribution,
      int t_numberResources, int t_numberInitialVariety,
      int t_numberInitialVarietyDU, float t_deathProbability,
      float t_crossingDeviation);
  ~Ambient();
  int countSpecie(void);
  void runDeath(void);
};

// Ambient constructor. initialize parameters, create grid set it's varieties
// and resources
  Ambient::Ambient(int t_latticeSize, int t_nResourceDistribution,
      int t_numberResources, int t_numberInitialVariety,
      int t_numberInitialVarietyDU, float t_deathProbability,
      float t_crossingDeviation)
  : m_latticeSize(t_latticeSize)
  , m_deathProbability(t_deathProbability)
{
  grid = new Patch [m_latticeSize*m_latticeSize];
  setPatch(t_numberResources, t_nResourceDistribution,
      t_numberInitialVariety, t_numberInitialVarietyDU, t_crossingDeviation);
}

// Ambient destructor. Free grid memory and set it to nullptr
Ambient::~Ambient(){
  delete[] grid;
  grid = nullptr;
}

// Set the resources along each Patch. 3 options based on the
// nResourceDistribution, if it is 1 the same resource is using in the hole
// grid, if it is m_latticeSize^2 each Patch have different resources, and if
// it is a multiple of m_latticeSize a division is made to allocate the
// resource equally. Raises erros if none of that options is true.
void Ambient::setPatch(int numberResources, int nResourceDistribution,
    int numberInitialVariety, int numberInitialVarietyDU,
    float t_crossingDeviation){
  std::vector<VarietyData> varietyAvailable = defineInitialVarieties(
      numberResources, numberInitialVariety);
  std::uniform_int_distribution<long> uniIntSP(0,numberInitialVariety-1);
  std::uniform_int_distribution<long> uniIntSPdu(0,numberInitialVarietyDU-1);
  std::vector<std::vector<int> > indexVarietyDU = defineDUindex(
      uniIntSP, numberInitialVarietyDU);

  if(nResourceDistribution == m_latticeSize*m_latticeSize){
    std::vector<float> resources(numberResources);
    for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
      for(int j = 0; j < numberResources; ++j){
        resources[j] = gaussRes(rand64);
        while(resources[j] < 0 || resources[j] > 1)
          resources[j] = gaussRes(rand64);
      }
      int varietyIdx = indexVarietyDU[computeDUnumber(i)][uniIntSPdu(rand64)];
      grid[i].initializePatch(resources, varietyAvailable[varietyIdx],
          t_crossingDeviation);
    }
  }
  else if(nResourceDistribution == 1){
    std::vector<float> resources(numberResources);
    for(int i = 0; i < numberResources; ++i){
      resources[i] = gaussRes(rand64);
      while(resources[i] < 0 || resources[i] > 1)
        resources[i] = gaussRes(rand64);
    }

    for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
      int varietyIdx = indexVarietyDU[computeDUnumber(i)][uniIntSPdu(rand64)];
      grid[i].initializePatch(resources, varietyAvailable[varietyIdx],
          t_crossingDeviation);
    }
  }
  else{
    std::vector<vector<float>> resources;
    for(int i = 0; i < nResourceDistribution; ++i){
      std::vector<float> temp(numberResources);
      for(int j = 0; j < numberResources; ++j){
        temp[j] = gaussRes(rand64);
        while(temp[j] < 0 || temp[j] > 1)
          temp[j] = gaussRes(rand64);
      }
      resources.push_back(temp);
    }

    int sizeH = ceil(m_latticeSize / (nResourceDistribution / 2.0));
    int sizeV = ceil(m_latticeSize / 2.0);
    for(int lin = 0; lin < m_latticeSize; ++lin){
      for(int col = 0; col < m_latticeSize; ++col){
        int idxRes = (lin / sizeV) * nResourceDistribution / 2 + col / sizeH;
        int idxVar = indexVarietyDU[
          computeDUnumber(lin*m_latticeSize+col)][uniIntSPdu(rand64)];
        grid[lin*m_latticeSize+col].initializePatch(
            resources[idxRes], varietyAvailable[idxVar], t_crossingDeviation);
      }
    }
  }
}

// Return the a vector of vector of int, containing numberVarDU random index
std::vector<std::vector<int> > Ambient::defineDUindex(
    std::uniform_int_distribution<long> uniIntSP, int numberVarDU){
  std::vector<std::vector<int> > indexDU(49, std::vector<int>(numberVarDU));
  for(uint i = 0; i < 49; ++i)
    for(uint j = 0; j < numberVarDU; ++j)
      indexDU[i][j] = uniIntSP(rand64);
  return indexDU;
}

// Receives on patch index and return the domestic unity that own this patch
int Ambient::computeDUnumber(int place){
  int sizeGrid = 49;
  int sizeDU = sqrt(sizeGrid);
  int lin = place / sizeGrid;
  int col = place % sizeGrid;
  return sizeDU * (lin / sizeDU) + col / sizeDU;
}

std::vector<VarietyData> Ambient::defineInitialVarieties(int numberResources,
    int numberInitialVariety){
  std::vector<VarietyData> varietyAvailable (numberInitialVariety);

  for(int i = 0; i < numberInitialVariety; ++i){
    for(int j = 0; j < numberResources; ++j){
      varietyAvailable[i].halfSaturation.push_back(gaussK(rand64));
      while(varietyAvailable[i].halfSaturation[j] < 0 ||
          varietyAvailable[i].halfSaturation[j] > 1)
        varietyAvailable[i].halfSaturation[j] = gaussK(rand64);
    }
    varietyAvailable[i].quality = gaussQ(rand64);
      while(varietyAvailable[i].quality < 0 ||
          varietyAvailable[i].quality > 1)
        varietyAvailable[i].quality = gaussQ(rand64);
  }
  return varietyAvailable;
}

// Count the number of different varieties in the grid, return a int to
// that number
int Ambient::countSpecie(void){
  std::map<int,bool> varietyAvailability;
  for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
    int varNumber = grid[i].variety.varietyNumber;
    varietyAvailability[varNumber] = true;
  }
  if(varietyAvailability.count(-1))
    return varietyAvailability.size() - 1;
  return varietyAvailability.size();
}

void Ambient::runDeath(void){
  for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
    if(uniFLOAT(rand64) < m_deathProbability)
      grid[i].killVariety();
  }
}

#endif
