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
  int m_numberVariety;
  float m_alpha;
  float m_outsideTradeLimit;
  float m_insideTradeLimit;
  std::vector<int> m_indexLinkedDU;
  std::vector<int> m_indexOwenedPatches;
  std::vector<DUvariety> varietyOwened;
  int m_worstVarietyIdx;
  int m_bestVarietyIdx;
  Variety* m_variety;
  float m_DUpreference;
  int m_bestVarietySeedQuantity;
  void changeProduction(int var);
  int findWorstVarietyPlace(void);
  float computePunctuation(float varFitness, float varAppererence);
public:
  float punctuation;
  int bestVarietyNumber;
  void initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid, std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches, int t_nVariety, float t_outsideTradeLimit, float t_insideTradeLimit, float t_alpha, Variety* t_variety);
  void computeDUpunctuations(void);
  void evaluateProduction(void);
  void consumeVariety(void);
};

// Initialize DomesticUnity parameters (have to test member initialize list)
void DomesticUnity::initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid, std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches, int t_nVariety, float t_outsideTradeLimit, float t_insideTradeLimit, float t_alpha, Variety* t_variety){
  m_domesticUnity = t_domesticUnity;
  m_grid = t_grid;
  m_indexLinkedDU = t_indexLinkedDU;
  m_indexOwenedPatches = t_indexOwenedPatches;
  m_numberVariety = t_nVariety;
  m_outsideTradeLimit = t_outsideTradeLimit;
  m_insideTradeLimit = t_insideTradeLimit;
  m_alpha = t_alpha;
  m_variety = t_variety;
  m_DUpreference = uniFLOAT(rand64);
}

// Iterate over the Oweneds Patches, colect varieties and set varietyOwened, m_worstVarietyIdx,
// bestVarietyNumber, m_bestVarietySeedQuantity and m_bestVarietyIdx
void DomesticUnity::computeDUpunctuations(void){
  std::vector<float> varietyQuantity(m_numberVariety, 0);
  std::vector<float> varietyFitness(m_numberVariety, 0);

  for(auto i : m_indexOwenedPatches){
    int varNumber = m_grid[i].plantedVariety;
    ++varietyQuantity[varNumber];
    varietyFitness[varNumber] += m_grid[i].fitness;
  }

  varietyOwened.clear();
  float bestVarPunctuation = -100.0;
  float worstVarPunctuation = 100.0;
  for(int i = 0; i < m_numberVariety; ++i){
    if(varietyQuantity[i] > 0){
      DUvariety newVar;
      newVar.number = i;
      newVar.quantity = varietyQuantity[i];
      newVar.punctuation = computePunctuation(varietyFitness[i] / varietyQuantity[i], m_variety[i].appearence);
      varietyOwened.push_back(newVar);
      if(newVar.punctuation > bestVarPunctuation){
        bestVarPunctuation = newVar.punctuation;
        bestVarietyNumber = i;
        m_bestVarietyIdx = varietyOwened.size()-1;
      }
      if(newVar.punctuation < worstVarPunctuation){
        worstVarPunctuation = newVar.punctuation;
        m_worstVarietyIdx = varietyOwened.size()-1;
      }
    }
  }
  m_bestVarietySeedQuantity = 3 * varietyOwened[m_bestVarietyIdx].quantity;

  //Set DU punctuation
  float puncTemp = 0;
  for(auto i : varietyOwened){
    puncTemp += i.punctuation;
  }
  punctuation = puncTemp / varietyOwened.size();
}

float DomesticUnity::computePunctuation(float varFitness, float varAppererence){
  return m_alpha * varFitness + (1 - m_alpha) * abs(varAppererence - m_DUpreference);
}

// Find the DU with best punctuation, if the difference is larger than m_outsideTradeLimit it takes this DU best
// variety and replace for the DU worst variety. If the difference is larger than m_insideTradeLimit it makes
// the same thing but with best variety of the own DU.
void DomesticUnity::evaluateProduction(void){
  float bestDUpunctuation = -1;
  int bestDUindex;


  for(int i = 0; i < m_indexLinkedDU.size(); ++i){
    if(m_domesticUnity[m_indexLinkedDU[i]].punctuation > bestDUpunctuation){
      bestDUindex = m_indexLinkedDU[i];
      bestDUpunctuation = m_domesticUnity[bestDUindex].punctuation;
    }
  }
  if(bestDUpunctuation - punctuation > m_outsideTradeLimit && m_domesticUnity[bestDUindex].bestVarietyNumber != varietyOwened[m_worstVarietyIdx].number){
    int varNumber = m_domesticUnity[bestDUindex].bestVarietyNumber;
    m_domesticUnity[bestDUindex].consumeVariety();
    changeProduction(varNumber);
  }
  else if(bestDUpunctuation - punctuation > m_insideTradeLimit){
    int varNumber = bestVarietyNumber;
    consumeVariety();
    changeProduction(varNumber);
  }
}

// Takes one place (between the owened patches) where the worst variety exists and replace it with var
void DomesticUnity::changeProduction(int var){
  int worstVarietyPlace = findWorstVarietyPlace();
  m_grid[worstVarietyPlace].setVariety(var);
}

// Find one place (between the owened patches) where the worst variety exists. If it does not find raise error
int DomesticUnity::findWorstVarietyPlace(void){
  int varietyNumber = varietyOwened[m_worstVarietyIdx].number;
  for(auto i : m_indexOwenedPatches)
    if(m_grid[i].plantedVariety == varietyNumber)
      return i;

  cout << "ERROR in DomesticUnity::findWorstVarietyPlace. Variety not found" << endl;
  exit(-1);
}

// Consume one seed of the best variety. If the number of seeds of the best variety reaches 0 the new best variety
// is calculated, setting bestVarietyNumber, m_bestVarietySeedQuantity and m_bestVarietyIdx
void DomesticUnity::consumeVariety(void){
  --m_bestVarietySeedQuantity;
  if(m_bestVarietySeedQuantity == 0){ // If seed is over compute next best variety
    float bestVarPunctuation = -1;
    for(uint i = 0; i < varietyOwened.size(); ++i){
      if(varietyOwened[i].punctuation > bestVarPunctuation){
        bestVarPunctuation = varietyOwened[i].punctuation;
        bestVarietyNumber = varietyOwened[i].number;
        m_bestVarietyIdx = i;
      }
    }
    m_bestVarietySeedQuantity = 3 * varietyOwened[m_bestVarietyIdx].quantity;
  }
}
#endif
