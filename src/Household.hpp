#ifndef HOUSEHOLD_H
#define HOUSEHOLD_H

// Household class. It is where the decision making takes place.
class Household{
private:
  struct HDvariety;
  Household* m_household;
  Patch* m_grid;
  HDParameter m_hdParameter;
  float m_HDpreference;
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
  std::vector<HDvariety> varietyOwened;
  std::vector<int> indexLinkedHD;
  int numberVarietyOwened(void);
  void initializeHD(Household* t_household, Patch* t_grid,
      std::vector<int> t_indexLinkedHD, std::vector<int> t_indexOwenedPatches,
      HDParameter t_hdParameter);
  void computeHDpunctuations(void);
  void iterateHD(void);
  float computeSimpsonHD(void);
  float computeShannonHD(void);
};

struct Household::HDvariety{
  int number;
  int quantity;
  float punctuation;
  float productivity_punctuation;
  VarietyData varietyData;
};

// Initialize Household parameters (have to test member initialize list)
void Household::initializeHD(Household* t_household, Patch* t_grid,
    std::vector<int> t_indexLinkedHD, std::vector<int> t_indexOwenedPatches,
    HDParameter t_hdParameter){
  m_household = t_household;
  m_grid = t_grid;
  indexLinkedHD = t_indexLinkedHD;
  m_indexOwenedPatches = t_indexOwenedPatches;
  m_hdParameter.outsideTradeLimit = t_hdParameter.outsideTradeLimit;
  m_hdParameter.selectionStrength = t_hdParameter.selectionStrength;
  m_hdParameter.alpha = t_hdParameter.alpha;
  m_hdParameter.probabilityNewVar = t_hdParameter.probabilityNewVar;
  m_hdParameter.percentageNewRandomVar = t_hdParameter.percentageNewRandomVar;
  m_HDpreference = gaussPref(rand64);
  while(!(m_HDpreference > 0 && m_HDpreference < 1))
    m_HDpreference = gaussPref(rand64);
  varietyOwened = std::vector<HDvariety>(m_indexOwenedPatches.size());
  uniIntPlace.param(std::uniform_int_distribution<long>::param_type(0,
        m_indexOwenedPatches.size()-1));
}

// Iterate over the Oweneds Patches, colect varieties and set varietyOwened
void Household::computeHDpunctuations(void){
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

  //Set HD punctuation
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
void Household::fillvarietyOwened(std::map<int,
    std::vector<float> >* varietyData){
  varietyOwened.clear();
  map<int, std::vector<float> >::iterator itr;
  for(itr = varietyData->begin(); itr != varietyData->end(); ++itr){
    HDvariety newVar;
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
      newVar.productivity_punctuation = m_hdParameter.alpha *
        itr->second[1] / itr->second[0];
    }
    varietyOwened.push_back(newVar);
  }
}

// Compute the punctuation based on the productivity, quality and alpha
float Household::computePunctuation(float varProductivity,
    float varQuality){
  float culturalPunc = 1 - 2 *  std::min(abs(varQuality -
        m_HDpreference), 1 - abs(varQuality - m_HDpreference));
  return m_hdParameter.alpha * varProductivity +
    (1 - m_hdParameter.alpha) * culturalPunc;
}

// Iterate the household. Changes cultivated variety if
// max(majorDelta, minorDelta) > 'i',
// exist a null variety,
// extpunctuationDifference > 'o',
// or with a probability 'p' a new variety is created
void Household::iterateHD(void){
  int majorDeltaIdx;
  int minorDeltaIdx;
  computeDeltas(&minorDeltaIdx, &majorDeltaIdx);

  if(minorDeltaIdx != majorDeltaIdx){
    changeProduction(varietyOwened[majorDeltaIdx].varietyData,
        varietyOwened[minorDeltaIdx].number);
    int numberTrades = uniIntChangeHdProduction(rand64);
    for(int i = 0; i < numberTrades; ++i){
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

  for(auto ud :indexLinkedHD){
    if(uniFLOAT(rand64) <
        m_household[ud].punctuation * m_hdParameter.outsideTradeLimit){
      std::vector<HDvariety>* hdVec = &m_household[ud].varietyOwened;
      int extBestVarietyIdx = floor(uniFLOAT(rand64) * hdVec->size());
      while(hdVec->at(extBestVarietyIdx).number == -1)
        extBestVarietyIdx = floor(uniFLOAT(rand64) * hdVec->size());
      changeProduction(hdVec->at(extBestVarietyIdx).varietyData);
    }
  }

  if(uniFLOAT(rand64) < m_hdParameter.probabilityNewVar *
      m_hdParameter.percentageNewRandomVar){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    m_grid[newPlace].setRandomVariety();
  }

  if(uniFLOAT(rand64) < m_hdParameter.probabilityNewVar *
      (1 - m_hdParameter.percentageNewRandomVar)){
    int newPlace = m_indexOwenedPatches[uniIntPlace(rand64)];
    int newPlace2 = m_indexOwenedPatches[uniIntPlace(rand64)];
    VarietyData varietyData1 = m_grid[newPlace].giveVarietyData();
    VarietyData varietyData2 = m_grid[newPlace2].giveVarietyData();
    m_grid[newPlace].setNewVariety(varietyData1, varietyData2);
  }
}

// If varNumber is specified remove one of it and replace it with varietyData
// If it is not remove one random variety and replace it with varietyData
void Household::changeProduction(VarietyData varietyData, int varNumber){
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
float Household::renormalizationFunction(float x){
  return (m_hdParameter.selectionStrength*x) /
    sqrt(1+pow(2 * m_hdParameter.selectionStrength * x, 2)) + 0.5;
}

// Calculate the difference between intented number of variety and real number
// of variety.
void Household::computeDeltas(int * minorDeltaIdx, int * majorDeltaIdx){
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
    if(varietyOwened[i].number == -1)
      continue;

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
int Household::findVariety(int var){
  for(auto i : m_indexOwenedPatches)
    if(m_grid[i].variety.varietyNumber == var)
      return i;
  return -1;
}

// Return the number of variety owened by the HD, excluding empty patches.
int Household::numberVarietyOwened(void){
  for(auto i : varietyOwened){
    if(i.number == -1)
      return varietyOwened.size() - 1;
  }
  return varietyOwened.size();
}

float Household::computeSimpsonHD(void){
  float simpson = 0;
  for(auto i : varietyOwened){
    if(i.number != -1)
      simpson += pow((float) i.quantity / m_indexOwenedPatches.size(), 2);
  }
  return simpson;
}

float Household::computeShannonHD(void){
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
