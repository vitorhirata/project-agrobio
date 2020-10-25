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
};

void ModelRunner::run_standard(void){
  Parameter parameter;
  Result result(parameter, parameter.maxTime / parameter.timeInterval + 1);
  Result resultTemp;

  clock_t tStart = clock();
  for(int run = 0; run < parameter.nRun; ++run){
    Model model(parameter);
    resultTemp = model.runStandard();
    result.sumResult(&resultTemp);
  }
  cout << "Time taken: "<< (clock() - tStart)/CLOCKS_PER_SEC << "s." << endl;

  write_standard_results(parameter, &result);
}

void ModelRunner::run_plot(void){
  Parameter parameter;
  Model model(parameter);
  Result result;
  parameter.nRun = 1;

  clock_t tStart = clock();
  result = model.runPlot();
  cout << "Time taken: "<< (clock() - tStart)/CLOCKS_PER_SEC << "s." << endl;

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
    Result result(parameter, parameter.maxTime / parameter.timeInterval + 1);
    Result resultTemp;
    for(int run = 0; run < parameter.nRun; ++run){
      Model model(parameter);
      resultTemp = model.runStandard();
      result.sumResult(&resultTemp);
    }
    cout << "Finish " << param << " = " << paramValue << ". ";
    cout << "Time taken: "<< (clock() - tStart)/CLOCKS_PER_SEC << "s." << endl;

    timeline.write_timeline(&result, paramValue);
    histogram_productivity.write_histogram_productivity(&result, paramValue);
    hd_distribution.write_hd_distribution(&result, paramValue);
    variety_distribution.write_variety_distribution(&result, paramValue);
    variety_quantity.write_variety_quantity(&result, paramValue);

  }
}

void ModelRunner::run_var_param_fixed_points(char param){
  Parameter parameter;

  std::string param_str (1, param);
  Data fixed_points("varParamFixedPoints_" + param_str, parameter);
  Data histogram_productivity("histogramProductivityVar_" + param_str, parameter);
  Data hd_distribution("hdDistribution_" + param_str, parameter);
  Data variety_distribution("varietyDistribution_" + param_str, parameter);
  Data variety_quantity("varietyQuantity_" + param_str, parameter);

  std::vector<float> paramList = Parameter::get_parameter_variation(param);

  for(auto paramValue : paramList){
    parameter.set_parameter(param, paramValue);

    clock_t tStart = clock();
    Result result(parameter, 1);
    Result resultTemp;
    result.numberVariety[0];
    resultTemp.numberVariety.push_back(0);

    for(int run = 0; run < parameter.nRun; ++run){
      Model model(parameter);
      resultTemp = model.runFixedPoint();
      result.sumResult(&resultTemp);
    }
    cout << "Finish " << param << " = " << paramValue << ". ";
    cout << "Time taken: "<< (clock() - tStart)/CLOCKS_PER_SEC << "s." << endl;

    fixed_points.write_fixed_points(&result, paramValue);
    histogram_productivity.write_histogram_productivity(&result, paramValue);
    hd_distribution.write_hd_distribution(&result, paramValue);
    variety_distribution.write_variety_distribution(&result, paramValue);
    variety_quantity.write_variety_quantity(&result, paramValue);

  }
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

#endif
