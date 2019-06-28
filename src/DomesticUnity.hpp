#ifndef DOMESTICUNITY_H
#define DOMESTICUNITY_H

// DomesticUnity class. It is where the decision making takes place.
class DomesticUnity{
private:
  struct DUvariety;
  DomesticUnity* m_domesticUnity;
  Patch* m_grid;
  DUParameter m_duParameter;
  float m_DUpreference;
  std::vector<int> m_indexOwenedPatches;
  void changeProduction(VarietyData varietyData, int varNumber = -10);
  int findVariety(int var);
  float computePunctuation(float varFitness, float varAppererence);
  void fillvarietyOwened(std::map<int,std::vector<float> >* varietyData);
  int computeBestDU(float * bestDUpunctuation);
  float computeMaxDelta(int * minorDeltaIdx, int * majorDeltaIdx);
  float renormalizationFunction(float x);
public:
  float punctuation;
  float fitness_punctuation;
  std::vector<DUvariety> varietyOwened;
  std::vector<int> indexLinkedDU;
  int numberVarietyOwened(void);
  void initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid,
      std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches,
      DUParameter t_duParameter);
  void computeDUpunctuations(void);
  void iterateDU(void);
};

struct DomesticUnity::DUvariety{
  int number;
  int quantity;
  float punctuation;
  float fitness_punctuation;
  VarietyData varietyData;
};

// Initialize DomesticUnity parameters (have to test member initialize list)
void DomesticUnity::initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid,
    std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches,
    DUParameter t_duParameter){
  m_domesticUnity = t_domesticUnity;
  m_grid = t_grid;
  indexLinkedDU = t_indexLinkedDU;
  m_indexOwenedPatches = t_indexOwenedPatches;
  m_duParameter.outsideTradeLimit = t_duParameter.outsideTradeLimit;
  m_duParameter.insideTradeLimit = t_duParameter.insideTradeLimit;
  m_duParameter.selectionStrength = t_duParameter.selectionStrength;
  m_duParameter.alpha = t_duParameter.alpha;
  m_duParameter.probabilityNewVar = t_duParameter.probabilityNewVar;
  m_DUpreference = gauss(rand64);
  while(!(m_DUpreference > 0 && m_DUpreference < 1))
    m_DUpreference = gauss(rand64);
  varietyOwened = std::vector<DUvariety>(m_indexOwenedPatches.size());
  uniIntPlace.param(std::uniform_int_distribution<long>::param_type(0,
        m_indexOwenedPatches.size()-1));
}

// Iterate over the Oweneds Patches, colect varieties and set varietyOwened
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
      newVar.fitness_punctuation = m_duParameter.alpha *
        itr->second[1] / itr->second[0];
    }
    varietyOwened.push_back(newVar);
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

// Iterate the domestic unity. Changes cultivated variety if
// max(majorDelta, minorDelta) > 'i',
// exist a null variety,
// extpunctuationDifference > 'o',
// or with a probability 'p' a new variety is created
void DomesticUnity::iterateDU(void){
  float bestDUpunctuation;
  int bestDUidx = computeBestDU(&bestDUpunctuation);
  float extpunctuationDifference = bestDUpunctuation - punctuation;

  int majorDeltaIdx;
  int minorDeltaIdx;
  float maxDelta = computeMaxDelta(&minorDeltaIdx, &majorDeltaIdx);

  if(maxDelta > m_duParameter.insideTradeLimit &&
      minorDeltaIdx != majorDeltaIdx){
    changeProduction(varietyOwened[majorDeltaIdx].varietyData,
        varietyOwened[minorDeltaIdx].number);
  }
  while(findVariety(-1) >= 0)
    changeProduction(varietyOwened[majorDeltaIdx].varietyData,-1);
  if(extpunctuationDifference > m_duParameter.outsideTradeLimit){
    std::vector<DUvariety>* duVec = &m_domesticUnity[bestDUidx].varietyOwened;
    int extBestVarietyIdx = floor(uniFLOAT(rand64) * duVec->size());
    while(duVec->at(extBestVarietyIdx).number == -1)
      extBestVarietyIdx = floor(uniFLOAT(rand64) * duVec->size());
    changeProduction(duVec->at(extBestVarietyIdx).varietyData);
  }
  if(uniFLOAT(rand64) < m_duParameter.probabilityNewVar){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    m_grid[newPlace].setRandomVariety();
  }
}

// If varNumber is specified remove one of it and replace it with varietyData
// If it is not remove one random variety and replace it with varietyData
void DomesticUnity::changeProduction(VarietyData varietyData, int varNumber){
  int varietyRemovePlace;
  if(varNumber == -10)
    varietyRemovePlace = m_indexOwenedPatches[uniIntPlace(rand64)];
  else{
    varietyRemovePlace = findVariety(varNumber);
    if(varietyRemovePlace == -1){
      cout << "ERROR: VARIETY TO BE REMOVED NOT FOUND." << endl;
      exit(-1);
    }
  }
  m_grid[varietyRemovePlace].setVariety(varietyData);
}

// Calculate the idx of the connected domestic unity that has better
// punctuation, also calculates bestDUpunctuation
int DomesticUnity::computeBestDU(float * bestDUpunctuation){
  int bestDUindex = 0;
  *bestDUpunctuation = -1;
  for(uint i = 0; i < indexLinkedDU.size(); ++i){
    if(m_domesticUnity[indexLinkedDU[i]].punctuation > *bestDUpunctuation){
      bestDUindex = indexLinkedDU[i];
      *bestDUpunctuation = m_domesticUnity[bestDUindex].punctuation;
    }
  }
  return bestDUindex;
}

// Logistic function that takes [0,1] -> [0,1]
float DomesticUnity::renormalizationFunction(float x){
  return (m_duParameter.selectionStrength*x) /
    sqrt(1+pow(2 * m_duParameter.selectionStrength * x, 2)) + 0.5;
}

// Calculate the difference between intented number of variety and real number
// of variety. Return that index of the variaty that has major and minor
// differences.
float DomesticUnity::computeMaxDelta(int * minorDeltaIdx, int * majorDeltaIdx){
  float minorDelta = -10;
  float majorDelta = -10;
  float totalPunctuationNorm = 0;
  float averagePunctuation = 0;
  *minorDeltaIdx = -10;
  *majorDeltaIdx = -10;
  for(uint i = 0; i < varietyOwened.size(); ++i)
    averagePunctuation += varietyOwened[i].punctuation;
  averagePunctuation /= varietyOwened.size();
  for(uint i = 0; i < varietyOwened.size(); ++i){
    totalPunctuationNorm += renormalizationFunction(
        varietyOwened[i].punctuation - averagePunctuation);
  }

  for(uint i = 0; i < varietyOwened.size(); ++i){
    float temp = renormalizationFunction(varietyOwened[i].punctuation -
      averagePunctuation) / totalPunctuationNorm -
      (float) varietyOwened[i].quantity / m_indexOwenedPatches.size();
    if(temp > majorDelta){
      majorDelta = temp;
      *majorDeltaIdx = i;
    }
    if(temp < - minorDelta && varietyOwened[i].number != -1){
      minorDelta = - temp;
      *minorDeltaIdx = i;
    }
  }
  return std::max(majorDelta,minorDelta);
}

// Find one place (between the owened patches) where the given variety exists.
// If it does not find return -1
int DomesticUnity::findVariety(int var){
  for(auto i : m_indexOwenedPatches)
    if(m_grid[i].variety.varietyNumber == var)
      return i;
  return -1;
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
