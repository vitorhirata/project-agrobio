namespace worker{
  void Run_standard(void){
    Parameter parameter;
    Result result(parameter.maxTime/parameter.timeInterval, round(1 / 0.05));
    Result resultTemp(0,0);

    clock_t tStart = clock();
    for(int run = 0; run < parameter.nRun; ++run){
      Model model(parameter);
      resultTemp = model.runStandard();
      std::transform(result.numberVariety.begin(), result.numberVariety.end(), resultTemp.numberVariety.begin(), result.numberVariety.begin(), std::plus<float>());
      std::transform(result.fitnessFrequency.begin(), result.fitnessFrequency.end(), resultTemp.fitnessFrequency.begin(), result.fitnessFrequency.begin(), std::plus<float>());
      std::transform(result.appearenceFrequency.begin(), result.appearenceFrequency.end(), resultTemp.appearenceFrequency.begin(), result.appearenceFrequency.begin(), std::plus<float>());
    }
    time_t now = time(NULL);
    std::string timestr = to_string(now);

    fstream varietyFile;
    varietyFile.open("test/" + timestr + "_standard.csv",ios::out);
    metrics::printParameters(varietyFile, parameter);
    varietyFile << "time; nVar" << endl;
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
      varietyFile << i*parameter.timeInterval << "; " << (float) result.numberVariety[i] / parameter.nRun << endl;
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    varietyFile.close();

    fstream fitnessFile;
    fstream appearenceFile;
    fitnessFile.open("test/" + timestr + "_histogramFitness.csv",ios::out);
    appearenceFile.open("test/" + timestr + "_histogramAppearence.csv",ios::out);
    metrics::printParameters(fitnessFile, parameter);
    metrics::printParameters(appearenceFile, parameter);
    fitnessFile << "appearence; frequency" << endl;
    appearenceFile << "appearence; frequency" << endl;
    for(int i = 0; i < round(1 / 0.05); ++i){
      fitnessFile << i*0.05 + 0.025 << "; " << result.fitnessFrequency[i] / parameter.nRun << endl;
      appearenceFile << i*0.05 + 0.025 << "; " << result.appearenceFrequency[i] / parameter.nRun << endl;
    }
    fitnessFile.close();
    appearenceFile.close();
  }

  void Run_plot(void){
    Parameter parameter;
    Model model(parameter);
    Result result(0, 0);
    clock_t tStart = clock();
    result = model.runPlot();

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    fstream varietyFile;
    varietyFile.open("test/plot/" + timestr + "_standard.csv",ios::out);
    metrics::printParameters(varietyFile, parameter);
    varietyFile << "time; nVar" << endl;
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
      varietyFile << i*parameter.timeInterval << "; " << result.numberVariety[i] << endl;
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    varietyFile.close();

    fstream fitnessFile;
    fstream appearenceFile;
    fitnessFile.open("test/plot/" + timestr + "_histogramFitness.csv",ios::out);
    appearenceFile.open("test/plot/" + timestr + "_histogramAppearence.csv",ios::out);
    metrics::printParameters(fitnessFile, parameter);
    metrics::printParameters(appearenceFile, parameter);
    fitnessFile << "appearence; frequency" << endl;
    appearenceFile << "appearence; frequency" << endl;
    for(int i = 0; i < round(1 / 0.05); ++i){
      fitnessFile << i*0.05 + 0.025 << "; " << result.fitnessFrequency[i] << endl;
      appearenceFile << i*0.05 + 0.025 << "; " << result.appearenceFrequency[i] << endl;
    }
    fitnessFile.close();
    appearenceFile.close();
  }


  void Run_varParam(char param, std::vector<float> paramList){
    Parameter parameter;

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    fstream varietyFile;
    varietyFile.open("test/" + timestr + "_varParam_" + param + ".csv",ios::out);
    metrics::printParameters(varietyFile, parameter);
    varietyFile << "time; nVar; param" << endl;

    fstream fitnessFile;
    fstream appearenceFile;
    fitnessFile.open("test/" + timestr + "_histogramFitnessVar_" + param + ".csv",ios::out);
    appearenceFile.open(std::string ("test/" + timestr + "_histogramAppearenceVar_") + param + ".csv",ios::out);
    metrics::printParameters(fitnessFile, parameter);
    metrics::printParameters(appearenceFile, parameter);
    fitnessFile << "appearence; frequency; param" << endl;
    appearenceFile << "appearence; frequency; param" << endl;

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
          parameter.numberHabitat = paramValue;
          break;
        case 'n':
          parameter.numberInitialVariety = paramValue;
          break;
        case 'a':
          parameter.alpha = paramValue;
          break;
      }
      clock_t tStart = clock();
      Result result(parameter.maxTime/parameter.timeInterval, round(1 / 0.05));
      Result resultTemp(0,0);
      for(int run = 0; run < parameter.nRun; ++run){
        Model model(parameter);
        resultTemp = model.runStandard();
        std::transform(result.numberVariety.begin(), result.numberVariety.end(), resultTemp.numberVariety.begin(), result.numberVariety.begin(), std::plus<float>());
        std::transform(result.fitnessFrequency.begin(), result.fitnessFrequency.end(), resultTemp.fitnessFrequency.begin(), result.fitnessFrequency.begin(), std::plus<float>());
        std::transform(result.appearenceFrequency.begin(), result.appearenceFrequency.end(), resultTemp.appearenceFrequency.begin(), result.appearenceFrequency.begin(), std::plus<float>());
      }

      for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
        varietyFile << i*parameter.timeInterval << "; " << (float) result.numberVariety[i] / parameter.nRun << "; " << paramValue << endl;

      for(int i = 0; i < round(1 / 0.05); ++i){
        fitnessFile << i*0.05 + 0.025 << "; " << result.fitnessFrequency[i] / parameter.nRun << "; " << paramValue << endl;
        appearenceFile << i*0.05 + 0.025 << "; " << result.appearenceFrequency[i] / parameter.nRun << "; " << paramValue << endl;
      }

      cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    }
    varietyFile.close();
    fitnessFile.close();
    appearenceFile.close();
  }

  void Run_varParamFixedPoints(char param){
    Parameter parameter;

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    fstream varietyFile;
    varietyFile.open("test/" + timestr + "_varParamFixedPoints_" + param + ".csv",ios::out);
    metrics::printParameters(varietyFile, parameter);
    varietyFile << "param; nVar" << endl;

    fstream fitnessFile;
    fstream appearenceFile;
    fitnessFile.open("test/" + timestr + "_histogramFitnessVarFixedPoints_" + param + ".csv",ios::out);
    appearenceFile.open("test/" + timestr + "_histogramAppearenceVarFixedPoints_" + param + ".csv",ios::out);
    metrics::printParameters(fitnessFile, parameter);
    metrics::printParameters(appearenceFile, parameter);
    fitnessFile << "appearence; frequency; param" << endl;
    appearenceFile << "appearence; frequency; param" << endl;

    std::vector<float> paramList;
    switch (param){
      case 'o':
        paramList = std::vector<float> {0.001, 0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.075, 0.1, 0.25, 0.5};
        break;
      case 'i':
        paramList = std::vector<float> {0.0001, 0.0005, 0.001, 0.0015, 0.002, 0.0025, 0.003, 0.0035, 0.004, 0.0045, 0.005, 0.0075, 0.01};
        break;
      case 'L':
        paramList = std::vector<float> {25, 36, 49, 64};
        break;
      case 'r':
        paramList = std::vector<float> {1, 7, 49, 2401};
        break;
      case 'n':
        paramList = std::vector<float> {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
        break;
      case 'a':
        paramList = std::vector<float> {0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0};
        break;
    }

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
          parameter.numberHabitat = paramValue;
          break;
        case 'n':
          parameter.numberInitialVariety = paramValue;
          break;
        case 'a':
          parameter.alpha = paramValue;
          break;
      }
      clock_t tStart = clock();
      Result result(1, round(1 / 0.05));
      Result resultTemp(0,0);
      result.numberVariety[0];
      resultTemp.numberVariety.push_back(0);

      for(int run = 0; run < parameter.nRun; ++run){
        Model model(parameter);
        resultTemp = model.runFixedPoint();
        result.numberVariety[0] += resultTemp.numberVariety[0];
        std::transform(result.fitnessFrequency.begin(), result.fitnessFrequency.end(), resultTemp.fitnessFrequency.begin(), result.fitnessFrequency.begin(), std::plus<float>());
        std::transform(result.appearenceFrequency.begin(), result.appearenceFrequency.end(), resultTemp.appearenceFrequency.begin(), result.appearenceFrequency.begin(), std::plus<float>());
      }

      varietyFile << paramValue << "; " << (float) result.numberVariety[0] / parameter.nRun << endl;
      for(int i = 0; i < round(1 / 0.05); ++i){
        fitnessFile << i*0.05 + 0.025 << "; " << result.fitnessFrequency[i] / parameter.nRun << "; " << paramValue << endl;
        appearenceFile << i*0.05 + 0.025 << "; " << result.appearenceFrequency[i] / parameter.nRun << "; " << paramValue << endl;
      }

      cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    }


    varietyFile.close();
  }

}
