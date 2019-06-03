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
    VarietyData varietyData;
  };
  DomesticUnity* m_domesticUnity;
  Patch* m_grid;
  DUParameter m_duParameter;
  std::vector<int> m_indexLinkedDU;
  std::vector<int> m_indexOwenedPatches;
  int m_worstVarietyIdx;
  float m_DUpreference;
  void changeProduction(VarietyData varietyData);
  int findVariety(int var);
  float computePunctuation(float varFitness, float varAppererence);
  void updateWorstVar(int oldWorstVar);
  void fillvarietyOwened(std::map<int,std::vector<float> >* varietyData);
public:
  std::vector<DUvariety> varietyOwened;
  float punctuation;
  float fitness_punctuation;
  int bestVarietyIdx;
  int numberVarietyOwened(void);
  void initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid,
      std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches,
      DUParameter t_duParameter);
  void computeDUpunctuations(void);
  void iterateDU(void);
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
  uniIntPlace.param(std::uniform_int_distribution<long>::param_type(0,
        m_indexOwenedPatches.size()-1));
}

// Iterate over the Oweneds Patches, colect varieties and set varietyOwened,
// m_worstVarietyIdx and bestVarietyIdx
void DomesticUnity::computeDUpunctuations(void){
  // Map the variety in a vector, where the 0th element is quantity,
  // the 1st fitness and the 2nd appearence
  std::map<int,std::vector<float> > varietyData;

  // Fill the map
  for(uint i = 0; i < m_indexOwenedPatches.size(); ++i){
    int patchNumber = m_indexOwenedPatches[i];
    int varNumber = m_grid[patchNumber].variety.varietyNumber;
    if(varietyData.count(varNumber) == 0){
      varietyData[varNumber] = std::vector<float>(4,0);
      varietyData[varNumber][3] = patchNumber;
    }
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
void DomesticUnity::fillvarietyOwened(std::map<int,
    std::vector<float> >* varietyData){
  varietyOwened.clear();
  float bestVarPunctuation = -100.0;
  float worstVarPunctuation = 100.0;
  map<int, std::vector<float> >::iterator itr;
  for(itr = varietyData->begin(); itr != varietyData->end(); ++itr){
    DUvariety newVar;
    newVar.number = itr->first;
    newVar.quantity = itr->second[0];
    newVar.varietyData = m_grid[(int) itr->second[3]].giveVarietyData();
    if(newVar.number == -1){ // if there is no variety punctuation is zero
      newVar.punctuation = 0;
      newVar.fitness_punctuation = 0;
    }
    else{
      newVar.punctuation = computePunctuation(itr->second[1] / itr->second[0],
          itr->second[2] / itr->second[0]);
      newVar.fitness_punctuation = itr->second[1] / itr->second[0];
    }
    varietyOwened.push_back(newVar);
    if(newVar.punctuation > bestVarPunctuation){
      bestVarPunctuation = newVar.punctuation;
      bestVarietyIdx = varietyOwened.size() - 1;
    }
    if(newVar.punctuation < worstVarPunctuation){
      worstVarPunctuation = newVar.punctuation;
      m_worstVarietyIdx = varietyOwened.size() - 1;
    }
  }
}

// Compute the punctuation based on the fitness appearence and alpha
float DomesticUnity::computePunctuation(float varFitness,
    float varAppererence){
  float appearencePunc = 1 - std::min(abs(varAppererence - m_DUpreference),
      1 - abs(varAppererence - m_DUpreference));
  return m_duParameter.alpha * varFitness +
    (1 - m_duParameter.alpha) * appearencePunc;
}

// Find the DU with best punctuation, if the difference is larger than
// m_duParameter.outsideTradeLimit it takes this DU best
// variety and replace for the DU worst variety. If the difference is larger
// than m_duParameter.insideTradeLimit it makes
// the same thing but with best variety of the own DU.
void DomesticUnity::iterateDU(void){
  float bestDUpunctuation = -1;
  int bestDUindex;

  for(uint i = 0; i < m_indexLinkedDU.size(); ++i){
    if(m_domesticUnity[m_indexLinkedDU[i]].punctuation > bestDUpunctuation){
      bestDUindex = m_indexLinkedDU[i];
      bestDUpunctuation = m_domesticUnity[bestDUindex].punctuation;
    }
  }

  float extpunctuationDifference = bestDUpunctuation - punctuation;
  float intpunctuationDifference = varietyOwened[bestVarietyIdx].punctuation
    - varietyOwened[m_worstVarietyIdx].punctuation;
  int extBestVarietyIdx = floor(uniFLOAT(rand64) *
    m_domesticUnity[bestDUindex].varietyOwened.size());
  DUvariety duVarietyExt =
    m_domesticUnity[bestDUindex].varietyOwened[extBestVarietyIdx];
  if(extpunctuationDifference > m_duParameter.outsideTradeLimit
      && duVarietyExt.number != varietyOwened[m_worstVarietyIdx].number){
    changeProduction(duVarietyExt.varietyData);
    if(varietyOwened[m_worstVarietyIdx].quantity == 1)
      updateWorstVar(varietyOwened[m_worstVarietyIdx].number);
  }
  if(intpunctuationDifference > m_duParameter.insideTradeLimit &&
      varietyOwened[bestVarietyIdx].number !=
      varietyOwened[m_worstVarietyIdx].number){
    changeProduction(varietyOwened[bestVarietyIdx].varietyData);
  }
  if(uniFLOAT(rand64) < m_duParameter.probabilityNewVar){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    m_grid[newPlace].setRandomVariety();
  }
  if(uniFLOAT(rand64) < m_duParameter.probabilityDeath){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    m_grid[newPlace].killVariety();
  }
}

// Takes one place (between the owened patches) where the worst variety exists
// and replace it with varietyData
void DomesticUnity::changeProduction(VarietyData varietyData){
  int worstVarietyPlace = findVariety(varietyOwened[m_worstVarietyIdx].number);
  if(worstVarietyPlace == -1){
    cout << "ERROR: VARIETY TO BE REMOVED NOT FOUND." << endl;
    exit(-1);
  }
  m_grid[worstVarietyPlace].setVariety(varietyData);
}

// Find one place (between the owened patches) where the given variety exists.
// If it does not find return -1
int DomesticUnity::findVariety(int var){
  for(auto i : m_indexOwenedPatches)
    if(m_grid[i].variety.varietyNumber == var)
      return i;
  return -1;
}

// Receive the actual worstVariety and update the worst variety, so that it is
// not the received value
void DomesticUnity::updateWorstVar(int oldWorstVar){
  float worstVarPunctuation = 100;
  for(uint i = 0; i < varietyOwened.size(); ++i){
    if(varietyOwened[i].punctuation < worstVarPunctuation &&
        varietyOwened[i].number != oldWorstVar){
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
