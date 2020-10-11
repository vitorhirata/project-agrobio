#ifndef MODEL_H
#define MODEL_H

class Model{
private:
  const Parameter m_parameter;
  Ambient* ambient;
  Household* household;
  void setAmbient(void);
  void setHousehold(void);
  void iterate(void);
  float computeAverageSimpson(void);
  float computeAverageShannon(void);
public:
  Model(Parameter t_parameter);
  ~Model();
  Result runStandard(void);
  Result runPlot(void);
  Result runFixedPoint(void);
};

// Model constructor, receive model parameters, initialize then, and call for
// each class initialization.
Model::Model(Parameter t_parameter)
  : m_parameter(t_parameter)
{
  setAmbient();
  setHousehold();
}

// Model destructor, free memory of the pointers and set pointers to nullptr
Model::~Model(){
  delete ambient;
  delete[] household;
  ambient = nullptr;
  household = nullptr;
}

// Create ambient, passing the parameters using inside it
void Model::setAmbient(void){
  ambient = new Ambient(m_parameter.latticeSize, m_parameter.numberHabitat,
      m_parameter.numberResources, m_parameter.numberInitialVariety,
      m_parameter.numberInitialVarietyHD, m_parameter.deathProbability,
      m_parameter.crossingDeviation);
}

// Create Household array, set indexLinkedHDs, indexOwenedsPatches and
// pass it to initialize each Household
void Model::setHousehold(void){
  HDParameter hdParameter(m_parameter);
  household =  new Household [m_parameter.numberHousehold];

  Network network(m_parameter.networkType, m_parameter.mSF, m_parameter.kWT,
      m_parameter.betaWT, m_parameter.probabilyConnectionER,
      m_parameter.numberHousehold);

  // Set indexOwenedsPatches
  std::vector<std::vector<int> > indexOwenedsPatches(
      m_parameter.numberHousehold);
  int indexHD, sizeHD = m_parameter.latticeSize /
    sqrt(m_parameter.numberHousehold);
  for(int lin = 0; lin < m_parameter.latticeSize; ++lin){
    for(int col = 0; col < m_parameter.latticeSize; ++col){
      indexHD = (lin/sizeHD)*sizeHD + col/sizeHD;
      indexOwenedsPatches[indexHD].push_back(lin*m_parameter.latticeSize+col);
    }
  }

  // Pass the parameters to actualy initialize each household
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    household[i].initializeHD(household, ambient->grid,
        network.indexLinkedHDs[i], indexOwenedsPatches[i], hdParameter);
  }
}

// Run standard version of the model. Gives as output a vector with the
// number of variety at each timeInterval
Result Model::runStandard(void){
  Result result(0, 0, 0, 0);
  result.numberVariety.push_back(ambient->countSpecie());

  for(int t = 0; t < m_parameter.maxTime; ++t){
    iterate();
    if (t % m_parameter.timeInterval == 0){
      result.numberVariety.push_back(ambient->countSpecie());
      result.meanVarietyHD.push_back(metrics::computeVarietyMeanProfile(
            household, m_parameter.numberHousehold,
            m_parameter.latticeSize));
      std::vector<float> tempPunctuation = metrics::computePunctuationAverage(
          household, m_parameter.numberHousehold);
      result.totalPunctuation.push_back(tempPunctuation[0]);
      result.productivityPunctuation.push_back(tempPunctuation[1]);
      result.bergerParkerCommunity.push_back(metrics::computeBergerParker(
            household, m_parameter.numberHousehold,
            m_parameter.latticeSize));
      result.bergerParkerHD.push_back(metrics::computeBergerParkerHD(
            household, m_parameter.numberHousehold));
      result.simpsonCommunity.push_back(metrics::computeSimpson(household,
            m_parameter.numberHousehold, m_parameter.latticeSize));
      result.shannonCommunity.push_back(metrics::computeShannon(household,
            m_parameter.numberHousehold, m_parameter.latticeSize));
      result.simpsonHD.push_back(computeAverageSimpson());
      result.shannonHD.push_back(computeAverageShannon());
    }
  }
  std::vector<float> tempPunctuation = metrics::computePunctuationAverage(
      household, m_parameter.numberHousehold);
  result.totalPunctuation.push_back(tempPunctuation[0]);
  result.productivityPunctuation.push_back(tempPunctuation[1]);
  result.productivityFrequency = metrics::computeProductivityProfile(
      ambient->grid, m_parameter.latticeSize);
  result.qualityFrequency = metrics::computeQualityProfile(
      ambient->grid, m_parameter.latticeSize);
  result.hdDistribution = metrics::computeHDprofile(
      household, m_parameter.numberHousehold, m_parameter.latticeSize);
  result.varietyDistribution = metrics::computeVarietyProfile(
      household, m_parameter.numberHousehold);
  result.varietyQuantity = metrics::computeVarietyQuantity(
      household, m_parameter.numberHousehold, m_parameter.latticeSize);
  return result;
}

// Run the model giving as output the final number of varieties and both
// histograms
Result Model::runFixedPoint(void){
  Result result(0, 0, 0, 0);
  for(int t = 0; t < m_parameter.maxTime; ++t)
    iterate();

  result.numberVariety.push_back(ambient->countSpecie());
  result.meanVarietyHD.push_back(metrics::computeVarietyMeanProfile(
        household, m_parameter.numberHousehold,
        m_parameter.latticeSize));
  std::vector<float> tempPunctuation = metrics::computePunctuationAverage(
      household, m_parameter.numberHousehold);
  result.totalPunctuation.push_back(tempPunctuation[0]);
  result.productivityPunctuation.push_back(tempPunctuation[1]);
  result.simpsonCommunity.push_back(metrics::computeSimpson(household,
        m_parameter.numberHousehold, m_parameter.latticeSize));
  result.bergerParkerCommunity.push_back(metrics::computeBergerParker(
            household, m_parameter.numberHousehold,
            m_parameter.latticeSize));
  result.shannonCommunity.push_back(metrics::computeShannon(household,
        m_parameter.numberHousehold, m_parameter.latticeSize));
  result.simpsonHD.push_back(computeAverageSimpson());
  result.shannonHD.push_back(computeAverageShannon());
  result.bergerParkerHD.push_back(metrics::computeBergerParkerHD(
        household, m_parameter.numberHousehold));
  result.productivityFrequency = metrics::computeProductivityProfile(
      ambient->grid, m_parameter.latticeSize);
  result.qualityFrequency = metrics::computeQualityProfile(
      ambient->grid, m_parameter.latticeSize);
  result.hdDistribution = metrics::computeHDprofile(
      household, m_parameter.numberHousehold, m_parameter.latticeSize);
  result.varietyDistribution = metrics::computeVarietyProfile(
      household, m_parameter.numberHousehold);
  result.varietyQuantity = metrics::computeVarietyQuantity(
      household, m_parameter.numberHousehold, m_parameter.latticeSize);
  return result;
}

// Run the model plotting each time image of the simulation. Gives as output
// a vector with the number of variety at each timeInterval
Result Model::runPlot(void){
  Result result(0, 0, 0, 0);
  result.numberVariety.push_back(ambient->countSpecie());
  metrics::printState(0, ambient->grid, m_parameter.latticeSize);

  for(int t = 0; t < m_parameter.maxTime; ++t){
    iterate();
    if (t % m_parameter.timeInterval == 0){
      result.numberVariety.push_back(ambient->countSpecie());
      metrics::printState(t+1, ambient->grid, m_parameter.latticeSize);
      result.meanVarietyHD.push_back(metrics::computeVarietyMeanProfile(
            household, m_parameter.numberHousehold,
            m_parameter.latticeSize));
      std::vector<float> tempPunctuation = metrics::computePunctuationAverage(
          household, m_parameter.numberHousehold);
      result.totalPunctuation.push_back(tempPunctuation[0]);
      result.productivityPunctuation.push_back(tempPunctuation[1]);
      result.bergerParkerCommunity.push_back(metrics::computeBergerParker(
            household, m_parameter.numberHousehold,
            m_parameter.latticeSize));
      result.bergerParkerHD.push_back(metrics::computeBergerParkerHD(
            household, m_parameter.numberHousehold));
      result.simpsonCommunity.push_back(metrics::computeSimpson(household,
            m_parameter.numberHousehold, m_parameter.latticeSize));
      result.shannonCommunity.push_back(metrics::computeShannon(household,
            m_parameter.numberHousehold, m_parameter.latticeSize));
      result.simpsonHD.push_back(computeAverageSimpson());
      result.shannonHD.push_back(computeAverageShannon());
    }
  }
  result.productivityFrequency = metrics::computeProductivityProfile(
      ambient->grid, m_parameter.latticeSize);
  result.qualityFrequency = metrics::computeQualityProfile(
      ambient->grid, m_parameter.latticeSize);
  result.hdDistribution = metrics::computeHDprofile(
      household, m_parameter.numberHousehold, m_parameter.latticeSize);
  result.varietyDistribution = metrics::computeVarietyProfile(
      household, m_parameter.numberHousehold);
  result.varietyQuantity = metrics::computeVarietyQuantity(
      household, m_parameter.numberHousehold, m_parameter.latticeSize);
  return result;
}

// Run one interation of the model, computing HD punctuations and evaluating
// it's production
void Model::iterate(void){
  ambient->runDeath();
  for(int i = 0; i < m_parameter.numberHousehold; ++i)
    household[i].computeHDpunctuations();

  // Shuffle order randomly
  std::vector<int> HD_list(m_parameter.numberHousehold);
  for(int i = 0; i < m_parameter.numberHousehold; ++i)
    HD_list[i] = i;
  std::random_shuffle(HD_list.begin(),HD_list.end());
  for(auto i : HD_list)
    household[i].iterateHD();
}

float Model::computeAverageSimpson(void){
  float simpson = 0;
  for(int i = 0; i < m_parameter.numberHousehold; ++i)
    simpson += household[i].computeSimpsonHD();
  simpson /= m_parameter.numberHousehold;
  return simpson;
}

float Model::computeAverageShannon(void){
  float shannon = 0;
  for(int i = 0; i < m_parameter.numberHousehold; ++i)
    shannon += household[i].computeShannonHD();
  shannon /= m_parameter.numberHousehold;
  return shannon;
}

#endif
