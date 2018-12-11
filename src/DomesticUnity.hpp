#ifndef DOMESTICUNITY_H
#define DOMESTICUNITY_H

// DomesticUnity class. It is where the decision making takes place.
class DomesticUnity{
private:
  struct DUvariety{
    int number;
    int quantity;
    float punctuation;
  };
  DomesticUnity* m_domesticUnity;
  Patch* m_grid;
  float m_alpha;
  float m_probabilityNewVar;
  float m_outsideTradeLimit;
  float m_insideTradeLimit;
  std::vector<int> m_indexLinkedDU;
  std::vector<int> m_indexOwenedPatches;
  int m_worstVarietyIdx;
  int m_bestVarietyIdx;
  float m_DUpreference;
  int m_bestVarietySeedQuantity;
  void changeProduction(int newVar, int duIdx);
  int findVariety(int var);
  float computePunctuation(float varFitness, float varAppererence);
  void updateBestVar(int bestVar);
  void fillvarietyOwened(std::vector<int> varietyList,
      std::vector<int> varietyQuantity,
      std::vector<float> varietyFitness,
      std::vector<float> varietyAppearence);
public:
  std::vector<DUvariety> varietyOwened;
  float punctuation;
  int bestVarietyNumber;
  void initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid,
      std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches,
      float t_outsideTradeLimit, float t_insideTradeLimit, float t_alpha,
      float t_probabilityNewVar);
  void computeDUpunctuations(void);
  void evaluateProduction(void);
  void consumeVariety(void);
};

// Initialize DomesticUnity parameters (have to test member initialize list)
void DomesticUnity::initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid,
    std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches,
    float t_outsideTradeLimit, float t_insideTradeLimit, float t_alpha,
    float t_probabilityNewVar){
  m_domesticUnity = t_domesticUnity;
  m_grid = t_grid;
  m_indexLinkedDU = t_indexLinkedDU;
  m_indexOwenedPatches = t_indexOwenedPatches;
  m_outsideTradeLimit = t_outsideTradeLimit;
  m_insideTradeLimit = t_insideTradeLimit;
  m_alpha = t_alpha;
  m_probabilityNewVar = t_probabilityNewVar;
  m_DUpreference = gauss(rand64);
  while(!(m_DUpreference > 0 && m_DUpreference < 1))
    m_DUpreference = gauss(rand64);

  uniIntPlace.param(std::uniform_int_distribution<long>::param_type(0, m_indexOwenedPatches.size()-1));
}

// Iterate over the Oweneds Patches, colect varieties and set varietyOwened, m_worstVarietyIdx,
// bestVarietyNumber, m_bestVarietySeedQuantity and m_bestVarietyIdx
void DomesticUnity::computeDUpunctuations(void){
  std::vector<int> varietyList;
  std::vector<int> varietyQuantity;
  std::vector<float> varietyFitness;
  std::vector<float> varietyAppearence;

  // Add first element
  varietyList.push_back(m_grid[m_indexOwenedPatches[0]].variety.varietyNumber);
  varietyQuantity.push_back(1);
  varietyFitness.push_back(m_grid[m_indexOwenedPatches[0]].fitness);
  varietyAppearence.push_back(m_grid[m_indexOwenedPatches[0]].variety.appearence);

  // Complete to fill the vectors
  for(uint i = 1; i < m_indexOwenedPatches.size(); ++i){
    int patchNumber = m_indexOwenedPatches[i];
    int varNumber = m_grid[patchNumber].variety.varietyNumber;
    int idx = std::lower_bound(varietyList.begin(), varietyList.end(), varNumber) - varietyList.begin();
    if(varietyList[idx] != varNumber){
      varietyList.insert(varietyList.begin()+idx, varNumber);
      varietyQuantity.insert(varietyQuantity.begin()+idx, 0);
      varietyFitness.insert(varietyFitness.begin()+idx, 0);
      varietyAppearence.insert(varietyAppearence.begin()+idx, 0);
    }
    ++varietyQuantity[idx];
    varietyFitness[idx] += m_grid[patchNumber].fitness;
    varietyAppearence[idx] += m_grid[patchNumber].variety.appearence;
  }

  fillvarietyOwened(varietyList, varietyQuantity, varietyFitness, varietyAppearence);

  //Set DU punctuation
  float puncTemp = 0;
  for(auto i : varietyOwened){
    puncTemp += i.punctuation;
  }
  punctuation = puncTemp / varietyOwened.size();
}


void DomesticUnity::fillvarietyOwened(std::vector<int> varietyList,
      std::vector<int> varietyQuantity,
      std::vector<float> varietyFitness,
      std::vector<float> varietyAppearence){
  varietyOwened.clear();
  float bestVarPunctuation = -100.0;
  float worstVarPunctuation = 100.0;
  for(uint i = 0; i < varietyList.size(); ++i){
    DUvariety newVar;
    newVar.number = varietyList[i];
    newVar.quantity = varietyQuantity[i];
    newVar.punctuation = computePunctuation(varietyFitness[i] / varietyQuantity[i], varietyAppearence[i] / varietyQuantity[i]);
    varietyOwened.push_back(newVar);
    if(newVar.punctuation > bestVarPunctuation){
      bestVarPunctuation = newVar.punctuation;
      bestVarietyNumber = newVar.number;
      m_bestVarietyIdx = i;
    }
    if(newVar.punctuation < worstVarPunctuation){
      worstVarPunctuation = newVar.punctuation;
      m_worstVarietyIdx = i;
    }
  }
  m_bestVarietySeedQuantity = 3 * varietyOwened[m_bestVarietyIdx].quantity;
}

float DomesticUnity::computePunctuation(float varFitness, float varAppererence){
  float appearencePunc = 1 - std::min(abs(varAppererence - m_DUpreference), 1 - abs(varAppererence - m_DUpreference));
  return m_alpha * varFitness + (1 - m_alpha) * appearencePunc;
}

// Find the DU with best punctuation, if the difference is larger than m_outsideTradeLimit it takes this DU best
// variety and replace for the DU worst variety. If the difference is larger than m_insideTradeLimit it makes
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
  if(extpunctuationDifference < m_outsideTradeLimit && extpunctuationDifference > 0
      && m_domesticUnity[bestDUindex].bestVarietyNumber != varietyOwened[m_worstVarietyIdx].number){
    int varNumber = m_domesticUnity[bestDUindex].bestVarietyNumber;
    changeProduction(varNumber, bestDUindex);
    m_domesticUnity[bestDUindex].consumeVariety();
  }
  else if(intpunctuationDifference < m_insideTradeLimit &&
      bestVarietyNumber != varietyOwened[m_worstVarietyIdx].number){
    changeProduction(bestVarietyNumber, -1);
    consumeVariety();
  }
  if(uniFLOAT(rand64) < m_probabilityNewVar){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    m_grid[newPlace].setRandomVariety();
    if(findVariety(bestVarietyNumber) == -1) // If bestVariety no longer exists update
      updateBestVar(bestVarietyNumber);
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
    cout << "ERROR in DomesticUnity::findVariety. Variety not found, best=" << bestVarietyPlace << ", worst=" << worstVarietyPlace << ". DUidx=" << duIdx << endl;
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

// Consume one seed of the best variety. If the number of seeds of the best variety reaches 0 the new best variety
// is calculated, setting bestVarietyNumber, m_bestVarietySeedQuantity and m_bestVarietyIdx
void DomesticUnity::consumeVariety(void){
  --m_bestVarietySeedQuantity;
  if(m_bestVarietySeedQuantity == 0) // If seed is over compute next best variety
    updateBestVar(bestVarietyNumber);
}

// Receive the actual bestVariety and update the best variety, so that it is not the received value
void DomesticUnity::updateBestVar(int bestVar){
  int oldBestVar = bestVar;
  float bestVarPunctuation = -1;
  for(uint i = 0; i < varietyOwened.size(); ++i){
    if(varietyOwened[i].punctuation > bestVarPunctuation && varietyOwened[i].number != oldBestVar && findVariety(varietyOwened[i].number) != -1){
      bestVarPunctuation = varietyOwened[i].punctuation;
      bestVarietyNumber = varietyOwened[i].number;
      m_bestVarietyIdx = i;
    }
  }
  m_bestVarietySeedQuantity = 3 * varietyOwened[m_bestVarietyIdx].quantity;
}

#endif
