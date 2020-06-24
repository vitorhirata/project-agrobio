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

    int hdSize = parameter.latticeSize / sqrt(parameter.numberHousehold);
    hdSize = hdSize * hdSize;
    std::string header ("time; nVar; meanHD; totalPunctuation; ");
    header.append("productivityPunctuation; qualityPunctuation; ");
    header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
    header.append("bergerHD; simpsonHD; shannonHD");
    fstream timeFile = worker::openFile(
        "test/" + timestr + "_standard.csv", header, parameter);
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i){
      timeFile << i*parameter.timeInterval << "; ";
      timeFile << (float) result.numberVariety[i] / parameter.nRun << "; ";
      timeFile << (float) result.meanVarietyHD[i] / parameter.nRun << "; ";
      timeFile << result.totalPunctuation[i] / parameter.nRun << "; ";
      timeFile << result.productivityPunctuation[i] / parameter.nRun << "; ";
      timeFile << (result.totalPunctuation[i]-
          result.productivityPunctuation[i]) / parameter.nRun << "; ";
      timeFile << result.bergerParkerCommunity[i] / (hdSize * parameter.nRun);
      timeFile << "; " << result.simpsonCommunity[i] / parameter.nRun << "; ";
      timeFile << result.shannonCommunity[i] / parameter.nRun << "; ";
      timeFile << result.bergerParkerHD[i] / (hdSize * parameter.nRun) << "; ";
      timeFile << result.simpsonHD[i] / parameter.nRun << "; ";
      timeFile << result.shannonHD[i] / parameter.nRun << endl;
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    timeFile.close();

    fstream histogramFile = worker::openFile(
        "test/" + timestr +
        "_histogramProductivity.csv", "value; productivity; quality", parameter);
    fstream hdDistFile = worker::openFile(
        "test/" + timestr + "_hdDistribution.csv",
        "value; hdDist", parameter);
    fstream varietyDistFile = worker::openFile(
        "test/" + timestr + "_varietyDistribution.csv",  "value; varDist",
        parameter);
    fstream varietyQuantFile = worker::openFile(
        "test/" + timestr + "_varietyQuantity.csv",
        "quantity; frequency", parameter);
    for(int i = 0; i < round(1 / 0.05); ++i){
      histogramFile << i*0.05 + 0.025 << "; ";
      histogramFile << 100 * result.productivityFrequency[i] / parameter.nRun;
      histogramFile << "; ";
      histogramFile << 100 * result.qualityFrequency[i] / parameter.nRun;
      histogramFile << endl;
    }

    for(int i = 0; i <= hdSize - 1; ++i){
      hdDistFile << i << "; " << 100*result.hdDistribution[i] / parameter.nRun;
      hdDistFile << endl;
    }
    for(int i = 0; i < parameter.numberHousehold; ++i){
      varietyDistFile << 100.0*(i+1.0) / parameter.numberHousehold << "; ";
      varietyDistFile << 100 * result.varietyDistribution[i] / parameter.nRun;
      varietyDistFile << endl;
    }
    for(int i = 0; i < result.varietyQuantity.size(); ++i){
      varietyQuantFile << 100*(pow(10,i * 0.2)) / pow(parameter.latticeSize,2);
      varietyQuantFile << "; ";
      varietyQuantFile << 100*result.varietyQuantity[i]/parameter.nRun<< endl;
    }

    histogramFile.close();
    hdDistFile.close();
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

    int hdSize = parameter.latticeSize / sqrt(parameter.numberHousehold);
    hdSize = hdSize * hdSize;
    std::string header ("time; nVar; meanHD; totalPunctuation; ");
    header.append("productivityPunctuation; qualityPunctuation; ");
    header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
    header.append("bergerHD; simpsonHD; shannonHD");
    fstream timeFile = worker::openFile(
        "test/plot/" + timestr + "_standard.csv", header,
        parameter);
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i){
      timeFile << i*parameter.timeInterval << "; ";
      timeFile << result.numberVariety[i] << "; ";
      timeFile << result.meanVarietyHD[i] << "; ";
      timeFile << result.totalPunctuation[i] << "; ";
      timeFile << result.productivityPunctuation[i] << "; ";
      timeFile << result.totalPunctuation[i]-result.productivityPunctuation[i];
      timeFile << "; " << result.bergerParkerCommunity[i] / hdSize << "; ";
      timeFile << result.simpsonCommunity[i] << "; ";
      timeFile << result.shannonCommunity[i] << "; ";
      timeFile << result.bergerParkerHD[i] / hdSize << "; ";
      timeFile << result.simpsonHD[i] << "; ";
      timeFile << result.shannonHD[i] << endl;
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    timeFile.close();

    fstream histogramFile = worker::openFile(
        "test/plot/" + timestr + "_histogramProductivity.csv",
        "value; productivity; quality", parameter);
    fstream hdDistFile = worker::openFile(
        "test/plot/" + timestr + "_hdDistribution.csv",  "value; hdDist",
        parameter);
    fstream varietyDistFile = worker::openFile(
        "test/plot/" + timestr + "_varietyDistribution.csv",
        "value; varDist", parameter);
    fstream varietyQuantFile = worker::openFile(
        "test/plot/" + timestr + "_varietyQuantity.csv",
        "quantity; frequency", parameter);
    for(int i = 0; i < round(1 / 0.05); ++i){
      histogramFile << i*0.05+0.025 << "; ";
      histogramFile << 100*result.productivityFrequency[i] << "; ";
      histogramFile << 100 * result.qualityFrequency[i] << endl;
    }
    for(int i = 0; i <= hdSize; ++i)
      hdDistFile << i << "; " << 100 * result.hdDistribution[i] << endl;
    for(int i = 0; i < parameter.numberHousehold; ++i){
      varietyDistFile << 100 * (i+1.0) / parameter.numberHousehold << "; ";
      varietyDistFile << 100 * result.varietyDistribution[i] << endl;
    }
    for(int i = 0; i < result.varietyQuantity.size(); ++i){
      varietyQuantFile << 100*(pow(10,i * 0.2)) / pow(parameter.latticeSize,2);
      varietyQuantFile << "; " << 100 * result.varietyQuantity[i] << endl;
    }

    histogramFile.close();
    hdDistFile.close();
    varietyDistFile.close();
    varietyQuantFile.close();
  }


  void Run_varParam(char param, std::vector<float> paramList){
    Parameter parameter;
    if(param == 'm')
      parameter.nRun = 1;

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    std::string header ("time; nVar; meanHD; totalPunctuation; ");
    header.append("productivityPunctuation; qualityPunctuation; ");
    header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
    header.append("bergerHD; simpsonHD; shannonHD; param");
    fstream timeFile = worker::openFile(
        "test/" + timestr + "_varParam_" + param + ".csv",
        header, parameter);
    fstream histogramFile = worker::openFile(
        "test/" + timestr + "_histogramProductivityVar_" + param + ".csv",
        "value; productivity; quality; param", parameter);
    fstream hdDistFile = worker::openFile(
        "test/" + timestr + "_hdDistribution_" + param + ".csv",
        "value; hdDist; param", parameter);
    fstream varietyDistFile = worker::openFile(
        "test/" + timestr + "_varietyDistribution_" + param + ".csv",
        "value; varDist; param", parameter);
    fstream varietyQuantFile = worker::openFile(
        "test/" + timestr + "_varietyQuantity_" + param + ".csv",
        "quantity; frequency; param", parameter);

    for(auto paramValue : paramList){
      switch (param){
        case 'T':
          parameter.outsideTradeLimit = paramValue;
          break;
        case 'L':
          if(round(sqrt(paramValue)) * round(sqrt(paramValue)) != paramValue){
            cout << "ERROR: the latticeSize should be a perfect square.";
            cout << endl;
            exit(-1);
          }
          parameter.latticeSize = paramValue;
          parameter.numberHousehold = paramValue;
          break;
        case 'H':
          parameter.numberHabitat = paramValue;
          break;
        case 'v':
          if(paramValue > parameter.numberInitialVariety){
            cout << "ERROR: the number of initial variety per HD should be ";
            cout << "smaller than total number of initial variety." << endl;
            exit(-1);
          }
          parameter.numberInitialVarietyHD = paramValue;
          break;
        case 'V':
          parameter.numberInitialVariety = paramValue;
          break;
        case 'a':
          parameter.alpha = paramValue;
          break;
        case 'R':
          parameter.networkType = paramValue;
          break;
        case 'N':
          parameter.probabilityNewVar = paramValue;
          break;
        case 'b':
          parameter.betaWT = paramValue;
          break;
        case 'r':
          parameter.numberResources = paramValue;
          break;
        case 'S':
          parameter.selectionStrength = paramValue;
          break;
        case 'Q':
          parameter.percentageNewRandomVar = paramValue;
          break;
        case 'C':
          parameter.crossingDeviation = paramValue;
          break;
        case 'M':
          parameter.deathProbability = paramValue;
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

      int hdSize = parameter.latticeSize / sqrt(parameter.numberHousehold);
      hdSize = hdSize * hdSize;
      for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i){
        timeFile << i * parameter.timeInterval << "; ";
        timeFile << (float) result.numberVariety[i] / parameter.nRun << "; ";
        timeFile << (float) result.meanVarietyHD[i] / parameter.nRun<< "; ";
        timeFile << result.totalPunctuation[i] / parameter.nRun << "; ";
        timeFile << result.productivityPunctuation[i] / parameter.nRun << "; ";
        timeFile << (result.totalPunctuation[i]-
          result.productivityPunctuation[i]) / parameter.nRun << "; ";
        timeFile << result.bergerParkerCommunity[i] / (hdSize*parameter.nRun);
        timeFile << "; " << result.simpsonCommunity[i]/parameter.nRun << "; ";
        timeFile << result.shannonCommunity[i] / parameter.nRun << "; ";
        timeFile << result.bergerParkerHD[i] / (hdSize * parameter.nRun);
        timeFile << "; " << result.simpsonHD[i] / parameter.nRun << "; ";
        timeFile << result.shannonHD[i] / parameter.nRun << "; ";
        timeFile << paramValue << endl;
      }
      for(int i = 0; i < round(1 / 0.05); ++i){
        histogramFile << i*0.05 + 0.025 << "; ";
        histogramFile << 100*result.productivityFrequency[i] / parameter.nRun;
        histogramFile << "; ";
        histogramFile << 100 * result.qualityFrequency[i] / parameter.nRun;
        histogramFile << "; " << paramValue << endl;
      }
      for(int i = 0; i <= hdSize - 1; ++i){
        hdDistFile << i << "; ";
        hdDistFile << 100 * result.hdDistribution[i] / parameter.nRun << "; ";
        hdDistFile << paramValue << endl;
      }
      for(int i = 0; i < parameter.numberHousehold; ++i){
        varietyDistFile << 100.0*(i+1) / parameter.numberHousehold << "; ";
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
    hdDistFile.close();
    varietyDistFile.close();
    varietyQuantFile.close();
  }

  void Run_varParamFixedPoints(char param){
    Parameter parameter;

    time_t now = time(NULL);
    std::string timestr = to_string(now);

    std::string header ("param; nVar; meanHD; totalPunctuation; ");
    header.append("productivityPunctuation; qualityPunctuation; ");
    header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
    header.append("bergerHD; simpsonHD; shannonHD");
    fstream mainFile = worker::openFile(
        "test/" + timestr + "_varParamFixedPoints_" + param + ".csv",
        header, parameter);
    fstream histogramFile = worker::openFile(
        "test/" + timestr + "_histogramProductivityVar_" + param + ".csv",
        "value; productivity; quality; param", parameter);
    fstream hdDistFile = worker::openFile(
        "test/" + timestr + "_hdDistribution_" + param + ".csv",
        "value; hdDist; param", parameter);
    fstream varietyDistFile = worker::openFile(
        "test/" + timestr + "_varietyDistribution_" + param + ".csv",
        "value; varDist; param", parameter);
    fstream varietyQuantFile = worker::openFile(
        "test/" + timestr + "_varietyQuantity_" + param + ".csv",
        "quantity; frequency; param", parameter);

    std::vector<float> paramList;
    switch (param){
      case 'T':
        paramList = std::vector<float> {
          0, 0.05, 0.1, 0.15, 0.20, 0.225, 0.25, 0.275, 0.3, 0.35, 0.5, 0.8, 1.0};
        break;
      case 'L':
        paramList = std::vector<float> {25, 36, 49, 64};
        break;
      case 'H':
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
      case 'R':
        paramList = std::vector<float> {0, 1, 2};
        break;
      case 'N':
        paramList = std::vector<float> {
          0, 0.0001, 0.0005, 0.001, 0.003, 0.005, 0.007, 0.01, 0.05};
        break;
      case 'b':
        paramList = std::vector<float> {
          0, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};
        break;
      case 'r':
        paramList = std::vector<float> {1, 3, 5, 7, 10};
        break;
      case 'S':
        paramList = std::vector<float> {1, 3, 5, 10, 15, 20, 30, 50, 100};
        break;
      case 'Q':
        paramList = std::vector<float> {0.01, 0.25, 0.05, 0.75, 0.1, 0.2, 0.3};
        break;
      case 'C':
        paramList = std::vector<float> {0, 0.005, 0.01, 0.05, 0.08, 0.1};
        break;
      case 'M':
        paramList = std::vector<float> {0, 0.01, 0.05, 0.1, 0.16, 0.2, 0.3,
          0.4, 0.5};
    }

    for(auto paramValue : paramList){
      switch (param){
        case 'T':
          parameter.outsideTradeLimit = paramValue;
          break;
        case 'L':
          if (round(sqrt(paramValue)) * round(sqrt(paramValue)) != paramValue){
            cout << "ERROR: the latticeSize should be a perfect square.";
            cout << endl;
            exit(-1);
          }
          parameter.latticeSize = paramValue;
          parameter.numberHousehold = paramValue;
          break;
        case 'H':
          parameter.numberHabitat = paramValue;
          break;
        case 'v':
          if(paramValue > parameter.numberInitialVariety){
            cout << "ERROR: the number of initial variety per HD should be ";
            cout << "smaller than total number of initial variety." << endl;
            exit(-1);
          }
          parameter.numberInitialVarietyHD = paramValue;
          break;
        case 'V':
          parameter.numberInitialVariety = paramValue;
          break;
        case 'a':
          parameter.alpha = paramValue;
          break;
        case 'R':
          parameter.networkType = paramValue;
          break;
        case 'N':
          parameter.probabilityNewVar = paramValue;
          break;
        case 'b':
          parameter.betaWT = paramValue;
          break;
        case 'r':
          parameter.numberResources = paramValue;
          break;
        case 'S':
          parameter.selectionStrength = paramValue;
          break;
        case 'Q':
          parameter.percentageNewRandomVar = paramValue;
          break;
        case 'C':
          parameter.crossingDeviation = paramValue;
          break;
        case 'M':
          parameter.deathProbability = paramValue;
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

      int hdSize = parameter.latticeSize / sqrt(parameter.numberHousehold);
      hdSize = hdSize * hdSize;
      mainFile << paramValue << "; ";
      mainFile << (float) result.numberVariety[0] / parameter.nRun << "; ";
      mainFile << (float) result.meanVarietyHD[0] / parameter.nRun << "; ";
      mainFile << result.totalPunctuation[0] / parameter.nRun << "; ";
      mainFile << result.productivityPunctuation[0] / parameter.nRun << "; ";
      mainFile << (result.totalPunctuation[0]-
            result.productivityPunctuation[0]) / parameter.nRun << "; ";
      mainFile << result.bergerParkerCommunity[0] / (hdSize * parameter.nRun);
      mainFile << "; " << result.simpsonCommunity[0] / parameter.nRun << "; ";
      mainFile << result.shannonCommunity[0] / parameter.nRun << "; ";
      mainFile << result.bergerParkerHD[0] / (hdSize * parameter.nRun) << "; ";
      mainFile << result.simpsonHD[0] / parameter.nRun << "; ";
      mainFile << result.shannonHD[0] / parameter.nRun << endl;
      for(int i = 0; i < round(1 / 0.05); ++i){
        histogramFile << i*0.05 + 0.025 << "; ";
        histogramFile << 100 *result.productivityFrequency[i] / parameter.nRun;
        histogramFile << "; ";
        histogramFile << 100 * result.qualityFrequency[i] / parameter.nRun;
        histogramFile << "; " << paramValue << endl;
      }
      for(int i = 0; i <= hdSize - 1; ++i){
        hdDistFile << i << "; ";
        hdDistFile << 100 * result.hdDistribution[i] / parameter.nRun << "; ";
        hdDistFile << paramValue << endl;
      }
      for(int i = 0; i < parameter.numberHousehold; ++i){
        varietyDistFile << 100.0*(i+1) / parameter.numberHousehold << "; ";
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
    hdDistFile.close();
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
