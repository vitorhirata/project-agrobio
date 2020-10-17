#ifndef METRICS_H
#define METRICS_H

class Metrics{
public:
  float computeVarietyMeanProfile(Household* household,
      const int t_numberHousehold, const int t_HDsize);
  std::vector<float> computeHDprofile(Household* household,
      const int t_numberHousehold, const int t_HDsize);
  std::vector<float> computeVarietyProfile(
      Household* household, const int t_numberHousehold);
  std::vector<float> computeVarietyQuantity(Household* household,
      const int t_numberHousehold, const int t_latticeSize);
  float computeBergerParker(Household* household,
      const int t_numberHousehold, const int t_latticeSize);
  float computeBergerParkerHD(Household* household,
      const int t_numberHousehold);
  float computeShannon(Household* household,
      const int t_numberHousehold, const int t_latticeSize);
  float computeSimpson(Household* household,
      const int t_numberHousehold, const int t_latticeSize);
  std::vector<float> computePunctuationAverage(Household* household,
      const int t_numberHousehold);
  std::vector<float> computeProductivityProfile(
      Patch* t_grid, const int t_latticeSize);
  std::vector<float> computeQualityProfile(
      Patch* t_grid, const int t_latticeSize);
  float computeCorrelation(Household* household, int numberHD);
};

// Return the frequency of the number of varieties owened by the
// Households
std::vector<float> Metrics::computeHDprofile(Household* household,
    const int t_numberHousehold, const int t_HDsize){
  std::vector<float> hdDistribution(t_HDsize + 1, 0);
  for(int i = 0; i < t_numberHousehold; ++i){
    int num = household[i].numberVarietyOwened();;
    hdDistribution[num] += 1.0 / t_numberHousehold;
  }
  return hdDistribution;
}

// Return the frequency of the number of HD cultivating each variety
std::vector<float> Metrics::computeVarietyProfile(
    Household* household, const int t_numberHousehold){
  std::map<int,int> numberHD;
  for(int i = 0; i < t_numberHousehold; ++i){
    for(auto var : household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        ++numberHD[var.number];
      else
        numberHD[var.number] = 1;
    }
  }
  if(numberHD.count(-1) > 0)
    numberHD.erase(-1);
  std::vector<float> varietyProfile(t_numberHousehold, 0);
  for(auto i : numberHD)
    varietyProfile[i.second-1] += 1.0 / numberHD.size();

  return varietyProfile;
}

// Return the frequency of the area cultivated by each variety
std::vector<float> Metrics::computeVarietyQuantity(Household* household,
    const int t_numberHousehold, const int t_latticeSize){
  std::map<int,int> numberHD;
  for(int i = 0; i < t_numberHousehold; ++i){
    for(auto var : household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        numberHD[var.number] += var.quantity;
      else
        numberHD[var.number] = var.quantity;
    }
  }
  if(numberHD.count(-1) > 0)
    numberHD.erase(-1);
  float step = 0.2;
  float size = floor(log10(t_latticeSize * t_latticeSize)/step) + 1;
  std::vector<float> varietyProfile(size, 0);
  for(auto i : numberHD){
    int idx = floor(log10(i.second) / step);
    varietyProfile[idx] += 1.0 / numberHD.size();
  }

  return varietyProfile;
}

// Return the frequency of the area cultivated by each variety
float Metrics::computeBergerParker(Household* household,
    const int t_numberHousehold, const int t_latticeSize){
  std::map<int,int> numberHD;
  for(int i = 0; i < t_numberHousehold; ++i){
    for(auto var : household[i].varietyOwened){
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
  bergerParker /= t_latticeSize;
  return bergerParker;
}

// Return the average area cultivated by the most present variety of each HD
float Metrics::computeBergerParkerHD(Household* household,
    const int t_numberHousehold){
  std::vector<int> quantityBestVarHD(t_numberHousehold, -1);
  for(int i = 0; i < t_numberHousehold; ++i){
    for(auto var : household[i].varietyOwened){
      if(var.quantity > quantityBestVarHD[i] && var.number != -1)
        quantityBestVarHD[i] = var.quantity;
    }
  }
  float averageMaxQuantity = 0;
  for(int i = 0; i < t_numberHousehold; ++i)
    averageMaxQuantity += quantityBestVarHD[i];
  averageMaxQuantity /= t_numberHousehold;
  return averageMaxQuantity;
}

// Return the Simpson diversity index for the community
float Metrics::computeSimpson(Household* household,
    const int t_numberHousehold, const int t_latticeSize){
  std::map<int,int> numberHD;
  for(int i = 0; i < t_numberHousehold; ++i){
    for(auto var : household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        numberHD[var.number] += var.quantity;
      else
        numberHD[var.number] = var.quantity;
    }
  }
  float totalArea = t_latticeSize * t_latticeSize;
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
float Metrics::computeShannon(Household* household,
    const int t_numberHousehold, const int t_latticeSize){
  std::map<int,int> numberHD;
  for(int i = 0; i < t_numberHousehold; ++i){
    for(auto var : household[i].varietyOwened){
      if(numberHD.count(var.number) > 0)
        numberHD[var.number] += var.quantity;
      else
        numberHD[var.number] = var.quantity;
    }
  }
  float totalArea = t_latticeSize * t_latticeSize;
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

// Return the average punctuation of Households
std::vector<float> Metrics::computePunctuationAverage(
    Household* household, const int t_numberHousehold){
  std::vector<float> punctuationAverage(2, 0);
  for(int i = 0; i < t_numberHousehold; ++i){
    punctuationAverage[0] += household[i].punctuation;
    punctuationAverage[1] += household[i].productivity_punctuation;
  }
  punctuationAverage[0] /= t_numberHousehold;
  punctuationAverage[1] /= t_numberHousehold;
  return punctuationAverage;
}

// Return the mean number of varieties owened by the Households
float Metrics::computeVarietyMeanProfile(Household* household,
    const int t_numberHousehold, const int t_HDsize){
  std::vector<float> varietyDistribution(t_HDsize, 0);
  for(int i = 0; i < t_numberHousehold; ++i){
    int num = household[i].numberVarietyOwened();
    varietyDistribution[num] += 1.0 / t_numberHousehold;
  }
  float mean = 0;
  for(int i = 0; i < t_HDsize; ++i){
    mean += varietyDistribution[i] * i;
  }
  return mean;
}

// Return the frequency of each range of quality. The range size is 0.05
std::vector<float> Metrics::computeQualityProfile(
    Patch* t_grid, const int t_latticeSize){
  float step = 0.05;
  std::vector<float> varFrequency(round(1 / step), 0);
  for(int i = 0; i < t_latticeSize*t_latticeSize; ++i){
    if(t_grid[i].variety.quality != -1){
      int idx = floor(t_grid[i].variety.quality / step);
      varFrequency[idx] += 1.0 / (t_latticeSize*t_latticeSize);
    }
  }
  return varFrequency;
}

// Return the frequency of each range of productivity. The range size is 0.05
std::vector<float> Metrics::computeProductivityProfile(
    Patch* t_grid, const int t_latticeSize){
  float step = 0.05;
  std::vector<float> productivityFrequency(round(1 / step), 0);
  for(int i = 0; i < t_latticeSize*t_latticeSize; ++i){
    if(t_grid[i].productivity != 0){
      float position = step;
      while(position < t_grid[i].productivity)
        position += step;
      int tick = round((position - step) / step);
      productivityFrequency[tick] += (1.0 / (t_latticeSize * t_latticeSize));
    }
  }
  return productivityFrequency;
}

float Metrics::computeCorrelation(Household* household, int numberHD){
  std::vector<int> degreeHD(numberHD);
  std::vector<int> varietyHD(numberHD);
  for(uint i = 0; i < numberHD; ++i){
    degreeHD[i] = household[i].indexLinkedHD.size();
    varietyHD[i] = household[i].varietyOwened.size();
  }
  float averageDegree = accumulate(degreeHD.begin(), degreeHD.end(), 0.0) /
    degreeHD.size();
  float averageVariety = accumulate(varietyHD.begin(), varietyHD.end(),0.0) /
    varietyHD.size();
  float correlationNumerator = 0;
  float correlationDenominator1 = 0;
  float correlationDenominator2 = 0;
  for(uint i = 0; i < numberHD; ++i){
    correlationNumerator += ((degreeHD[i]-averageDegree) *
        (varietyHD[i]-averageVariety));
    correlationDenominator1 += ((degreeHD[i]-averageDegree) *
        (degreeHD[i]-averageDegree));
    correlationDenominator2 += ((varietyHD[i]-averageVariety) *
        (varietyHD[i]-averageVariety));
  }
  float correlation = correlationNumerator / (sqrt(correlationDenominator1)*
      sqrt(correlationDenominator2));
  return correlation;
}

#endif
