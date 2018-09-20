namespace worker{
  void Run_standard(void){
    Parameter parameter;

    clock_t tStart = clock();
    std::vector<int> result(parameter.maxTime/parameter.timeInterval, 0);
    std::vector<float> initialFrequency(round(1 / 0.05), 0);
    std::vector<float> finalFrequency(round(1 / 0.05), 0);
    std::vector<int> temp;
    std::vector<float> temp2;
    std::vector<float> temp3;

    for(int run = 0; run < parameter.nRun; ++run){
      Model model(parameter);
      std::tie(temp, temp2, temp3) = model.runStandard();
      std::transform(result.begin(), result.end(), temp.begin(), result.begin(), std::plus<float>());
      std::transform(initialFrequency.begin(), initialFrequency.end(), temp2.begin(), initialFrequency.begin(), std::plus<float>());
      std::transform(finalFrequency.begin(), finalFrequency.end(), temp3.begin(), finalFrequency.begin(), std::plus<float>());
    }

    fstream arquivo;
    arquivo.open("test/standard.csv",ios::out);
    metrics::printParameters(arquivo, parameter);
    arquivo << "time; nVar" << endl;
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
      arquivo << i*parameter.timeInterval << "; " << (float) result[i] / parameter.nRun << endl;
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    arquivo.close();

    fstream histotram1;
    fstream histotram2;
    histotram1.open("test/histogramInitial.csv",ios::out);
    histotram2.open("test/histogramFinal.csv",ios::out);
    metrics::printParameters(histotram1, parameter);
    metrics::printParameters(histotram2, parameter);
    histotram1 << "appearence; frequency" << endl;
    histotram2 << "appearence; frequency" << endl;
    for(int i = 0; i < round(1 / 0.05); ++i){
      histotram1 << i*0.05 + 0.025 << "; " << initialFrequency[i] / parameter.nRun << endl;
      histotram2 << i*0.05 + 0.025 << "; " << finalFrequency[i] / parameter.nRun << endl;
    }
    histotram1.close();
    histotram2.close();
  }

  void Run_plot(void){
    Parameter parameter;
    clock_t tStart = clock();
    Model model(parameter);

    std::vector<float> initialFrequency;
    std::vector<float> finalFrequency;
    std::vector<int> result;
    std::tie(result, initialFrequency, finalFrequency) = model.runPlot();

    fstream arquivo;
    arquivo.open("test/plot/standard.csv",ios::out);
    metrics::printParameters(arquivo, parameter);
    arquivo << "time; nVar" << endl;
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
      arquivo << i*parameter.timeInterval << "; " << result[i] << endl;
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    arquivo.close();

    fstream histotram1;
    fstream histotram2;
    histotram1.open("test/plot/histogramInitial.csv",ios::out);
    histotram2.open("test/plot/histogramFinal.csv",ios::out);
    metrics::printParameters(histotram1, parameter);
    metrics::printParameters(histotram2, parameter);
    histotram1 << "appearence; frequency" << endl;
    histotram2 << "appearence; frequency" << endl;
    for(int i = 0; i < round(1 / 0.05); ++i){
      histotram1 << i*0.05 + 0.025 << "; " << initialFrequency[i] << endl;
      histotram2 << i*0.05 + 0.025 << "; " << finalFrequency[i] << endl;
    }
    histotram1.close();
    histotram2.close();
  }


  void Run_varParam(char param, std::vector<float> paramList){
    Parameter parameter;

    fstream arquivo;
    arquivo.open(std::string ("test/varParam_")+param+".csv",ios::out);
    metrics::printParameters(arquivo, parameter);
    arquivo << "time; nVar; param" << endl;

    fstream histotram1;
    fstream histotram2;
    histotram1.open(std::string ("test/histogramVarInitial_") + param + ".csv",ios::out);
    histotram2.open(std::string ("test/histogramVarFinal_") + param + ".csv",ios::out);
    metrics::printParameters(histotram1, parameter);
    metrics::printParameters(histotram2, parameter);
    histotram1 << "appearence; frequency; param" << endl;
    histotram2 << "appearence; frequency; param" << endl;

    for(auto paramValue : paramList){
      switch (param){
        case 'o':
          parameter.outsideTradeLimit = paramValue;
          break;
        case 'i':
          parameter.insideTradeLimit = paramValue;
          break;
        case 'L':
          if (round(sqrt(paramValue)) * round(sqrt(paramValue)) != paramValue){
            cout << "ERROR: the latticeSize should be a perfect square." << endl;
            exit(-1);
          }
          parameter.latticeSize = paramValue;
          parameter.numberDomesticUnity = paramValue;
          break;
        case 'r':
          parameter.numberResourceDistribution = paramValue;
          break;
        case 'n':
          parameter.numberVariety = paramValue;
          break;
        case 'a':
          parameter.alpha = paramValue;
          break;
      }
      clock_t tStart = clock();
      std::vector<int> result(parameter.maxTime/parameter.timeInterval, 0);
      std::vector<float> initialFrequency(round(1 / 0.05), 0);
      std::vector<float> finalFrequency(round(1 / 0.05), 0);
      std::vector<int> temp;
      std::vector<float> temp2;
      std::vector<float> temp3;
      for(int run = 0; run < parameter.nRun; ++run){
        Model model(parameter);
        std::tie(temp, temp2, temp3) = model.runStandard();
        std::transform(result.begin(), result.end(), temp.begin(), result.begin(), std::plus<float>());
        std::transform(initialFrequency.begin(), initialFrequency.end(), temp2.begin(), initialFrequency.begin(), std::plus<float>());
        std::transform(finalFrequency.begin(), finalFrequency.end(), temp3.begin(), finalFrequency.begin(), std::plus<float>());
      }

      for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
        arquivo << i*parameter.timeInterval << "; " << (float) result[i] / parameter.nRun << "; " << paramValue << endl;

      for(int i = 0; i < round(1 / 0.05); ++i){
        histotram1 << i*0.05 + 0.025 << "; " << initialFrequency[i] / parameter.nRun << "; " << paramValue << endl;
        histotram2 << i*0.05 + 0.025 << "; " << finalFrequency[i] / parameter.nRun << "; " << paramValue << endl;
      }

      cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    }
    arquivo.close();
    histotram1.close();
    histotram2.close();
  }

}



namespace metrics{

  // Return the frequency of each range of appearence. The range size is 0.05
  std::vector<float> computeVarietyProfile(Patch* t_grid, Variety* variety, const int t_latticeSize, const int t_numberVariety){
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

  void printParameters(fstream& arquivo, Parameter parameter){
    arquivo << "### PARAMETERS VALUE ###" << endl;
    arquivo << "### LATTICESIZE = " << parameter.latticeSize << ", NVARIETY = " << parameter.numberVariety;
    arquivo << ", NRESOURCE = " << parameter.numberResources << ", NRESOURCEDIST = " << parameter.numberResourceDistribution;
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
