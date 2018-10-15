namespace metrics{

  // Return the frequency of the number of varieties owened by the Domestic Unities
  std::vector<float> computeVarietyProfile(DomesticUnity* domesticUnity, const int t_numberDomesticUnity, const int t_DUsize){
    std::vector<float> varietyDistribution(t_DUsize, 0);
    for(int i = 0; i < t_numberDomesticUnity; ++i){
      int num = domesticUnity[i].varietyOwened.size();
      varietyDistribution[num-1] += 1.0 / t_numberDomesticUnity;
    }
    return varietyDistribution;
  }

  // Return the mean number of varieties owened by the Domestic Unities
  float computeVarietyMeanProfile(DomesticUnity* domesticUnity, const int t_numberDomesticUnity, const int t_DUsize){
    std::vector<float> varietyDistribution(t_DUsize, 0);
    for(int i = 0; i < t_numberDomesticUnity; ++i){
      int num = domesticUnity[i].varietyOwened.size();
      varietyDistribution[num-1] += 1.0 / t_numberDomesticUnity;
    }
    float mean = 0;
    for(int i = 0; i < t_DUsize; ++i){
      mean += varietyDistribution[i] * (i + 1);
    }
    return mean;
  }

  // Return the frequency of each range of appearence. The range size is 0.05
  std::vector<float> computeAppearenceProfile(Patch* t_grid, Variety* variety, const int t_latticeSize, const int t_numberVariety){
    std::vector<float> varietyQuantity(t_numberVariety, 0);
    for(int i = 0; i < t_latticeSize*t_latticeSize; ++i)
      ++varietyQuantity[t_grid[i].plantedVariety];

    std::vector<float> varFrequency(round(1 / 0.05), 0);
    for(int i = 0; i < t_numberVariety; ++i){
      if(varietyQuantity[i] > 0){
        float j = 0;
        while(j < variety[i].appearence)
          j += 0.05;
        int tick = round((j - 0.05) / 0.05);
        varFrequency[tick] += varietyQuantity[i] / (t_latticeSize*t_latticeSize);
      }
    }
    return varFrequency;
  }

  // Return the frequency of each range of fitness. The range size is 0.05
  std::vector<float> computeFitnessProfile(Patch* t_grid, const int t_latticeSize){
    std::vector<float> fitnessFrequency(round(1 / 0.05), 0);
    for(int i = 0; i < t_latticeSize*t_latticeSize; ++i){
      float position = 0;
      while(position < t_grid[i].fitness)
        position += 0.05;
      int tick = round((position - 0.05) / 0.05);
      //cout << tick << ", " << t_grid[i].fitness << endl;
      fitnessFrequency[tick] += (1.0 / (t_latticeSize * t_latticeSize));
    }
    return fitnessFrequency;
  }

  void printParameters(fstream& arquivo, Parameter parameter){
    arquivo << "### PARAMETERS VALUE ###" << endl;
    arquivo << "### LATTICESIZE = " << parameter.latticeSize << ", NVARIETY = " << parameter.numberInitialVariety;
    arquivo << ", NRESOURCE = " << parameter.numberResources << ", NRESOURCEDIST = " << parameter.numberHabitat;
    arquivo << ", PROBABILITYCONNECTION = " << parameter.probabilyConnection << ", NDOMESTICUNITY = " << parameter.numberDomesticUnity;
    arquivo << ", OUTSIDETRADELIMIT = " << parameter.outsideTradeLimit << ", INSIDETRADELIMIT = " << parameter.insideTradeLimit;
    arquivo << ", ALPHA = " << parameter.alpha;
    arquivo << ", MAXTIME = " << parameter.maxTime << ", TIMEINTERVAL = " << parameter.timeInterval << ", NRUN = " << parameter.nRun << " ###" << endl << endl;
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
        int var  = grid[i*t_latticeSize+j].plantedVariety;
        floatToRGB(var, &r, &g, &b);
        for(int x = 0; x < SIZE; x++)
          for(int y = 0; y < SIZE; y++){
            Image(j*SIZE+y,i*SIZE+x)->Red = r;
            Image(j*SIZE+y,i*SIZE+x)->Green = g;
            Image(j*SIZE+y,i*SIZE+x)->Blue = b;
            Image(j*SIZE+y,i*SIZE+x)->Alpha = 0.1;
          }
      }
    }
    //Image.SetDPI(1000,1000);
    Image.WriteToFile(name.c_str());

  }


  void floatToRGB(int n, int* R, int* G, int* B){
    float r, g, b, x = (float) n / 100;

    if (x < 0 || x > 1){
      cout << "ERROR" << endl;
      exit(-1);
    }
    if (n == 0){
      r = 0;
      g = 0;
      b = 0;
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
