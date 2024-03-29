#ifndef METRICS_H
#define METRICS_H

/*
Class responsible for computing model's metrics.
*/

class Metrics{
public:
  Metrics() = default;
  Metrics(Parameter t_parameter, Household* t_household, Patch* t_grid);
  int computeVarietyRichness();
  float computeVarietyMeanProfile();
  std::vector<float> computeHDprofile();
  std::vector<float> computeVarietyProfile();
  std::vector<float> computeVarietyQuantity();
  float computeBergerParker();
  float computeBergerParkerHD();
  float computeShannon();
  float computeSimpson();
  std::vector<float> computeScoreAverage();
  std::vector<float> computeProductivityProfile();
  std::vector<float> computeQualityProfile();
  float computeAverageSimpson();
  float computeAverageShannon();
private:
  Parameter m_parameter;
  Household* m_household;
  Patch* m_grid;
};

Metrics::Metrics(Parameter t_parameter, Household* t_household, Patch* t_grid)
  : m_parameter(t_parameter)
  , m_household(t_household)
  , m_grid(t_grid) {}

// Return the frequency of the number of varieties owened by the
// Households
std::vector<float> Metrics::computeHDprofile(){
  std::vector<float> hdDistribution(m_parameter.latticeSize + 1, 0);
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    int num = m_household[i].numberVarietyOwened();;
    hdDistribution[num] += 1.0 / m_parameter.numberHousehold;
  }
  return hdDistribution;
}

// Return the frequency of the number of HD cultivating each variety
std::vector<float> Metrics::computeVarietyProfile(){
  std::map<int,int> numberHD;
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    for(auto var : m_household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        ++numberHD[var.number];
      else
        numberHD[var.number] = 1;
    }
  }
  if(numberHD.count(-1) > 0)
    numberHD.erase(-1);
  std::vector<float> varietyProfile(m_parameter.numberHousehold, 0);
  for(auto i : numberHD)
    varietyProfile[i.second-1] += 1.0 / numberHD.size();

  return varietyProfile;
}

// Return the frequency of the area cultivated by each variety
std::vector<float> Metrics::computeVarietyQuantity(){
  std::map<int,int> numberHD;
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    for(auto var : m_household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        numberHD[var.number] += var.quantity;
      else
        numberHD[var.number] = var.quantity;
    }
  }
  if(numberHD.count(-1) > 0)
    numberHD.erase(-1);
  float step = 0.2;
  float size = floor(log10(m_parameter.number_patches())/step) + 1;
  std::vector<float> varietyProfile(size, 0);
  for(auto i : numberHD){
    int idx = floor(log10(i.second) / step);
    varietyProfile[idx] += 1.0 / numberHD.size();
  }

  return varietyProfile;
}

// Return the frequency of the area cultivated by each variety
float Metrics::computeBergerParker(){
  std::map<int,int> numberHD;
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    for(auto var : m_household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        numberHD[var.number] += var.quantity;
      else
        numberHD[var.number] = var.quantity;
    }
  }
  if(numberHD.count(-1) > 0)
    numberHD.erase(-1);
  float bergerParker = -1;
  for(auto i : numberHD){
    if(i.second > bergerParker)
      bergerParker = i.second;
  }
  bergerParker /= m_parameter.latticeSize;
  return bergerParker;
}

// Return the average area cultivated by the most present variety of each HD
float Metrics::computeBergerParkerHD(){
  std::vector<int> quantityBestVarHD(m_parameter.numberHousehold, -1);
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    for(auto var : m_household[i].varietyOwened){
      if(var.quantity > quantityBestVarHD[i] && var.number != -1)
        quantityBestVarHD[i] = var.quantity;
    }
  }
  float averageMaxQuantity = 0;
  for(int i = 0; i < m_parameter.numberHousehold; ++i)
    averageMaxQuantity += quantityBestVarHD[i];
  averageMaxQuantity /= m_parameter.numberHousehold;
  return averageMaxQuantity;
}

// Return the Simpson diversity index for the community
float Metrics::computeSimpson(){
  std::map<int,int> numberHD;
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    for(auto var : m_household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        numberHD[var.number] += var.quantity;
      else
        numberHD[var.number] = var.quantity;
    }
  }
  float totalArea = m_parameter.number_patches();
  if(numberHD.count(-1) > 0){
    totalArea -= numberHD[-1];
    numberHD.erase(-1);
  }
  float simpson = 0;
  for(auto i : numberHD)
    simpson += ((i.second / totalArea) * (i.second / totalArea));

  return (1 - simpson);
}

// Return the Shannon diversity index for the community
float Metrics::computeShannon(){
  std::map<int,int> numberHD;
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    for(auto var : m_household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        numberHD[var.number] += var.quantity;
      else
        numberHD[var.number] = var.quantity;
    }
  }
  float totalArea = m_parameter.number_patches();
  if(numberHD.count(-1) > 0){
    totalArea -= numberHD[-1];
    numberHD.erase(-1);
  }
  if(numberHD.size() == 1)
    return 1;
  float shannon = 0;
  for(auto i : numberHD)
    shannon += (-1 * (i.second / totalArea) * log(i.second / totalArea));
  shannon /= log(numberHD.size());
  return shannon;
}

// Return the average score of Households
std::vector<float> Metrics::computeScoreAverage(){
  std::vector<float> scoreAverage(2, 0);
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    scoreAverage[0] += m_household[i].score;
    scoreAverage[1] += m_household[i].productivity_score;
  }
  scoreAverage[0] /= m_parameter.numberHousehold;
  scoreAverage[1] /= m_parameter.numberHousehold;
  return scoreAverage;
}

// Count the number of different varieties in the grid, return an int with
// that number
int Metrics::computeVarietyRichness(){
  std::map<int,bool> varietyAvailability;

  for(int i = 0; i < m_parameter.number_patches(); ++i){
    int varNumber = m_grid[i].variety.varietyNumber;
    varietyAvailability[varNumber] = true;
  }
  if(varietyAvailability.count(-1))
    return varietyAvailability.size() - 1;
  return varietyAvailability.size();
}

// Return the mean number of varieties owened by the Households
float Metrics::computeVarietyMeanProfile(){
  std::vector<float> varietyDistribution(m_parameter.latticeSize, 0);
  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    int num = m_household[i].numberVarietyOwened();
    varietyDistribution[num] += 1.0 / m_parameter.numberHousehold;
  }
  float mean = 0;
  for(int i = 0; i < m_parameter.latticeSize; ++i){
    mean += varietyDistribution[i] * i;
  }
  return mean;
}

// Return the frequency of each range of quality. The range size is 0.05
std::vector<float> Metrics::computeQualityProfile(){
  float step = 0.05;
  std::vector<float> varFrequency(round(1 / step), 0);
  for(int i = 0; i < m_parameter.number_patches(); ++i){
    if(m_grid[i].variety.quality != -1){
      int idx = floor(m_grid[i].variety.quality / step);
      varFrequency[idx] += (1.0 / m_parameter.number_patches());
    }
  }
  return varFrequency;
}

// Return the frequency of each range of productivity. The range size is 0.05
std::vector<float> Metrics::computeProductivityProfile(){
  float step = 0.05;
  std::vector<float> productivityFrequency(round(1 / step), 0);
  for(int i = 0; i < m_parameter.number_patches(); ++i){
    if(m_grid[i].productivity != 0){
      float position = step;
      while(position < m_grid[i].productivity)
        position += step;
      int tick = round((position - step) / step);
      productivityFrequency[tick] += (1.0 / (m_parameter.number_patches()));
    }
  }
  return productivityFrequency;
}

float Metrics::computeAverageSimpson(){
  float simpson = 0;
  for(int i = 0; i < m_parameter.numberHousehold; ++i)
    simpson += m_household[i].computeSimpsonHD();
  simpson /= m_parameter.numberHousehold;
  return simpson;
}

float Metrics::computeAverageShannon(){
  float shannon = 0;
  for(int i = 0; i < m_parameter.numberHousehold; ++i)
    shannon += m_household[i].computeShannonHD();
  shannon /= m_parameter.numberHousehold;
  return shannon;
}

#endif
