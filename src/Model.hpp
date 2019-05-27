#ifndef MODEL_H
#define MODEL_H

class Model{
private:
  const Parameter m_parameter;
  Ambient* ambient;
  DomesticUnity* domesticUnity;
  void setAmbient(void);
  void setDomesticUnity(void);
  void iterate(void);
public:
  Model(Parameter t_parameter);
  ~Model();
  Result runStandard(void);
  Result runPlot(void);
  Result runFixedPoint(void);
};

// Model constructor, receive model parameters, initialize then, and call for each class initialization.
Model::Model(Parameter t_parameter)
  : m_parameter(t_parameter)
{
  setAmbient();
  setDomesticUnity();
}

// Model destructor, free memory of the pointers and set pointers to nullptr
Model::~Model(){
  delete ambient;
  delete[] domesticUnity;
  ambient = nullptr;
  domesticUnity = nullptr;
}

// Create ambient, passing the parameters using inside it
void Model::setAmbient(void){
  ambient = new Ambient(m_parameter.latticeSize, m_parameter.numberHabitat, m_parameter.numberResources, m_parameter.numberInitialVariety);
}

// Create DomesticUnity array, set indexLinkedDUs, indexOwenedsPatches and pass it to initialize each DomesticUnity
void Model::setDomesticUnity(void){
  domesticUnity =  new DomesticUnity [m_parameter.numberDomesticUnity];

  Network network(m_parameter.networkType, m_parameter.mSF, m_parameter.kWT, m_parameter.betaWT, m_parameter.probabilyConnectionER, m_parameter.numberDomesticUnity);

  // Set indexOwenedsPatches
  std::vector<std::vector<int> > indexOwenedsPatches(m_parameter.numberDomesticUnity);
  int indexDU, sizeDU = m_parameter.latticeSize/sqrt(m_parameter.numberDomesticUnity);
  for(int lin = 0; lin < m_parameter.latticeSize; ++lin){
    for(int col = 0; col < m_parameter.latticeSize; ++col){
      indexDU = (lin/sizeDU)*sizeDU + col/sizeDU;
      indexOwenedsPatches[indexDU].push_back(lin*m_parameter.latticeSize + col);
    }
  }

  // Pass the parameters to actualy initialize each domesticUnity
  for(int i = 0; i < m_parameter.numberDomesticUnity; ++i){
    domesticUnity[i].initializeDU(domesticUnity, ambient->grid, network.indexLinkedDUs[i], indexOwenedsPatches[i], m_parameter.outsideTradeLimit, m_parameter.insideTradeLimit, m_parameter.alpha, m_parameter.probabilityNewVar);
  }
}

// Run standard version of the model. Gives as output a vector with the number of variety at each timeInterval
Result Model::runStandard(void){
  Result result(0, 0, 0);
  result.numberVariety.push_back(ambient->countSpecie());

  for(int t = 0; t < m_parameter.maxTime; ++t){
    iterate();
    if (t % m_parameter.timeInterval == 0){
      result.numberVariety.push_back(ambient->countSpecie());
      result.meanVarietyDU.push_back(metrics::computeVarietyMeanProfile(domesticUnity, m_parameter.numberDomesticUnity, m_parameter.latticeSize));
      std::vector<float> tempPunctuation = metrics::computePunctuationAverage(domesticUnity, m_parameter.numberDomesticUnity);
      result.totalPunctuation.push_back(tempPunctuation[0]);
      result.fitnessPunctuation.push_back(tempPunctuation[1]);
    }
  }
  ambient->computeAllFitness();
  result.fitnessFrequency = metrics::computeFitnessProfile(ambient->grid, m_parameter.latticeSize);
  result.appearenceFrequency = metrics::computeAppearenceProfile(ambient->grid, m_parameter.latticeSize);
  result.duDistribution = metrics::computeDUprofile(domesticUnity, m_parameter.numberDomesticUnity, m_parameter.latticeSize);
  result.varietyDistribution = metrics::computeVarietyProfile(domesticUnity, m_parameter.numberDomesticUnity);
  return result;
}

// Run the model giving as output the final number of varieties and both histograms
Result Model::runFixedPoint(void){
  Result result(0, 0, 0);
  for(int t = 0; t < m_parameter.maxTime; ++t)
    iterate();

  ambient->computeAllFitness();
  result.numberVariety.push_back(ambient->countSpecie());
  result.meanVarietyDU.push_back(metrics::computeVarietyMeanProfile(domesticUnity, m_parameter.numberDomesticUnity, m_parameter.latticeSize));
  result.fitnessFrequency = metrics::computeFitnessProfile(ambient->grid, m_parameter.latticeSize);
  result.appearenceFrequency = metrics::computeAppearenceProfile(ambient->grid, m_parameter.latticeSize);
  result.duDistribution = metrics::computeDUprofile(domesticUnity, m_parameter.numberDomesticUnity, m_parameter.latticeSize);
  result.varietyDistribution = metrics::computeVarietyProfile(domesticUnity, m_parameter.numberDomesticUnity);
  return result;
}

// Run the model plotting each time image of the simulation. Gives as output a vector with the number of variety at each timeInterval
Result Model::runPlot(void){
  Result result(0, 0, 0);
  result.numberVariety.push_back(ambient->countSpecie());
  metrics::printState(0, ambient->grid, m_parameter.latticeSize);

  for(int t = 0; t < m_parameter.maxTime; ++t){
    iterate();
    if (t % m_parameter.timeInterval == 0){
      result.numberVariety.push_back(ambient->countSpecie());
      metrics::printState(t, ambient->grid, m_parameter.latticeSize);
      result.meanVarietyDU.push_back(metrics::computeVarietyMeanProfile(domesticUnity, m_parameter.numberDomesticUnity, m_parameter.latticeSize));
      std::vector<float> tempPunctuation = metrics::computePunctuationAverage(domesticUnity, m_parameter.numberDomesticUnity);
      result.totalPunctuation.push_back(tempPunctuation[0]);
      result.fitnessPunctuation.push_back(tempPunctuation[1]);
    }
  }

  ambient->computeAllFitness();
  result.fitnessFrequency = metrics::computeFitnessProfile(ambient->grid, m_parameter.latticeSize);
  result.appearenceFrequency = metrics::computeAppearenceProfile(ambient->grid, m_parameter.latticeSize);
  result.duDistribution = metrics::computeDUprofile(domesticUnity, m_parameter.numberDomesticUnity, m_parameter.latticeSize);
  result.varietyDistribution = metrics::computeVarietyProfile(domesticUnity, m_parameter.numberDomesticUnity);
  return result;
}

// Run one interation of the model, computing the fitness of ambient, computing DU punctuations and evaluating it's production
void Model::iterate(void){
  ambient->computeAllFitness();
  for(int i = 0; i < m_parameter.numberDomesticUnity; ++i)
    domesticUnity[i].computeDUpunctuations();

  // Shuffle order randomly
  std::vector<int> DU_list(m_parameter.numberDomesticUnity);
  for(int i = 0; i < m_parameter.numberDomesticUnity; ++i)
    DU_list[i] = i;
  std::random_shuffle(DU_list.begin(),DU_list.end());
  for(auto i : DU_list)
    domesticUnity[i].evaluateProduction();
}

#endif
