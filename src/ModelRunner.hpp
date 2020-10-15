#ifndef MODELRUNNER_H
#define MODELRUNNER_H

class ModelRunner{
public:
  void run_standard(void);
  void run_plot(void);
  void run_var_param(char param, std::vector<float> paramList);
  void run_var_param_fixed_points(char param);
private:
  fstream open_file(
      std::string fileName, std::string varNames, Parameter parameter);
};

void ModelRunner::run_standard(void){
  Parameter parameter;
  Result result(parameter.maxTime/parameter.timeInterval,
      round(1 / 0.05), parameter.latticeSize, 17);
  Result resultTemp(0, 0, 0, 0);

  clock_t tStart = clock();
  for(int run = 0; run < parameter.nRun; ++run){
    Model model(parameter);
    resultTemp = model.runStandard();
    result.sumResult(&resultTemp);
  }
  cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;

  Data standard("standard", parameter);
  standard.write_standard(&result);

  fstream histogramFile = open_file(
      Data::BASE_NAME + "histogramProductivity.csv",
      "value; productivity; quality", parameter);
  fstream hdDistFile = open_file(
      Data::BASE_NAME + "hdDistribution.csv",
      "value; hdDist", parameter);
  fstream varietyDistFile = open_file(
      Data::BASE_NAME + "varietyDistribution.csv",
      "value; varDist", parameter);
  fstream varietyQuantFile = open_file(
      Data::BASE_NAME + "varietyQuantity.csv",
      "quantity; frequency", parameter);

  for(int i = 0; i < round(1 / 0.05); ++i){
    histogramFile << i*0.05 + 0.025 << "; ";
    histogramFile << 100 * result.productivityFrequency[i] / parameter.nRun;
    histogramFile << "; ";
    histogramFile << 100 * result.qualityFrequency[i] / parameter.nRun;
    histogramFile << endl;
  }

  int hdSize = parameter.latticeSize / sqrt(parameter.numberHousehold);
  hdSize = hdSize * hdSize;
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

void ModelRunner::run_plot(void){
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
  fstream timeFile = open_file(
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

  fstream histogramFile = open_file(
      "test/plot/" + timestr + "_histogramProductivity.csv",
      "value; productivity; quality", parameter);
  fstream hdDistFile = open_file(
      "test/plot/" + timestr + "_hdDistribution.csv",  "value; hdDist",
      parameter);
  fstream varietyDistFile = open_file(
      "test/plot/" + timestr + "_varietyDistribution.csv",
      "value; varDist", parameter);
  fstream varietyQuantFile = open_file(
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


void ModelRunner::run_var_param(char param, std::vector<float> paramList){
  Parameter parameter;
  if(param == 'm')
    parameter.nRun = 1;

  time_t now = time(NULL);
  std::string timestr = to_string(now);

  std::string header ("time; nVar; meanHD; totalPunctuation; ");
  header.append("productivityPunctuation; qualityPunctuation; ");
  header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
  header.append("bergerHD; simpsonHD; shannonHD; param");
  fstream timeFile = open_file(
      "test/" + timestr + "_varParam_" + param + ".csv",
      header, parameter);
  fstream histogramFile = open_file(
      "test/" + timestr + "_histogramProductivityVar_" + param + ".csv",
      "value; productivity; quality; param", parameter);
  fstream hdDistFile = open_file(
      "test/" + timestr + "_hdDistribution_" + param + ".csv",
      "value; hdDist; param", parameter);
  fstream varietyDistFile = open_file(
      "test/" + timestr + "_varietyDistribution_" + param + ".csv",
      "value; varDist; param", parameter);
  fstream varietyQuantFile = open_file(
      "test/" + timestr + "_varietyQuantity_" + param + ".csv",
      "quantity; frequency; param", parameter);

  for(auto paramValue : paramList){
    parameter.set_parameter(param, paramValue);

    clock_t tStart = clock();
    Result result(parameter.maxTime/parameter.timeInterval,
        round(1 / 0.05), parameter.latticeSize, 17);
    Result resultTemp(0, 0, 0, 0);
    for(int run = 0; run < parameter.nRun; ++run){
      Model model(parameter);
      resultTemp = model.runStandard();
      result.sumResult(&resultTemp);
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

void ModelRunner::run_var_param_fixed_points(char param){
  Parameter parameter;

  time_t now = time(NULL);
  std::string timestr = to_string(now);

  std::string header ("param; nVar; meanHD; totalPunctuation; ");
  header.append("productivityPunctuation; qualityPunctuation; ");
  header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
  header.append("bergerHD; simpsonHD; shannonHD");
  fstream mainFile = open_file(
      "test/" + timestr + "_varParamFixedPoints_" + param + ".csv",
      header, parameter);
  fstream histogramFile = open_file(
      "test/" + timestr + "_histogramProductivityVar_" + param + ".csv",
      "value; productivity; quality; param", parameter);
  fstream hdDistFile = open_file(
      "test/" + timestr + "_hdDistribution_" + param + ".csv",
      "value; hdDist; param", parameter);
  fstream varietyDistFile = open_file(
      "test/" + timestr + "_varietyDistribution_" + param + ".csv",
      "value; varDist; param", parameter);
  fstream varietyQuantFile = open_file(
      "test/" + timestr + "_varietyQuantity_" + param + ".csv",
      "quantity; frequency; param", parameter);

  std::vector<float> paramList = Parameter::get_parameter_variation(param);

  for(auto paramValue : paramList){
    parameter.set_parameter(param, paramValue);

    clock_t tStart = clock();
    Result result(1, round(1 / 0.05), parameter.latticeSize, 17);
    Result resultTemp(0, 0, 0, 0);
    result.numberVariety[0];
    resultTemp.numberVariety.push_back(0);

    for(int run = 0; run < parameter.nRun; ++run){
      Model model(parameter);
      resultTemp = model.runFixedPoint();
      result.sumResult(&resultTemp);
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

fstream ModelRunner::open_file(
    std::string fileName, std::string varNames, Parameter parameter){
  fstream file;
  file.open(fileName,ios::out);
  parameter.write_to_file(file);
  file << varNames << endl;
  return file;
}

#endif
