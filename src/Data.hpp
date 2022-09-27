#ifndef DATA_H
#define DATA_H

/*
Class responsible for writting model's result in a csv file.
*/

class Data{
public:
  static std::string BASE_NAME;
  Data(std::string name, Parameter t_parameter);
  void write_timeline(Result* result, float param = -1.0);
  void write_histogram_productivity(Result* result, float param = -1.0);
  void write_hd_distribution(Result* result, float param = -1.0);
  void write_variety_distribution(Result* result, float param = -1.0);
  void write_variety_quantity(Result* result, float param = -1.0);
  void write_final_state(Result* result, float param, float param2 = -1.0);
private:
  fstream file;
  Parameter m_parameter;
  void write_header(std::string name, float param);
  bool need_header = true;
  static std::string BASE_TIMELINE_HEADER;
};

Data::Data(std::string name, Parameter t_parameter)
  : m_parameter(t_parameter)
{
  file.open(BASE_NAME + name + ".csv", ios::out);
  m_parameter.write_to_file(file);
}

void Data::write_timeline(Result* result, float param){
  if(need_header)
    write_header("time; " + BASE_TIMELINE_HEADER, param);

  for(int i = 0; i < result->numberVariety.size(); ++i){
    file << i*m_parameter.timeInterval << "; ";
    file << (float) result->numberVariety[i] / m_parameter.nRun << "; ";
    file << (float) result->meanVarietyHD[i] / m_parameter.nRun << "; ";
    file << result->totalScore[i] / m_parameter.nRun << "; ";
    file << result->productivityScore[i] / m_parameter.nRun << "; ";
    file << (result->totalScore[i]-
        result->productivityScore[i]) / m_parameter.nRun << "; ";
    file << result->bergerParkerCommunity[i] /
      (m_parameter.household_size() * m_parameter.nRun);
    file << "; " << result->simpsonCommunity[i] / m_parameter.nRun << "; ";
    file << result->shannonCommunity[i] / m_parameter.nRun << "; ";
    file << result->bergerParkerHD[i] /
      (m_parameter.household_size() * m_parameter.nRun) << "; ";
    file << result->simpsonHD[i] / m_parameter.nRun << "; ";
    file << result->shannonHD[i] / m_parameter.nRun;
    if(round(param) != -1)
      file << "; " << param;
    file << endl;
  }
}

void Data::write_histogram_productivity(Result* result, float param){
  if(need_header)
    write_header("value; productivity; quality", param);

  for(int i = 0; i < result->productivityFrequency.size(); ++i){
    file << i * 0.05 + 0.025 << "; ";
    file << 100 * result->productivityFrequency[i] / m_parameter.nRun;
    file << "; ";
    file << 100 * result->qualityFrequency[i] / m_parameter.nRun;
    if(round(param) != -1)
      file << "; " << param;
    file << endl;
  }
}

void Data::write_hd_distribution(Result* result, float param){
  if(need_header)
    write_header("value; hdDist", param);

  for(int i = 0; i <= result->hdDistribution.size(); ++i){
    file << i << "; " << 100 * result->hdDistribution[i] / m_parameter.nRun;
    if(round(param) != -1)
      file << "; " << param;
    file << endl;
  }
}

void Data::write_variety_distribution(Result* result, float param){
  if(need_header)
    write_header("value; varDist", param);

  for(int i = 0; i < result->varietyDistribution.size(); ++i){
    file << 100.0 * (i + 1.0) / m_parameter.numberHousehold << "; ";
    file << 100 * result->varietyDistribution[i] / m_parameter.nRun;
    if(round(param) != -1)
      file << "; " << param;
    file << endl;
  }
}

void Data::write_variety_quantity(Result* result, float param){
  if(need_header)
    write_header("quantity; frequency", param);

  for(int i = 0; i < result->varietyQuantity.size(); ++i){
    file << 100 * (pow(10,i * 0.2)) / m_parameter.number_patches() << "; ";
    file << 100 * result->varietyQuantity[i] / m_parameter.nRun;
    if(round(param) != -1)
      file << "; " << param;
    file << endl;
  }
}

void Data::write_final_state(Result* result, float param, float param2){
  if(need_header){
    std::string params = (round(param2) == -1) ? "param; " : "param; param2; ";
    write_header(params + BASE_TIMELINE_HEADER, -1);
  }

  file << param << "; ";
  if(round(param2) != -1)
    file << param2 << "; ";
  file << (float) result->numberVariety[0] / m_parameter.nRun << "; ";
  file << (float) result->meanVarietyHD[0] / m_parameter.nRun << "; ";
  file << result->totalScore[0] / m_parameter.nRun << "; ";
  file << result->productivityScore[0] / m_parameter.nRun << "; ";
  file << (result->totalScore[0] -
      result->productivityScore[0]) / m_parameter.nRun << "; ";
  file << result->bergerParkerCommunity[0] /
    (m_parameter.household_size() * m_parameter.nRun);
  file << "; " << result->simpsonCommunity[0] / m_parameter.nRun << "; ";
  file << result->shannonCommunity[0] / m_parameter.nRun << "; ";
  file << result->bergerParkerHD[0] /
    (m_parameter.household_size() * m_parameter.nRun) << "; ";
  file << result->simpsonHD[0] / m_parameter.nRun << "; ";
  file << result->shannonHD[0] / m_parameter.nRun << endl;
}

void Data::write_header(std::string name, float param){
  file << name;
  if(round(param) != -1)
    file << "; param";
  file << endl;

  need_header = false;
}

std::string Data::BASE_NAME = "src/test/" + to_string(time(NULL)) + '_';

std::string Data::BASE_TIMELINE_HEADER =
  "nVar; meanHD; "
  "totalScore; productivityScore; qualityScore; "
  "bergerCommunity; simpsonCommunity; shannonCommunity; "
  "bergerHD; simpsonHD; shannonHD";
#endif
