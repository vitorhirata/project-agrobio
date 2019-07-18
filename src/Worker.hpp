namespace worker{
  void Run_standard(void){
    Parameter parameter;
    Result result(parameter.maxTime/parameter.timeInterval,
        round(1 / 0.05), parameter.latticeSize, 17);
    Result resultTemp(0, 0, 0, 0);

    clock_t tStart = clock();
    for(int run = 0; run < parameter.nRun; ++run){
      Model model(parameter);
      resultTemp = model.runStandard();
      metrics::sumResults(&result, &resultTemp);
    }
    time_t now = time(NULL);
    std::string timestr = to_string(now);

    int duSize = parameter.latticeSize / sqrt(parameter.numberDomesticUnity);
    duSize = duSize * duSize;
    std::string header ("time; nVar; meanDU; totalPunctuation; ");
    header.append("fitnessPunctuation; appearencePunctuation; bergerDU; ");
    header.append("simpsonCommunity; shannonCommunity; ");
    header.append("simpsonDU; shannonDU");
    fstream timeFile = worker::openFile(
        "test/" + timestr + "_standard.csv", header, parameter);
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i){
      timeFile << i*parameter.timeInterval << "; ";
      timeFile << (float) result.numberVariety[i] / parameter.nRun << "; ";
      timeFile << (float) result.meanVarietyDU[i] / parameter.nRun << "; ";
      timeFile << result.totalPunctuation[i] / parameter.nRun << "; ";
      timeFile << result.fitnessPunctuation[i] / parameter.nRun << "; ";
      timeFile << (result.totalPunctuation[i]-
          result.fitnessPunctuation[i]) / parameter.nRun << "; ";
      timeFile << result.bergerParkerDU[i] /
        (duSize * parameter.nRun) << "; ";
      timeFile << result.simpsonCommunity[i] / parameter.nRun << "; ";
      timeFile << result.shannonCommunity[i] / parameter.nRun << "; ";
      timeFile << result.simpsonDU[i] / parameter.nRun << "; ";
      timeFile << result.shannonDU[i] / parameter.nRun << endl;
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    timeFile.close();

    fstream histogramFile = worker::openFile(
        "test/" + timestr +
        "_histogramFitness.csv", "value; fitness; appearence", parameter);
    fstream duDistFile = worker::openFile(
        "test/" + timestr + "_duDistribution.csv",
        "value; duDist", parameter);
    fstream varietyDistFile = worker::openFile(
        "test/" + timestr + "_varietyDistribution.csv",  "value; varDist",
        parameter);
    fstream varietyQuantFile = worker::openFile(
        "test/" + timestr + "_varietyQuantity.csv",
        "quantity; frequency", parameter);
    for(int i = 0; i < round(1 / 0.05); ++i){
      histogramFile << i*0.05 + 0.025 << "; ";
      histogramFile << 100*result.fitnessFrequency[i] / parameter.nRun << "; ";
      histogramFile << 100*result.appearenceFrequency[i] / parameter.nRun;
      histogramFile << endl;
    }

    for(int i = 0; i <= duSize; ++i){
      duDistFile << i << "; " << 100*result.duDistribution[i] / parameter.nRun;
      duDistFile << endl;
    }
    for(int i = 0; i < parameter.numberDomesticUnity; ++i){
      varietyDistFile << i + 1 << "; ";
      varietyDistFile << 100 * result.varietyDistribution[i] / parameter.nRun;
      varietyDistFile << endl;
    }
    for(int i = 0; i < result.varietyQuantity.size(); ++i){
      varietyQuantFile << 100*(pow(10,i * 0.2)) / pow(parameter.latticeSize,2);
      varietyQuantFile << "; ";
      varietyQuantFile << 100*result.varietyQuantity[i]/parameter.nRun<< endl;
    }

    histogramFile.close();
    duDistFile.close();
    varietyDistFile.close();
    varietyQuantFile.close();
  }

  void Run_plot(void){
    Parameter parameter;
    Model model(parameter);
    Result result(0, 0, 0, 0);
    clock_t tStart = clock();
    result = model.runPlot();

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    int duSize = parameter.latticeSize / sqrt(parameter.numberDomesticUnity);
    duSize = duSize * duSize;
    std::string header ("time; nVar; meanDU; totalPunctuation; ");
    header.append("fitnessPunctuation; appearencePunctuation; bergerDU; ");
    header.append("simpson; shannon");
    fstream timeFile = worker::openFile(
        "test/plot/" + timestr + "_standard.csv", header,
        parameter);
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i){
      timeFile << i*parameter.timeInterval << "; ";
      timeFile << result.numberVariety[i] << "; ";
      timeFile << result.meanVarietyDU[i] << "; ";
      timeFile << result.totalPunctuation[i] << "; ";
      timeFile << result.fitnessPunctuation[i] << "; ";
      timeFile << result.totalPunctuation[i] - result.fitnessPunctuation[i];
      timeFile << "; " << result.bergerParkerDU[i] / duSize << "; ";
      timeFile << result.simpsonCommunity[i] << "; ";
      timeFile << result.shannonCommunity[i] << "; ";
      timeFile << result.simpsonDU[i] << "; ";
      timeFile << result.shannonDU[i] << endl;
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    timeFile.close();

    fstream histogramFile = worker::openFile(
        "test/plot/" + timestr + "_histogramFitness.csv",
        "value; fitness; appearence", parameter);
    fstream duDistFile = worker::openFile(
        "test/plot/" + timestr + "_duDistribution.csv",  "value; duDist",
        parameter);
    fstream varietyDistFile = worker::openFile(
        "test/plot/" + timestr + "_varietyDistribution.csv",
        "value; varDist", parameter);
    fstream varietyQuantFile = worker::openFile(
        "test/plot/" + timestr + "_varietyQuantity.csv",
        "quantity; frequency", parameter);
    for(int i = 0; i < round(1 / 0.05); ++i){
      histogramFile << i*0.05+0.025 << "; " << 100*result.fitnessFrequency[i];
      histogramFile << "; " << 100 * result.appearenceFrequency[i] << endl;
    }
    for(int i = 0; i <= duSize; ++i)
      duDistFile << i << "; " << 100 * result.duDistribution[i] << endl;
    for(int i = 0; i < parameter.numberDomesticUnity; ++i){
      varietyDistFile << i+1 << "; ";
      varietyDistFile << 100 * result.varietyDistribution[i] << endl;
    }
    for(int i = 0; i < result.varietyQuantity.size(); ++i){
      varietyQuantFile << 100*(pow(10,i * 0.2)) / pow(parameter.latticeSize,2);
      varietyQuantFile << "; " << 100 * result.varietyQuantity[i] << endl;
    }

    histogramFile.close();
    duDistFile.close();
    varietyDistFile.close();
    varietyQuantFile.close();
  }


  void Run_varParam(char param, std::vector<float> paramList){
    Parameter parameter;
    if(param == 'm')
      parameter.nRun = 1;

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    std::string header ("time; nVar; meanDU; totalPunctuation; ");
    header.append("fitnessPunctuation; appearencePunctuation; ");
    header.append("bergerDU; simpson; shannon; param");
    fstream timeFile = worker::openFile(
        "test/" + timestr + "_varParam_" + param + ".csv",
        header, parameter);
    fstream histogramFile = worker::openFile(
        "test/" + timestr + "_histogramFitnessVar_" + param + ".csv",
        "value; fitness; appearence; param", parameter);
    fstream duDistFile = worker::openFile(
        "test/" + timestr + "_duDistribution_" + param + ".csv",
        "value; duDist; param", parameter);
    fstream varietyDistFile = worker::openFile(
        "test/" + timestr + "_varietyDistribution_" + param + ".csv",
        "value; varDist; param", parameter);
    fstream varietyQuantFile = worker::openFile(
        "test/" + timestr + "_varietyQuantity_" + param + ".csv",
        "quantity; frequency; param", parameter);

    for(auto paramValue : paramList){
      switch (param){
        case 'o':
          parameter.outsideTradeLimit = paramValue;
          break;
        case 'i':
          parameter.insideTradeLimit = paramValue;
          break;
        case 'L':
          if(round(sqrt(paramValue)) * round(sqrt(paramValue)) != paramValue){
            cout << "ERROR: the latticeSize should be a perfect square.";
            cout << endl;
            exit(-1);
          }
          parameter.latticeSize = paramValue;
          parameter.numberDomesticUnity = paramValue;
          break;
        case 'r':
          parameter.numberHabitat = paramValue;
          break;
        case 'v':
          if(paramValue > parameter.numberInitialVariety){
            cout << "ERROR: the number of initial variety per DU should be ";
            cout << "smaller than total number of initial variety." << endl;
            exit(-1);
          }
          parameter.numberInitialVarietyDU = paramValue;
          break;
        case 'V':
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
        case 's':
          parameter.selectionStrength = paramValue;
          break;
        case 'd':
          parameter.deathStrength = paramValue;
      }
      clock_t tStart = clock();
      Result result(parameter.maxTime/parameter.timeInterval,
          round(1 / 0.05), parameter.latticeSize, 17);
      Result resultTemp(0, 0, 0, 0);
      for(int run = 0; run < parameter.nRun; ++run){
        Model model(parameter);
        resultTemp = model.runStandard();
        metrics::sumResults(&result, &resultTemp);
      }

      int duSize = parameter.latticeSize / sqrt(parameter.numberDomesticUnity);
      duSize = duSize * duSize;
      for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i){
        timeFile << i * parameter.timeInterval << "; ";
        timeFile << (float) result.numberVariety[i] / parameter.nRun << "; ";
        timeFile << (float) result.meanVarietyDU[i] / parameter.nRun<< "; ";
        timeFile << result.totalPunctuation[i] / parameter.nRun << "; ";
        timeFile << result.fitnessPunctuation[i] / parameter.nRun << "; ";
        timeFile << (result.totalPunctuation[i]-
          result.fitnessPunctuation[i]) / parameter.nRun << "; ";
        timeFile << result.bergerParkerDU[i] /
          (duSize * parameter.nRun) << "; ";
        timeFile << result.simpsonCommunity[i] / parameter.nRun << "; ";
        timeFile << result.shannonCommunity[i] / parameter.nRun << "; ";
        timeFile << result.simpsonDU[i] / parameter.nRun << "; ";
        timeFile << result.shannonDU[i] / parameter.nRun << "; ";
        timeFile << paramValue << endl;
      }
      for(int i = 0; i < round(1 / 0.05); ++i){
        histogramFile << i*0.05 + 0.025 << "; ";
        histogramFile << 100 * result.fitnessFrequency[i] / parameter.nRun;
        histogramFile << "; ";
        histogramFile << 100 * result.appearenceFrequency[i] / parameter.nRun;
        histogramFile << "; " << paramValue << endl;
      }
      for(int i = 0; i <= duSize; ++i){
        duDistFile << i << "; ";
        duDistFile << 100 * result.duDistribution[i] / parameter.nRun << "; ";
        duDistFile << paramValue << endl;
      }
      for(int i = 0; i < parameter.numberDomesticUnity; ++i){
        varietyDistFile << i+1 << "; ";
        varietyDistFile << 100*result.varietyDistribution[i] / parameter.nRun;
        varietyDistFile << "; " << paramValue << endl;
      }
      for(int i = 0; i < result.varietyQuantity.size(); ++i){
        varietyQuantFile << 100*(pow(10,i*0.2)) / pow(parameter.latticeSize,2);
        varietyQuantFile << "; ";
        varietyQuantFile << 100 * result.varietyQuantity[i] / parameter.nRun;
        varietyQuantFile << "; " << paramValue << endl;
      }

      cout << "Time taken: "<< (double)(clock()-tStart)/CLOCKS_PER_SEC << endl;
    }
    timeFile.close();
    histogramFile.close();
    duDistFile.close();
    varietyDistFile.close();
    varietyQuantFile.close();
  }

  void Run_varParamFixedPoints(char param){
    Parameter parameter;

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    std::string header ("param; nVar; meanDU; totalPunctuation; ");
    header.append("fitnessPunctuation; appearencePunctuation; bergerDU; ");
    header.append("simpson; shannon");
    fstream mainFile = worker::openFile(
        "test/" + timestr + "_varParamFixedPoints_" + param + ".csv",
        header, parameter);
    fstream histogramFile = worker::openFile(
        "test/" + timestr + "_histogramFitnessVar_" + param + ".csv",
        "value; fitness; appearence; param", parameter);
    fstream duDistFile = worker::openFile(
        "test/" + timestr + "_duDistribution_" + param + ".csv",
        "value; duDist; param", parameter);
    fstream varietyDistFile = worker::openFile(
        "test/" + timestr + "_varietyDistribution_" + param + ".csv",
        "value; varDist; param", parameter);
    fstream varietyQuantFile = worker::openFile(
        "test/" + timestr + "_varietyQuantity_" + param + ".csv",
        "quantity; frequency; param", parameter);

    std::vector<float> paramList;
    switch (param){
      case 'o':
        paramList = std::vector<float> {
          0, 0.005, 0.0075, 0.01, 0.02, 0.035, 0.05, 0.075, 0.1, 1};
        break;
      case 'i':
        paramList = std::vector<float> {
          0, 0.005, 0.0075, 0.01, 0.02, 0.05, 0.075, 0.1, 1};
        break;
      case 'L':
        paramList = std::vector<float> {25, 36, 49, 64};
        break;
      case 'r':
        paramList = std::vector<float> {
          1, 2, 4, 8, 15, 25, 35, 49, 1000, 2401};
        break;
      case 'v':
        paramList = std::vector<float> {
          1, 2, 3, 4, 5, 10};
        break;
      case 'V':
        paramList = std::vector<float> {
          10, 15, 20, 30, 40, 50, 60, 70, 80, 90, 100};
        break;
      case 'a':
        paramList = std::vector<float> {
          0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
        break;
      case 'n':
        paramList = std::vector<float> {0, 1, 2};
        break;
      case 'p':
        paramList = std::vector<float> {
          0, 0.0001, 0.0005, 0.001, 0.0025, 0.005, 0.01};
        break;
      case 'b':
        paramList = std::vector<float> {
          0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
        break;
      case 'R':
        paramList = std::vector<float> {1, 3, 5, 7, 10};
        break;
      case 's':
        paramList = std::vector<float> {1, 3, 5, 10, 15, 20, 30, 50, 100};
        break;
      case 'd':
        paramList = std::vector<float> {5, 10, 18, 30, 50, 75, 100};
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
            cout << "ERROR: the latticeSize should be a perfect square.";
            cout << endl;
            exit(-1);
          }
          parameter.latticeSize = paramValue;
          parameter.numberDomesticUnity = paramValue;
          break;
        case 'r':
          parameter.numberHabitat = paramValue;
          break;
        case 'v':
          if(paramValue > parameter.numberInitialVariety){
            cout << "ERROR: the number of initial variety per DU should be ";
            cout << "smaller than total number of initial variety." << endl;
            exit(-1);
          }
          parameter.numberInitialVarietyDU = paramValue;
          break;
        case 'V':
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
        case 's':
          parameter.selectionStrength = paramValue;
          break;
        case 'd':
          parameter.deathStrength = paramValue;
      }
      clock_t tStart = clock();
      Result result(1, round(1 / 0.05), parameter.latticeSize, 17);
      Result resultTemp(0, 0, 0, 0);
      result.numberVariety[0];
      resultTemp.numberVariety.push_back(0);

      for(int run = 0; run < parameter.nRun; ++run){
        Model model(parameter);
        resultTemp = model.runFixedPoint();
        metrics::sumResults(&result, &resultTemp);
      }

      int duSize = parameter.latticeSize / sqrt(parameter.numberDomesticUnity);
      duSize = duSize * duSize;
      mainFile << paramValue << "; ";
      mainFile << (float) result.numberVariety[0] / parameter.nRun << "; ";
      mainFile << (float) result.meanVarietyDU[0] / parameter.nRun << "; ";
      mainFile << result.totalPunctuation[0] / parameter.nRun << "; ";
      mainFile << result.fitnessPunctuation[0] / parameter.nRun << "; ";
      mainFile << (result.totalPunctuation[0]-
            result.fitnessPunctuation[0]) / parameter.nRun << "; ";
      mainFile << result.bergerParkerDU[0] /
        (duSize * parameter.nRun) << "; ";
      mainFile << result.simpsonCommunity[0] / parameter.nRun << "; ";
      mainFile << result.shannonCommunity[0] / parameter.nRun << "; ";
      mainFile << result.simpsonDU[0] / parameter.nRun << "; ";
      mainFile << result.shannonDU[0] / parameter.nRun << endl;
      for(int i = 0; i < round(1 / 0.05); ++i){
        histogramFile << i*0.05 + 0.025 << "; ";
        histogramFile << 100 * result.fitnessFrequency[i] / parameter.nRun;
        histogramFile << "; ";
        histogramFile << 100 * result.appearenceFrequency[i] / parameter.nRun;
        histogramFile << "; " << paramValue << endl;
      }
      for(int i = 0; i <= duSize; ++i){
        duDistFile << i << "; ";
        duDistFile << 100 * result.duDistribution[i] / parameter.nRun << "; ";
        duDistFile << paramValue << endl;
      }
      for(int i = 0; i < parameter.numberDomesticUnity; ++i){
        varietyDistFile << i+1 << "; ";
        varietyDistFile << 100*result.varietyDistribution[i] / parameter.nRun;
        varietyDistFile << "; " << paramValue << endl;
      }
      for(int i = 0; i < result.varietyQuantity.size(); ++i){
        varietyQuantFile << 100*(pow(10,i*0.2)) / pow(parameter.latticeSize,2);
        varietyQuantFile << "; ";
        varietyQuantFile << 100 * result.varietyQuantity[i] / parameter.nRun;
        varietyQuantFile << "; " << paramValue << endl;
      }

      cout << "Time taken: "<< (double)(clock()-tStart)/CLOCKS_PER_SEC << endl;
    }

    mainFile.close();
    histogramFile.close();
    duDistFile.close();
    varietyDistFile.close();
    varietyQuantFile.close();
  }

  fstream openFile(
      std::string fileName, std::string varNames, Parameter parameter){
    fstream file;
    file.open(fileName,ios::out);
    metrics::printParameters(file, parameter);
    file << varNames << endl;
    return file;
  }

}
