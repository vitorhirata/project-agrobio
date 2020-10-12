namespace metrics{

  // Return the frequency of the number of varieties owened by the
  // Households
  std::vector<float> computeHDprofile(Household* household,
      const int t_numberHousehold, const int t_HDsize){
    std::vector<float> hdDistribution(t_HDsize + 1, 0);
    for(int i = 0; i < t_numberHousehold; ++i){
      int num = household[i].numberVarietyOwened();;
      hdDistribution[num] += 1.0 / t_numberHousehold;
    }
    return hdDistribution;
  }

  // Return the frequency of the number of HD cultivating each variety
  std::vector<float> computeVarietyProfile(
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
  std::vector<float> computeVarietyQuantity(Household* household,
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
  float computeBergerParker(Household* household,
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
  float computeBergerParkerHD(Household* household,
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
  float computeSimpson(Household* household,
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
  float computeShannon(Household* household,
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
  std::vector<float> computePunctuationAverage(
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
  float computeVarietyMeanProfile(Household* household,
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
  std::vector<float> computeQualityProfile(
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
  std::vector<float> computeProductivityProfile(
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

  float computeCorrelation(Household* household, int numberHD){
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

  void printParameters(fstream& arquivo, Parameter parameter){
    arquivo << "### PARAMETERS VALUE ###" << endl;
    arquivo << "### LATTICESIZE = " << parameter.latticeSize;
    arquivo << ", NVARIETY = " << parameter.numberInitialVariety;
    arquivo << ", NVARIETYHD = " << parameter.numberInitialVarietyHD;
    arquivo << ", NRESOURCE = " << parameter.numberResources;
    arquivo << ", NRESOURCEDIST = " << parameter.numberHabitat;
    arquivo << ", NETWORKTYPE = " << parameter.networkType;
    arquivo << ", PROBABILITYCONNECTION = " << parameter.probabilyConnectionER;
    arquivo << ", KWT = " << parameter.kWT;
    arquivo << ", BETAWT = " << parameter.betaWT;
    arquivo << ", MSF = " << parameter.mSF;
    arquivo << ", OUTSIDETRADELIMIT = " << parameter.outsideTradeLimit;
    arquivo << ", SELECTIONSTRENGTH = " << parameter.selectionStrength;
    arquivo << ", ALPHA = " << parameter.alpha;
    arquivo << ", NHOUSEHOLD = " << parameter.numberHousehold;
    arquivo << ", PROBABILITYNEWVAR = " << parameter.probabilityNewVar;
    arquivo << ", PERCENTAGENEWRANDVAR = " << parameter.percentageNewRandomVar;
    arquivo << ", CROSSINGDEVIATION = " << parameter.crossingDeviation;
    arquivo << ", DEATHPROBABILITY = " << parameter.deathProbability;
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
    float r, g, b, x = (float) n / 10101020;

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
