#ifndef MODELRUNNER_H
#define MODELRUNNER_H

class ModelRunner{
public:
  void run_standard(void);
  void run_plot(void);
  void run_var_param(char param, std::vector<float> paramList);
  void run_var_param_fixed_points(char param);
private:
  void write_standard_results(Parameter parameter, Result* result);
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

  write_standard_results(parameter, &result);
}

void ModelRunner::run_plot(void){
  Parameter parameter;
  Model model(parameter);
  Result result(0, 0, 0, 0);
  parameter.nRun = 1;

  clock_t tStart = clock();
  result = model.runPlot();
  cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;

  write_standard_results(parameter, &result);
}


void ModelRunner::run_var_param(char param, std::vector<float> paramList){
  Parameter parameter;
  if(param == 'm')
    parameter.nRun = 1;

  std::string param_str (1, param);
  Data timeline("varParam_" + param_str, parameter);
  Data histogram_productivity("histogramProductivityVar_" + param_str, parameter);
  Data hd_distribution("hdDistribution_" + param_str, parameter);
  Data variety_distribution("varietyDistribution_" + param_str, parameter);
  Data variety_quantity("varietyQuantity_" + param_str, parameter);

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
    cout << "Time taken: "<< (double)(clock()-tStart)/CLOCKS_PER_SEC << endl;

    timeline.write_timeline(&result, paramValue);
    histogram_productivity.write_histogram_productivity(&result, paramValue);
    hd_distribution.write_hd_distribution(&result, paramValue);
    variety_distribution.write_variety_distribution(&result, paramValue);
    variety_quantity.write_variety_quantity(&result, paramValue);

  }
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

void ModelRunner::write_standard_results(Parameter parameter, Result* result){
  Data timeline("standard", parameter);
  Data histogram_productivity("histogramProductivity", parameter);
  Data hd_distribution("hdDistribution", parameter);
  Data variety_distribution("varietyDistribution", parameter);
  Data variety_quantity("varietyQuantity", parameter);

  timeline.write_timeline(result);
  histogram_productivity.write_histogram_productivity(result);
  hd_distribution.write_hd_distribution(result);
  variety_distribution.write_variety_distribution(result);
  variety_quantity.write_variety_quantity(result);
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
