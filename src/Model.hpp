#ifndef MODEL_H
#define MODEL_H

class Model{
private:
  const int m_latticeSize; // Size of the lattice
  const int m_numberVariety; // Number of species in initialization
  const int m_numberResources; // Number of existing resources
  const int m_numberResourceDistribution; // Number of different resources in grid
  const int m_maxTime; // Max time of iteration
  const int m_timeInterval; // Interval in which metrics are counted
  const int m_numberDomesticUnity; // Number of domestic unities
  const float m_probabilyConnection; // Probability of connection
  const float m_outsideTradeLimit;
  const float m_insideTradeLimit;

  Ambient* ambient;
  DomesticUnity* domesticUnity;
  Variety* variety;
  void setVariety(void);
  void setAmbient(void);
  void setDomesticUnity(void);
  void iterate(void);
public:
  Model(int t_latticeSize, int t_nVariety, int t_numberResources, int t_nResourceDistribution, int t_maxTime, int t_timeInterval, int t_nDomesticUnity, float t_probabilyConnection, float t_outsideTradeLimit, float t_insideTradeLimit);
  ~Model();
  std::vector<int> runStandard(void);
  std::vector<int> runPlot(void);
};

// Model constructor, receive model parameters, initialize then, and call for each class initialization.
Model::Model(int t_latticeSize, int t_nVariety, int t_numberResources, int t_nResourceDistribution, int t_maxTime, int t_timeInterval, int t_nDomesticUnity, float t_probabilyConnection, float t_outsideTradeLimit, float t_insideTradeLimit)
  : m_latticeSize(t_latticeSize)
  , m_numberVariety(t_nVariety)
  , m_numberResources(t_numberResources)
  , m_numberResourceDistribution(t_nResourceDistribution)
  , m_maxTime(t_maxTime)
  , m_timeInterval(t_timeInterval)
  , m_numberDomesticUnity(t_nDomesticUnity)
  , m_probabilyConnection(t_probabilyConnection)
  , m_outsideTradeLimit(t_outsideTradeLimit)
  , m_insideTradeLimit(t_insideTradeLimit)
{
  setVariety();
  setAmbient();
  setDomesticUnity();
}

// Model destructor, free memory of the pointers and set pointers to nullptr
Model::~Model(){
  delete ambient;
  delete[] domesticUnity;
  delete[] variety;
  ambient = nullptr;
  domesticUnity = nullptr;
  variety = nullptr;
}

// Create variety array and initialize it's K value with random gauss distribution
void Model::setVariety(void){
  variety =  new Variety[m_numberVariety];

  std::vector<float> KTemp(m_numberResources);
  for(int i = 0; i < m_numberVariety; ++i){
    for(int j = 0; j < m_numberResources; ++j)
      KTemp[j] = gauss(rand64);
    variety[i].K = KTemp;
  }
}

// Create ambient, passing the parameters using inside it
void Model::setAmbient(void){
  ambient = new Ambient(m_latticeSize, m_numberVariety, m_numberResourceDistribution, m_numberResources, variety);
}

// Create DomesticUnity array, set indexLinkedDUs, indexOwenedsPatches and pass it to initialize each DomesticUnity
void Model::setDomesticUnity(void){
  domesticUnity =  new DomesticUnity [m_numberDomesticUnity];

  // Set indexLinkedDUs (random network)
  std::vector<std::vector<int> > indexLinkedDUs(m_numberDomesticUnity);
  for(int i = 0; i < m_numberDomesticUnity; ++i){
    for(int j = i+1; j < m_numberDomesticUnity; ++j)
      if(uniFLOAT(rand64) < m_probabilyConnection){
        indexLinkedDUs[i].push_back(j);
        indexLinkedDUs[j].push_back(i);
      }
  }

  // Set indexOwenedsPatches
  std::vector<std::vector<int> > indexOwenedsPatches(m_numberDomesticUnity);
  int indexDU, sizeDU = m_latticeSize/sqrt(m_numberDomesticUnity);
  for(int lin = 0; lin < m_latticeSize; ++lin){
    for(int col = 0; col < m_latticeSize; ++col){
      indexDU = (lin/sizeDU)*sizeDU + col/sizeDU;
      indexOwenedsPatches[indexDU].push_back(lin*m_latticeSize + col);
    }
  }

  // Pass the parameters to actualy initialize each domesticUnity
  for(int i = 0; i < m_numberDomesticUnity; ++i){
    domesticUnity[i].initializeDU(domesticUnity, ambient->grid, indexLinkedDUs[i], indexOwenedsPatches[i], m_numberVariety, m_outsideTradeLimit, m_insideTradeLimit);
  }
}

// Run standard version of the model. Gives as output a vector with the number of variety at each timeInterval
std::vector<int> Model::runStandard(void){
  clock_t tStart = clock();
  std::vector<int> numberVariety(m_maxTime/m_timeInterval);
  numberVariety[0] = ambient->countSpecie();
  for(int t = 0; t < m_maxTime; ++t){
    iterate();
    if (t % m_timeInterval == 0)
      numberVariety[t/m_timeInterval] = ambient->countSpecie();
  }
  cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  return numberVariety;
}

// Run the model plotting each time image of the simulation. Gives as output a vector with the number of variety at each timeInterval
std::vector<int> Model::runPlot(void){
  clock_t tStart = clock();
  std::vector<int> numberVariety(m_maxTime/m_timeInterval);
  numberVariety[0] = ambient->countSpecie();
  metrics::printState(0, ambient->grid, m_latticeSize);

  for(int t = 0; t < m_maxTime; ++t){
    iterate();
    if (t % m_timeInterval == 0){
      numberVariety[t/m_timeInterval] = ambient->countSpecie();
      metrics::printState(t, ambient->grid, m_latticeSize);
    }
  }
  cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  return numberVariety;
}

// Run one interation of the model, computing the fitness of ambient, computing DU punctuations and evaluating it's production
void Model::iterate(void){
  ambient->computeAllFitness();

  for(int i = 0; i < m_numberDomesticUnity; ++i)
    domesticUnity[i].computeDUpunctuations();

  // Shuffle order randomly
  std::vector<int> DU_list(m_numberDomesticUnity);
  for(int i = 0; i < m_numberDomesticUnity;++i)
    DU_list[i] = i;
  std::random_shuffle(DU_list.begin(),DU_list.end());
  for(auto i : DU_list)
    domesticUnity[i].evaluateProduction();
}


#endif
