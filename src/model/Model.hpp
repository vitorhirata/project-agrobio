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
  Result result(m_parameter, household, ambient->grid);
  result.save_timeline();

  for(int t = 0; t < m_parameter.maxTime; ++t){
    iterate();
    if (t % m_parameter.timeInterval == 0)
      result.save_timeline();
  }
  result.save_final_state();
  return result;
}

// Run the model giving as output the final number of varieties and both
// histograms
Result Model::runFixedPoint(void){
  Result result(m_parameter, household, ambient->grid);

  for(int t = 0; t < m_parameter.maxTime; ++t)
    iterate();

  result.save_timeline();
  result.save_final_state();

  return result;
}

// Run the model plotting each time image of the simulation. Gives as output
// a vector with the number of variety at each timeInterval
Result Model::runPlot(void){
  Result result(m_parameter, household, ambient->grid);
  WriteState writeState(ambient->grid, m_parameter.latticeSize);

  result.save_timeline();
  writeState.printState(0);

  for(int t = 0; t < m_parameter.maxTime; ++t){
    iterate();
    if (t % m_parameter.timeInterval == 0){
      result.save_timeline();
      writeState.printState(t+1);
    }
  }
  result.save_final_state();
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

#endif
