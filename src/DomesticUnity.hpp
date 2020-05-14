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
  float computePunctuation(float varProductivity, float varQuality);
  void fillvarietyOwened(std::map<int,std::vector<float> >* varietyData);
  void computeDeltas(int * minorDeltaIdx, int * majorDeltaIdx);
  float renormalizationFunction(float x);
public:
  float punctuation;
  float productivity_punctuation;
  std::vector<DUvariety> varietyOwened;
  std::vector<int> indexLinkedDU;
  int numberVarietyOwened(void);
  void initializeDU(DomesticUnity* t_domesticUnity, Patch* t_grid,
      std::vector<int> t_indexLinkedDU, std::vector<int> t_indexOwenedPatches,
      DUParameter t_duParameter);
  void computeDUpunctuations(void);
  void iterateDU(void);
  float computeSimpsonDU(void);
  float computeShannonDU(void);
};

struct DomesticUnity::DUvariety{
  int number;
  int quantity;
  float punctuation;
  float productivity_punctuation;
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
  m_duParameter.selectionStrength = t_duParameter.selectionStrength;
  m_duParameter.alpha = t_duParameter.alpha;
  m_duParameter.probabilityNewVar = t_duParameter.probabilityNewVar;
  m_duParameter.percentageNewRandomVar = t_duParameter.percentageNewRandomVar;
  m_DUpreference = gaussPref(rand64);
  while(!(m_DUpreference > 0 && m_DUpreference < 1))
    m_DUpreference = gaussPref(rand64);
  varietyOwened = std::vector<DUvariety>(m_indexOwenedPatches.size());
  uniIntPlace.param(std::uniform_int_distribution<long>::param_type(0,
        m_indexOwenedPatches.size()-1));
}

// Iterate over the Oweneds Patches, colect varieties and set varietyOwened
void DomesticUnity::computeDUpunctuations(void){
  // Map the variety in a vector, where the 0th element is quantity,
  // the 1st productivity and the 2nd quality
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
    varietyData[varNumber][1] += m_grid[patchNumber].productivity;
    varietyData[varNumber][2] += m_grid[patchNumber].variety.quality;
  }

  fillvarietyOwened(&varietyData);

  //Set DU punctuation
  float puncTemp = 0;
  float productivityPuncTemp = 0;
  for(auto i : varietyOwened){
    puncTemp += (i.punctuation * i.quantity);
    productivityPuncTemp += (i.productivity_punctuation * i.quantity);
  }
  punctuation = puncTemp / m_indexOwenedPatches.size();
  productivity_punctuation = productivityPuncTemp / m_indexOwenedPatches.size();
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
      newVar.productivity_punctuation = 0;
    }
    else{
      newVar.punctuation = computePunctuation(itr->second[1] / itr->second[0],
          itr->second[2] / itr->second[0]);
      newVar.productivity_punctuation = m_duParameter.alpha *
        itr->second[1] / itr->second[0];
    }
    varietyOwened.push_back(newVar);
  }
}

// Compute the punctuation based on the productivity, quality and alpha
float DomesticUnity::computePunctuation(float varProductivity,
    float varQuality){
  float culturalPunc = 1 - 2 *  std::min(abs(varQuality -
        m_DUpreference), 1 - abs(varQuality - m_DUpreference));
  return m_duParameter.alpha * varProductivity +
    (1 - m_duParameter.alpha) * culturalPunc;
}

// Iterate the domestic unity. Changes cultivated variety if
// max(majorDelta, minorDelta) > 'i',
// exist a null variety,
// extpunctuationDifference > 'o',
// or with a probability 'p' a new variety is created
void DomesticUnity::iterateDU(void){
  int majorDeltaIdx;
  int minorDeltaIdx;
  computeDeltas(&minorDeltaIdx, &majorDeltaIdx);

  if(minorDeltaIdx != majorDeltaIdx){
    changeProduction(varietyOwened[majorDeltaIdx].varietyData,
        varietyOwened[minorDeltaIdx].number);
    while(uniFLOAT(rand64) < 0.7){
      if(findVariety(varietyOwened[minorDeltaIdx].number) >= 0){
        changeProduction(varietyOwened[majorDeltaIdx].varietyData,
            varietyOwened[minorDeltaIdx].number);
      }
      else
        changeProduction(varietyOwened[majorDeltaIdx].varietyData);
    }
  }

  while(findVariety(-1) >= 0)
    changeProduction(varietyOwened[majorDeltaIdx].varietyData,-1);

  for(auto ud :indexLinkedDU){
    if(uniFLOAT(rand64) <
        m_domesticUnity[ud].punctuation * m_duParameter.outsideTradeLimit){
      std::vector<DUvariety>* duVec = &m_domesticUnity[ud].varietyOwened;
      int extBestVarietyIdx = floor(uniFLOAT(rand64) * duVec->size());
      while(duVec->at(extBestVarietyIdx).number == -1)
        extBestVarietyIdx = floor(uniFLOAT(rand64) * duVec->size());
      changeProduction(duVec->at(extBestVarietyIdx).varietyData);
    }
  }

  if(uniFLOAT(rand64) < m_duParameter.probabilityNewVar *
      m_duParameter.percentageNewRandomVar){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    m_grid[newPlace].setRandomVariety();
  }

  if(uniFLOAT(rand64) < m_duParameter.probabilityNewVar *
      (1 - m_duParameter.percentageNewRandomVar)){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    int newPlace2 = m_indexOwenedPatches[uniIntPlace(rand64)];
    VarietyData varietyData1 = m_grid[newPlace].giveVarietyData();
    VarietyData varietyData2 = m_grid[newPlace2].giveVarietyData();
    m_grid[newPlace].setNewVariety(varietyData1, varietyData2);
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

// Logistic function that takes [0,1] -> [0,1]
float DomesticUnity::renormalizationFunction(float x){
  return (m_duParameter.selectionStrength*x) /
    sqrt(1+pow(2 * m_duParameter.selectionStrength * x, 2)) + 0.5;
}

// Calculate the difference between intented number of variety and real number
// of variety.
void DomesticUnity::computeDeltas(int * minorDeltaIdx, int * majorDeltaIdx){
  float minorDelta = -10;
  float majorDelta = -10;
  float totalPunctuationNorm = 0;
  float averagePunctuation = 0;
  *minorDeltaIdx = -10;
  *majorDeltaIdx = -10;
  for(uint i = 0; i < varietyOwened.size(); ++i)
    averagePunctuation += varietyOwened[i].punctuation;
  averagePunctuation /= numberVarietyOwened();
  for(uint i = 0; i < varietyOwened.size(); ++i){
    if(varietyOwened[i].number != -1){
      totalPunctuationNorm += renormalizationFunction(
          varietyOwened[i].punctuation - averagePunctuation);
    }
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

float DomesticUnity::computeSimpsonDU(void){
  float simpson = 0;
  for(auto i : varietyOwened){
    if(i.number != -1)
      simpson += pow((float) i.quantity / m_indexOwenedPatches.size(), 2);
  }
  return simpson;
}

float DomesticUnity::computeShannonDU(void){
  if(numberVarietyOwened() == 1)
    return 1;
  float shannon = 0;
  for(auto i : varietyOwened){
    if(i.number != -1){
      shannon += (-1 * ((float) i.quantity / m_indexOwenedPatches.size()) *
          log((float) i.quantity / m_indexOwenedPatches.size()));
    }
  }
  shannon /= log(numberVarietyOwened());
  return shannon;
}

#endif
