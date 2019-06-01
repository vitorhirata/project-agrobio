#ifndef DOMESTICUNITY_H
#define DOMESTICUNITY_H

// DomesticUnity class. It is where the decision making takes place.
class DomesticUnity{
private:
  struct DUvariety{
    int number;
    int quantity;
    float punctuation;
    float fitness_punctuation;
  };
  DomesticUnity* m_domesticUnity;
  Patch* m_grid;
  DUParameter m_duParameter;
  std::vector<int> m_indexLinkedDU;
  std::vector<int> m_indexOwenedPatches;
  int m_worstVarietyIdx;
  int m_bestVarietyIdx;
  float m_DUpreference;
  void updateVarietyOwened(int oldVar, int newVar = -1);
  void changeProduction(int newVar, int duIdx);
  int findVariety(int var);
  float computePunctuation(float varFitness, float varAppererence);
  void computeBestVar(void);
  void computeWorstVar(void);
  void fillvarietyOwened(std::map<int,std::vector<float> >* varietyData);
public:
  std::vector<DUvariety> varietyOwened;
  float punctuation;
  float fitness_punctuation;
  int bestVarietyNumber;
  int numberVarietyOwened(void);
  void initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid,
      std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches,
      DUParameter t_duParameter);
  void computeDUpunctuations(void);
  void evaluateProduction(void);
};

// Initialize DomesticUnity parameters (have to test member initialize list)
void DomesticUnity::initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid,
    std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches,
    DUParameter t_duParameter){
  m_domesticUnity = t_domesticUnity;
  m_grid = t_grid;
  m_indexLinkedDU = t_indexLinkedDU;
  m_indexOwenedPatches = t_indexOwenedPatches;
  m_duParameter.outsideTradeLimit = t_duParameter.outsideTradeLimit;
  m_duParameter.insideTradeLimit = t_duParameter.insideTradeLimit;
  m_duParameter.alpha = t_duParameter.alpha;
  m_duParameter.probabilityNewVar = t_duParameter.probabilityNewVar;
  m_duParameter.probabilityDeath = t_duParameter.probabilityDeath;
  m_DUpreference = gauss(rand64);
  while(!(m_DUpreference > 0 && m_DUpreference < 1))
    m_DUpreference = gauss(rand64);
  varietyOwened = std::vector<DUvariety>(m_indexOwenedPatches.size());
  uniIntPlace.param(std::uniform_int_distribution<long>::param_type(0, m_indexOwenedPatches.size()-1));
}

// Iterate over the Oweneds Patches, colect varieties and set varietyOwened, m_worstVarietyIdx,
// bestVarietyNumber and m_bestVarietyIdx
void DomesticUnity::computeDUpunctuations(void){
  // Map the variety in a vector, where the 0th element is quantity, the 1st fitness and the 2nd appearence
  std::map<int,std::vector<float> > varietyData;

  // Fill the map
  for(uint i = 0; i < m_indexOwenedPatches.size(); ++i){
    int patchNumber = m_indexOwenedPatches[i];
    int varNumber = m_grid[patchNumber].variety.varietyNumber;
    if(varietyData.count(varNumber) == 0)
      varietyData[varNumber] = std::vector<float>(3,0);
    ++varietyData[varNumber][0];
    varietyData[varNumber][1] += m_grid[patchNumber].fitness;
    varietyData[varNumber][2] += m_grid[patchNumber].variety.appearence;
  }

  fillvarietyOwened(&varietyData);

  //Set DU punctuation
  float puncTemp = 0;
  float fitnessPuncTemp = 0;
  for(auto i : varietyOwened){
    puncTemp += (i.punctuation * i.quantity);
    fitnessPuncTemp += (i.fitness_punctuation * i.quantity);
  }
  punctuation = puncTemp / m_indexOwenedPatches.size();
  fitness_punctuation = fitnessPuncTemp / m_indexOwenedPatches.size();
}

// Fill the varietyOwened vector with the data provided by the input map
void DomesticUnity::fillvarietyOwened(std::map<int,std::vector<float> >* varietyData){
  varietyOwened.clear();
  float bestVarPunctuation = -100.0;
  float worstVarPunctuation = 100.0;
  map<int, std::vector<float> >::iterator itr;
  for(itr = varietyData->begin(); itr != varietyData->end(); ++itr){
    DUvariety newVar;
    newVar.number = itr->first;
    newVar.quantity = itr->second[0];
    if(newVar.number == -1){ // if there is no variety punctuation is zero
      newVar.punctuation = 0;
      newVar.fitness_punctuation = 0;
    }
    else{
      newVar.punctuation = computePunctuation(itr->second[1] / itr->second[0], itr->second[2] / itr->second[0]);
      newVar.fitness_punctuation = itr->second[1] / itr->second[0];
    }
    varietyOwened.push_back(newVar);
    if(newVar.punctuation > bestVarPunctuation){
      bestVarPunctuation = newVar.punctuation;
      bestVarietyNumber = newVar.number;
      m_bestVarietyIdx = varietyOwened.size() - 1;
    }
    if(newVar.punctuation < worstVarPunctuation){
      worstVarPunctuation = newVar.punctuation;
      m_worstVarietyIdx = varietyOwened.size() - 1;
    }
  }
}

// Compute the punctuation based on the fitness appearence and alpha
float DomesticUnity::computePunctuation(float varFitness, float varAppererence){
  float appearencePunc = 1 - std::min(abs(varAppererence - m_DUpreference), 1 - abs(varAppererence - m_DUpreference));
  return m_duParameter.alpha * varFitness + (1 - m_duParameter.alpha) * appearencePunc;
}

// Find the DU with best punctuation, if the difference is larger than m_duParameter.outsideTradeLimit it takes this DU best
// variety and replace for the DU worst variety. If the difference is larger than m_duParameter.insideTradeLimit it makes
// the same thing but with best variety of the own DU.
void DomesticUnity::evaluateProduction(void){
  float bestDUpunctuation = -1;
  int bestDUindex;

  for(uint i = 0; i < m_indexLinkedDU.size(); ++i){
    if(m_domesticUnity[m_indexLinkedDU[i]].punctuation > bestDUpunctuation){
      bestDUindex = m_indexLinkedDU[i];
      bestDUpunctuation = m_domesticUnity[bestDUindex].punctuation;
    }
  }

  float extpunctuationDifference = bestDUpunctuation - punctuation;
  float intpunctuationDifference = varietyOwened[m_bestVarietyIdx].punctuation - varietyOwened[m_worstVarietyIdx].punctuation;
  if(extpunctuationDifference > m_duParameter.outsideTradeLimit && extpunctuationDifference > 0
      && m_domesticUnity[bestDUindex].bestVarietyNumber != varietyOwened[m_worstVarietyIdx].number){
    int varNumber = m_domesticUnity[bestDUindex].bestVarietyNumber;
    changeProduction(varNumber, bestDUindex);
    updateVarietyOwened(varietyOwened[m_worstVarietyIdx].number, varNumber);
  }
  if(intpunctuationDifference > m_duParameter.insideTradeLimit &&
      bestVarietyNumber != varietyOwened[m_worstVarietyIdx].number){
    changeProduction(bestVarietyNumber, -1);
    updateVarietyOwened(varietyOwened[m_worstVarietyIdx].number);
  }
  if(uniFLOAT(rand64) < m_duParameter.probabilityNewVar){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    int oldVar = m_grid[newPlace].variety.varietyNumber;
    m_grid[newPlace].setRandomVariety();
    updateVarietyOwened(oldVar, m_grid[newPlace].variety.varietyNumber);
  }
  if(uniFLOAT(rand64) < m_duParameter.probabilityDeath){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    int oldVar = m_grid[newPlace].variety.varietyNumber;
    m_grid[newPlace].killVariety();
    updateVarietyOwened(oldVar);
  }
}

// Takes one place (between the owened patches) where the worst variety exists and replace it with var
void DomesticUnity::changeProduction(int newVar, int duIdx){
  int worstVarietyPlace = findVariety(varietyOwened[m_worstVarietyIdx].number);
  int bestVarietyPlace;
  if(duIdx == -1)
    bestVarietyPlace = findVariety(newVar);
  else
    bestVarietyPlace = m_domesticUnity[duIdx].findVariety(newVar);
  if(worstVarietyPlace == -1 || bestVarietyPlace == -1){
    cout << "ERROR in DomesticUnity::findVariety. Variety not found, best=" << newVar << "bestPlace=" << bestVarietyPlace << ", worst=" << varietyOwened[m_worstVarietyIdx].number << "worstPlace" << worstVarietyPlace << ". DUidx=" << duIdx << endl;
    exit(-1);
  }
  VarietyData data = m_grid[bestVarietyPlace].giveVarietyData();
  m_grid[worstVarietyPlace].setVariety(data);
}

// Find one place (between the owened patches) where the given variety exists. If it does not find return -1
int DomesticUnity::findVariety(int var){
  for(auto i : m_indexOwenedPatches)
    if(m_grid[i].variety.varietyNumber == var)
      return i;
  return -1;
}

void DomesticUnity::updateVarietyOwened(int oldVar, int newVar){
  uint varIdx = 0;
  while(varietyOwened[varIdx].number != oldVar && varIdx < varietyOwened.size())
    ++varIdx;
  if(varIdx == varietyOwened.size()){
    cout << "ERROR: VARIETY TO BE REMOVED DOES NOT EXIST." << endl;
    exit(-1);
  }
  --varietyOwened[varIdx].quantity;
  if(varietyOwened[varIdx].quantity == 0){
    varietyOwened.erase(varietyOwened.begin() + varIdx);
    if(m_worstVarietyIdx == varIdx)
      computeWorstVar();
    if(m_bestVarietyIdx == varIdx)
      computeBestVar();
  }

  if(newVar != -1){
    DUvariety duVariety;
    duVariety.number = newVar;
    duVariety.quantity = 1;
    duVariety.punctuation = 0;
    duVariety.fitness_punctuation = 0;
    varietyOwened.push_back(duVariety);
  }
}

// Receive the actual bestVariety and update the best variety, so that it is not the received value
void DomesticUnity::computeBestVar(void){
  float bestVarPunctuation = -1;
  for(uint i = 0; i < varietyOwened.size(); ++i){
    if(varietyOwened[i].punctuation > bestVarPunctuation){
      bestVarPunctuation = varietyOwened[i].punctuation;
      bestVarietyNumber = varietyOwened[i].number;
      m_bestVarietyIdx = i;
    }
  }
}

// Receive the actual worstVariety and update the worst variety, so that it is not the received value
void DomesticUnity::computeWorstVar(void){
  float worstVarPunctuation = 100;
  for(uint i = 0; i < varietyOwened.size(); ++i){
    if(varietyOwened[i].punctuation < worstVarPunctuation){
      worstVarPunctuation = varietyOwened[i].punctuation;
      m_worstVarietyIdx = i;
    }
  }
}

// Return the number of variety owened by the DU, excluding empty patches.
int DomesticUnity::numberVarietyOwened(void){
  for(auto i : varietyOwened){
    if(i.number == -1)
      return varietyOwened.size() - 1;
  }
  return varietyOwened.size();
}
#endif
