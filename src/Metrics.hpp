namespace metrics{

  void sumResults(Result* result, Result* resultTemp){
    std::transform((*result).numberVariety.begin(),
        (*result).numberVariety.end(), (*resultTemp).numberVariety.begin(),
        (*result).numberVariety.begin(), std::plus<float>());
    std::transform((*result).meanVarietyDU.begin(),
        (*result).meanVarietyDU.end(), (*resultTemp).meanVarietyDU.begin(),
        (*result).meanVarietyDU.begin(), std::plus<float>());
    if(!(*resultTemp).totalPunctuation.empty()){
    std::transform((*result).totalPunctuation.begin(),
        (*result).totalPunctuation.end(),
        (*resultTemp).totalPunctuation.begin(),
        (*result).totalPunctuation.begin(), std::plus<float>());
    std::transform((*result).fitnessPunctuation.begin(),
        (*result).fitnessPunctuation.end(),
        (*resultTemp).fitnessPunctuation.begin(),
        (*result).fitnessPunctuation.begin(), std::plus<float>());
    }
    std::transform((*result).varietyDistribution.begin(),
        (*result).varietyDistribution.end(),
        (*resultTemp).varietyDistribution.begin(),
        (*result).varietyDistribution.begin(), std::plus<float>());
    std::transform((*result).fitnessFrequency.begin(),
        (*result).fitnessFrequency.end(),
        (*resultTemp).fitnessFrequency.begin(),
        (*result).fitnessFrequency.begin(), std::plus<float>());
    std::transform((*result).appearenceFrequency.begin(),
        (*result).appearenceFrequency.end(),
        (*resultTemp).appearenceFrequency.begin(),
        (*result).appearenceFrequency.begin(), std::plus<float>());
    std::transform((*result).duDistribution.begin(),
        (*result).duDistribution.end(), (*resultTemp).duDistribution.begin(),
        (*result).duDistribution.begin(), std::plus<float>());
  }

  // Return the frequency of the number of varieties owened by the
  // Domestic Unities
  std::vector<float> computeDUprofile(DomesticUnity* domesticUnity,
      const int t_numberDomesticUnity, const int t_DUsize){
    std::vector<float> duDistribution(t_DUsize, 0);
    for(int i = 0; i < t_numberDomesticUnity; ++i){
      int num = domesticUnity[i].numberVarietyOwened();;
      duDistribution[num-1] += 1.0 / t_numberDomesticUnity;
    }
    return duDistribution;
  }

  // Return the frequency of the number of DU cultivating each variety
  std::vector<float> computeVarietyProfile(
      DomesticUnity* domesticUnity, const int t_numberDomesticUnity){
    std::map<int,int> numberDU;
    for(int i = 0; i < t_numberDomesticUnity; ++i){
      for(auto var : domesticUnity[i].varietyOwened){
        if(numberDU.count(var.number) > 0)
          ++numberDU[var.number];
        else
          numberDU[var.number] = 1;
      }
    }
    if(numberDU.count(-1) > 0)
      numberDU.erase(-1);
    std::vector<float> varietyProfile(t_numberDomesticUnity, 0);
    for(auto i : numberDU)
      varietyProfile[i.second-1] += 1.0 / numberDU.size();

    return varietyProfile;
  }

  // Return the average punctuation of Domestic Unities
  std::vector<float> computePunctuationAverage(
      DomesticUnity* domesticUnity, const int t_numberDomesticUnity){
    std::vector<float> punctuationAverage(2, 0);
    for(int i = 0; i < t_numberDomesticUnity; ++i){
      punctuationAverage[0] += domesticUnity[i].punctuation;
      punctuationAverage[1] += domesticUnity[i].fitness_punctuation;
    }
    punctuationAverage[0] /= t_numberDomesticUnity;
    punctuationAverage[1] /= t_numberDomesticUnity;
    return punctuationAverage;
  }

  // Return the mean number of varieties owened by the Domestic Unities
  float computeVarietyMeanProfile(DomesticUnity* domesticUnity,
      const int t_numberDomesticUnity, const int t_DUsize){
    std::vector<float> varietyDistribution(t_DUsize, 0);
    for(int i = 0; i < t_numberDomesticUnity; ++i){
      int num = domesticUnity[i].numberVarietyOwened();
      varietyDistribution[num] += 1.0 / t_numberDomesticUnity;
    }
    float mean = 0;
    for(int i = 0; i < t_DUsize; ++i){
      mean += varietyDistribution[i] * i;
    }
    return mean;
  }

  // Return the frequency of each range of appearence. The range size is 0.05
  std::vector<float> computeAppearenceProfile(
      Patch* t_grid, const int t_latticeSize){
    float step = 0.05;
    std::vector<float> varFrequency(round(1 / step), 0);
    for(int i = 0; i < t_latticeSize*t_latticeSize; ++i){
      int idx = floor(t_grid[i].variety.appearence / step);
      varFrequency[idx] += 1.0 / (t_latticeSize*t_latticeSize);
    }
    return varFrequency;
  }

  // Return the frequency of each range of fitness. The range size is 0.05
  std::vector<float> computeFitnessProfile(
      Patch* t_grid, const int t_latticeSize){
    float step = 0.05;
    std::vector<float> fitnessFrequency(round(1 / step), 0);
    for(int i = 0; i < t_latticeSize*t_latticeSize; ++i){
      float position = step;
      while(position < t_grid[i].fitness)
        position += step;
      int tick = round((position - step) / step);
      fitnessFrequency[tick] += (1.0 / (t_latticeSize * t_latticeSize));
    }
    return fitnessFrequency;
  }

  float computeCorrelation(DomesticUnity* domesticUnity, int numberDU){
    std::vector<int> degreeDU(numberDU);
    std::vector<int> varietyDU(numberDU);
    for(uint i = 0; i < numberDU; ++i){
      degreeDU[i] = domesticUnity[i].indexLinkedDU.size();
      varietyDU[i] = domesticUnity[i].varietyOwened.size();
    }
    float averageDegree = accumulate(degreeDU.begin(), degreeDU.end(), 0.0) /
      degreeDU.size();
    float averageVariety = accumulate(varietyDU.begin(), varietyDU.end(),0.0) /
      varietyDU.size();
    float correlationNumerator = 0;
    float correlationDenominator1 = 0;
    float correlationDenominator2 = 0;
    for(uint i = 0; i < numberDU; ++i){
      correlationNumerator += ((degreeDU[i]-averageDegree) *
          (varietyDU[i]-averageVariety));
      correlationDenominator1 += ((degreeDU[i]-averageDegree) *
          (degreeDU[i]-averageDegree));
      correlationDenominator2 += ((varietyDU[i]-averageVariety) *
          (varietyDU[i]-averageVariety));
    }
    float correlation = correlationNumerator / (sqrt(correlationDenominator1)*
        sqrt(correlationDenominator2));
    return correlation;
  }

  void printParameters(fstream& arquivo, Parameter parameter){
    arquivo << "### PARAMETERS VALUE ###" << endl;
    arquivo << "### LATTICESIZE = " << parameter.latticeSize;
    arquivo << ", NVARIETY = " << parameter.numberInitialVariety;
    arquivo << ", NRESOURCE = " << parameter.numberResources;
    arquivo << ", NRESOURCEDIST = " << parameter.numberHabitat;
    arquivo << ", NETWORKTYPE = " << parameter.networkType;
    arquivo << ", PROBABILITYCONNECTION = " << parameter.probabilyConnectionER;
    arquivo << ", KWT = " << parameter.kWT;
    arquivo << ", BETAWT = " << parameter.betaWT;
    arquivo << ", MSF = " << parameter.mSF;
    arquivo << ", OUTSIDETRADELIMIT = " << parameter.outsideTradeLimit;
    arquivo << ", INSIDETRADELIMIT = " << parameter.insideTradeLimit;
    arquivo << ", ALPHA = " << parameter.alpha;
    arquivo << ", NDOMESTICUNITY = " << parameter.numberDomesticUnity;
    arquivo << ", PROBABILITYNEWVAR = " << parameter.probabilityNewVar;
    arquivo << ", PROBABILITYDEATH = " << parameter.probabilityDeath;
    arquivo << ", MAXTIME = " << parameter.maxTime;
    arquivo << ", TIMEINTERVAL = " << parameter.timeInterval;
    arquivo << ", NRUN = " << parameter.nRun << " ###" << endl << endl;
  }

  void printState(int t, Patch* grid, const int t_latticeSize){
    std::string name ("test/plot/standard");
    name += std::to_string(t);
    name += ".bmp";

    BMP Image;
    int SIZE = 10;
    Image.SetSize(t_latticeSize*SIZE,t_latticeSize*SIZE);
    Image.SetBitDepth(8);
    int r, g, b;

    for(int i = 0; i < t_latticeSize; ++i){
      for(int j = 0; j < t_latticeSize; ++j){
        int var  = grid[i*t_latticeSize+j].variety.varietyNumber;
        float alpha = 0.1;
        if(var == -1){
          r = g = b = 0;
          alpha = 0.0;
        }
        else
          floatToRGB(var, &r, &g, &b);
        for(int x = 0; x < SIZE; x++)
          for(int y = 0; y < SIZE; y++){
            Image(j*SIZE+y,i*SIZE+x)->Red = r;
            Image(j*SIZE+y,i*SIZE+x)->Green = g;
            Image(j*SIZE+y,i*SIZE+x)->Blue = b;
            Image(j*SIZE+y,i*SIZE+x)->Alpha = alpha;
          }
      }
    }
    //Image.SetDPI(1000,1000);
    Image.WriteToFile(name.c_str());

  }


  void floatToRGB(int n, int* R, int* G, int* B){
    float r, g, b, x = (float) n / 3500;

    if (x < 0 || x > 1){
      cout << "ERROR: INVALID VARIETY NUMBER." << endl;
      exit(-1);
    }
    if (x < 0.125){
      r = 0.5 + 4*x;
      g = 0;
      b = 0;
    }
    else if (x < 0.375){
      r = 1;
      g = -0.5 + 4*x;
      b = 0;
    }
    else if (x < 0.625){
      r = 2.5 - 4*x;
      g = 1;
      b = -1.5 + 4*x;
    }
    else if (x < 0.875){
      r = 0;
      g = 3.5 - 4*x;
      b = 1;
    }
    else{
      r = 0;
      g = 0;
      b = 4.5 - 4*x;
    }

    if (r < 0 || g < 0 || b < 0 || r > 1 || g > 1 || b > 1){
      cout << "ERROR: " << r << ", " << g << ", " << b << endl;
      exit(-1);
    }
    *R = (int) (r * 255);
    *G = (int) (g * 255);
    *B = (int) (b * 255);
  }
}
