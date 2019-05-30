namespace worker{
  void Run_standard(void){
    Parameter parameter;
    Result result(parameter.maxTime/parameter.timeInterval, round(1 / 0.05), parameter.latticeSize);
    Result resultTemp(0, 0, 0);

    clock_t tStart = clock();
    for(int run = 0; run < parameter.nRun; ++run){
      Model model(parameter);
      resultTemp = model.runStandard();
      metrics::sumResults(&result, &resultTemp);
    }
    time_t now = time(NULL);
    std::string timestr = to_string(now);

    fstream varietyFile = worker::openFile("test/" + timestr + "_standard.csv", "time; nVar; meanDU", parameter);
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
      varietyFile << i*parameter.timeInterval << "; " << (float) result.numberVariety[i] / parameter.nRun << "; " << (float) result.meanVarietyDU[i] / parameter.nRun << endl;
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    varietyFile.close();

    fstream punctuationFile = worker::openFile("test/" + timestr + "_punctuation.csv", "time; totalPunctuation; fitnessPunctuation; appearencePunctuation", parameter);
    for(int i = 1; i < parameter.maxTime/parameter.timeInterval - 1; ++i)
      punctuationFile << i*parameter.timeInterval << "; " << result.totalPunctuation[i] / parameter.nRun << "; " << result.fitnessPunctuation[i] / parameter.nRun << "; " << (result.totalPunctuation[i]-result.fitnessPunctuation[i]) / parameter.nRun << endl;
    punctuationFile.close();

    fstream histogramFile = worker::openFile("test/" + timestr + "_histogramFitness.csv", "value; fitness; appearence", parameter);
    fstream duDistFile = worker::openFile("test/" + timestr + "_duDistribution.csv",  "value; duDist", parameter);
    fstream varietyDistFile = worker::openFile("test/" + timestr + "_varietyDistribution.csv",  "value; varDist", parameter);
    for(int i = 0; i < round(1 / 0.05); ++i)
      histogramFile << i*0.05 + 0.025 << "; " << result.fitnessFrequency[i] / parameter.nRun << "; " << result.appearenceFrequency[i] / parameter.nRun << endl;
    for(int i = 0; i < parameter.numberDomesticUnity; ++i){
      duDistFile << i+1 << "; " << result.duDistribution[i] / parameter.nRun << endl;
      varietyDistFile << (i+1.0) / parameter.numberDomesticUnity << "; " << result.varietyDistribution[i] / parameter.nRun << endl;
    }

    histogramFile.close();
    duDistFile.close();
    varietyDistFile.close();
  }

  void Run_plot(void){
    Parameter parameter;
    Model model(parameter);
    Result result(0, 0, 0);
    clock_t tStart = clock();
    result = model.runPlot();

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    fstream varietyFile = worker::openFile("test/plot/" + timestr + "_standard.csv", "time; nVar; meanDU", parameter);
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
      varietyFile << i*parameter.timeInterval << "; " << result.numberVariety[i] << "; " << result.meanVarietyDU[i] << endl;
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    varietyFile.close();

    fstream punctuationFile = worker::openFile("test/plot/" + timestr + "_punctuation.csv", "time; totalPunctuation; fitnessPunctuation; appearencePunctuation", parameter);
    for(int i = 1; i < parameter.maxTime/parameter.timeInterval - 1; ++i)
      punctuationFile << i*parameter.timeInterval << "; " << result.totalPunctuation[i] << "; " << result.fitnessPunctuation[i] << "; " << result.totalPunctuation[i]-result.fitnessPunctuation[i] << endl;
    punctuationFile.close();

    fstream histogramFile = worker::openFile("test/plot/" + timestr + "_histogramFitness.csv", "value; fitness; appearence", parameter);
    fstream duDistFile = worker::openFile("test/plot/" + timestr + "_duDistribution.csv",  "value; duDist", parameter);
    fstream varietyDistFile = worker::openFile("test/plot/" + timestr + "_varietyDistribution.csv",  "value; varDist", parameter);
    for(int i = 0; i < round(1 / 0.05); ++i)
      histogramFile << i*0.05 + 0.025 << "; " << result.fitnessFrequency[i] << "; " << result.appearenceFrequency[i] << endl;
    for(int i = 0; i < parameter.numberDomesticUnity; ++i){
      duDistFile << i+1 << "; " << result.duDistribution[i] << endl;
      varietyDistFile << (i+1.0) / parameter.numberDomesticUnity << "; " << result.varietyDistribution[i] << endl;
    }
    histogramFile.close();
    duDistFile.close();
    varietyDistFile.close();
  }


  void Run_varParam(char param, std::vector<float> paramList){
    Parameter parameter;
    if(param == 'm')
      parameter.nRun = 1;

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    fstream varietyFile = worker::openFile("test/" + timestr + "_varParam_" + param + ".csv", "time; nVar; meanDU; param", parameter);
    fstream histogramFile = worker::openFile("test/" + timestr + "_histogramFitnessVar_" + param + ".csv", "value; fitness; appearence; param", parameter);
    fstream duDistFile = worker::openFile("test/" + timestr + "_duDistribution_" + param + ".csv",  "value; duDist; param", parameter);
    fstream varietyDistFile = worker::openFile("test/" + timestr + "_varietyDistribution_" + param + ".csv", "value; varDist; param", parameter);

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
        case 'v':
          parameter.numberInitialVariety = paramValue;
          break;
        case 'a':
          parameter.alpha = paramValue;
          break;
        case 'n':
          parameter.networkType = paramValue;
          break;
        case 'p':
          parameter.probabilityNewVar = paramValue;
          break;
        case 'b':
          parameter.betaWT = paramValue;
          break;
        case 'R':
          parameter.numberResources = paramValue;
          break;
        case 'd':
          parameter.probabilityDeath = paramValue;
      }
      clock_t tStart = clock();
      Result result(parameter.maxTime/parameter.timeInterval, round(1 / 0.05), parameter.latticeSize);
      Result resultTemp(0, 0, 0);
      for(int run = 0; run < parameter.nRun; ++run){
        Model model(parameter);
        resultTemp = model.runStandard();
        metrics::sumResults(&result, &resultTemp);
      }

      for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
        varietyFile << i*parameter.timeInterval << "; " << (float) result.numberVariety[i] / parameter.nRun << "; " << (float) result.meanVarietyDU[i] / parameter.nRun<< "; " << paramValue << endl;
      for(int i = 0; i < round(1 / 0.05); ++i)
        histogramFile << i*0.05 + 0.025 << "; " << result.fitnessFrequency[i] / parameter.nRun << "; " << result.appearenceFrequency[i] / parameter.nRun << "; " << paramValue << endl;
      for(int i = 0; i < parameter.numberDomesticUnity; ++i){
        duDistFile << i+1 << "; " << result.duDistribution[i] / parameter.nRun << "; " << paramValue << endl;
        varietyDistFile << (i+1.0) / parameter.numberDomesticUnity << "; " << result.varietyDistribution[i] / parameter.nRun << "; " << paramValue << endl;
      }

      cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    }
    varietyFile.close();
    histogramFile.close();
    duDistFile.close();
    varietyDistFile.close();
  }

  void Run_varParamFixedPoints(char param){
    Parameter parameter;

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    fstream varietyFile = worker::openFile("test/" + timestr + "_varParamFixedPoints_" + param + ".csv", "param; nVar; meanDU", parameter);
    fstream histogramFile = worker::openFile("test/" + timestr + "_histogramFitnessVar_" + param + ".csv", "value; fitness; appearence; param", parameter);
    fstream duDistFile = worker::openFile("test/" + timestr + "_duDistribution_" + param + ".csv",  "value; duDist; param", parameter);
    fstream varietyDistFile = worker::openFile("test/" + timestr + "_varietyDistribution_" + param + ".csv", "value; varDist; param", parameter);

    std::vector<float> paramList;
    switch (param){
      case 'o':
        paramList = std::vector<float> {0.005, 0.0075, 0.01, 0.02, 0.03, 0.04, 0.05, 0.075, 0.1};
        break;
      case 'i':
        paramList = std::vector<float> {0.005, 0.0075, 0.01, 0.02, 0.03, 0.04, 0.05, 0.075, 0.1};
        break;
      case 'L':
        paramList = std::vector<float> {25, 36, 49, 64};
        break;
      case 'r':
        paramList = std::vector<float> {1, 2, 4, 8, 15, 25, 35, 49, 1000, 2401};
        break;
      case 'v':
        paramList = std::vector<float> {5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100};
        break;
      case 'a':
        paramList = std::vector<float> {0.0, 0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.75, 0.8, 0.85, 0.9, 0.95, 1.0};
        break;
      case 'n':
        paramList = std::vector<float> {0, 1, 2};
        break;
      case 'p':
        paramList = std::vector<float> {0.000025, 0.00005, 0.0001, 0.0002, 0.0005, 0.001, 0.002, 0.005, 0.01};
        break;
      case 'b':
        paramList = std::vector<float> {0.05, 0.1, 0.15, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.6, 0.7, 0.8, 0.9};
        break;
      case 'R':
        paramList = std::vector<float> {1, 3, 5, 7, 10};
        break;
      case 'd':
        paramList = std::vector<float> {0.001, 0.005, 0.01, 0.025, 0.05};
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
        case 'v':
          parameter.numberInitialVariety = paramValue;
          break;
        case 'a':
          parameter.alpha = paramValue;
          break;
        case 'n':
          parameter.networkType = paramValue;
          break;
        case 'p':
          parameter.probabilityNewVar = paramValue;
          break;
        case 'b':
          parameter.betaWT = paramValue;
          break;
        case 'R':
          parameter.numberResources = paramValue;
          break;
        case 'd':
          parameter.probabilityDeath = paramValue;
      }
      clock_t tStart = clock();
      Result result(1, round(1 / 0.05), parameter.latticeSize);
      Result resultTemp(0, 0, 0);
      result.numberVariety[0];
      resultTemp.numberVariety.push_back(0);

      for(int run = 0; run < parameter.nRun; ++run){
        Model model(parameter);
        resultTemp = model.runFixedPoint();
        metrics::sumResults(&result, &resultTemp);
      }

      varietyFile << paramValue << "; " << (float) result.numberVariety[0] / parameter.nRun << "; " << (float) result.meanVarietyDU[0] / parameter.nRun << endl;
      for(int i = 0; i < round(1 / 0.05); ++i)
        histogramFile << i*0.05 + 0.025 << "; " << result.fitnessFrequency[i] / parameter.nRun << "; " << result.appearenceFrequency[i] / parameter.nRun << "; " << paramValue << endl;
      for(int i = 0; i < parameter.numberDomesticUnity; ++i){
        duDistFile << i+1 << "; " << result.duDistribution[i] / parameter.nRun << "; " << paramValue << endl;
        varietyDistFile << (i+1.0) / parameter.numberDomesticUnity << "; " << result.varietyDistribution[i] / parameter.nRun << "; " << paramValue << endl;
      }

      cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    }

    varietyFile.close();
    histogramFile.close();
    duDistFile.close();
    varietyDistFile.close();
  }

  fstream openFile(std::string fileName, std::string varNames, Parameter parameter){
    fstream file;
    file.open(fileName,ios::out);
    metrics::printParameters(file, parameter);
    file << varNames << endl;
    return file;
  }

}
