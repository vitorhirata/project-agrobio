#ifndef AMBIENT_H
#define AMBIENT_H

/*
Class that groups all model's plot fractions. Responsible for initializing
plot fraction (or patch) varieties, and running death probability for each
plot fraction.
*/

class Ambient{
private:
  const int m_latticeSize;
  const float m_deathProbability;
  void setPatch(int numberResources, int nResourceDistribution,
      int numberInitialVariety, int numberInitialVarietyHD,
      float t_crossingDeviation);
  std::vector<VarietyData> defineInitialVarieties(
      int numberResources, int numberInitialVariety);
  std::vector<std::vector<int> > defineHDindex(
      std::uniform_int_distribution<long> uniIntSP, int numberVarHD);
  int computeHDnumber(int place);
public:
  Patch* grid;
  Ambient(int t_latticeSize, int t_nResourceDistribution,
      int t_numberResources, int t_numberInitialVariety,
      int t_numberInitialVarietyHD, float t_deathProbability,
      float t_crossingDeviation);
  ~Ambient();
  void runDeath(void);
};

// Ambient constructor. initialize parameters, create grid set it's varieties
// and resources
  Ambient::Ambient(int t_latticeSize, int t_nResourceDistribution,
      int t_numberResources, int t_numberInitialVariety,
      int t_numberInitialVarietyHD, float t_deathProbability,
      float t_crossingDeviation)
  : m_latticeSize(t_latticeSize)
  , m_deathProbability(t_deathProbability)
{
  grid = new Patch [m_latticeSize*m_latticeSize];
  setPatch(t_numberResources, t_nResourceDistribution,
      t_numberInitialVariety, t_numberInitialVarietyHD, t_crossingDeviation);
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
    int numberInitialVariety, int numberInitialVarietyHD,
    float t_crossingDeviation){
  std::vector<VarietyData> varietyAvailable = defineInitialVarieties(
      numberResources, numberInitialVariety);
  std::uniform_int_distribution<long> uniIntSP(0,numberInitialVariety-1);
  std::uniform_int_distribution<long> uniIntSPhd(0,numberInitialVarietyHD-1);
  std::vector<std::vector<int> > indexVarietyHD = defineHDindex(
      uniIntSP, numberInitialVarietyHD);

  if(nResourceDistribution == m_latticeSize*m_latticeSize){
    std::vector<float> resources(numberResources);
    for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
      for(int j = 0; j < numberResources; ++j){
        resources[j] = gaussRes(rand64);
        while(resources[j] < 0 || resources[j] > 1)
          resources[j] = gaussRes(rand64);
      }
      int varietyIdx = indexVarietyHD[computeHDnumber(i)][uniIntSPhd(rand64)];
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
      int varietyIdx = indexVarietyHD[computeHDnumber(i)][uniIntSPhd(rand64)];
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
        int idxVar = indexVarietyHD[
          computeHDnumber(lin*m_latticeSize+col)][uniIntSPhd(rand64)];
        grid[lin*m_latticeSize+col].initializePatch(
            resources[idxRes], varietyAvailable[idxVar], t_crossingDeviation);
      }
    }
  }
}

std::vector<std::vector<int> > Ambient::defineHDindex(
    std::uniform_int_distribution<long> uniIntSP, int numberVarHD){
  std::vector<std::vector<int> > indexHD(49, std::vector<int>(numberVarHD));
  for(uint i = 0; i < 49; ++i)
    for(uint j = 0; j < numberVarHD; ++j)
      indexHD[i][j] = uniIntSP(rand64);
  return indexHD;
}

// Receives a patch index and return the household that owns this patch
int Ambient::computeHDnumber(int place){
  int sizeGrid = 49;
  int sizeHD = sqrt(sizeGrid);
  int lin = place / sizeGrid;
  int col = place % sizeGrid;
  return sizeHD * (lin / sizeHD) + col / sizeHD;
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

// For each plot fraction decide if the variety will be killed with a given
// probability
void Ambient::runDeath(void){
  for(int i = 0; i < m_latticeSize*m_latticeSize; ++i){
    if(uniFLOAT(rand64) < m_deathProbability - 0.17 * grid[i].productivity)
      grid[i].killVariety();
  }
}

#endif
