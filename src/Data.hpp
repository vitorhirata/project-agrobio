#ifndef DATA_H
#define DATA_H

class Data{
public:
  static std::string BASE_NAME;
  Data(std::string name, Parameter t_parameter);
  void write_timeline(Result* result, float param = -1.0);
  void write_histogram_productivity(Result* result, float param = -1.0);
  void write_hd_distribution(Result* result, float param = -1.0);
  void write_variety_distribution(Result* result, float param = -1.0);
  void write_variety_quantity(Result* result, float param = -1.0);
  void write_fixed_points(Result* result, float param);
private:
  fstream file;
  Parameter m_parameter;
  void write_header(std::string name, float param);
  bool need_header = true;
};

Data::Data(std::string name, Parameter t_parameter)
  : m_parameter(t_parameter)
{
  file.open(BASE_NAME + name + ".csv", ios::out);
  m_parameter.write_to_file(file);
}

void Data::write_timeline(Result* result, float param){
  if(need_header){
    std::string header ("time; nVar; meanHD; totalPunctuation; ");
    header.append("productivityPunctuation; qualityPunctuation; ");
    header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
    header.append("bergerHD; simpsonHD; shannonHD");
    write_header(header, param);
  }

  int hdSize = m_parameter.latticeSize / sqrt(m_parameter.numberHousehold);
  hdSize = hdSize * hdSize;

  for(int i = 0; i < m_parameter.maxTime/m_parameter.timeInterval; ++i){
    file << i*m_parameter.timeInterval << "; ";
    file << (float) result->numberVariety[i] / m_parameter.nRun << "; ";
    file << (float) result->meanVarietyHD[i] / m_parameter.nRun << "; ";
    file << result->totalPunctuation[i] / m_parameter.nRun << "; ";
    file << result->productivityPunctuation[i] / m_parameter.nRun << "; ";
    file << (result->totalPunctuation[i]-
        result->productivityPunctuation[i]) / m_parameter.nRun << "; ";
    file << result->bergerParkerCommunity[i] / (hdSize * m_parameter.nRun);
    file << "; " << result->simpsonCommunity[i] / m_parameter.nRun << "; ";
    file << result->shannonCommunity[i] / m_parameter.nRun << "; ";
    file << result->bergerParkerHD[i] / (hdSize * m_parameter.nRun) << "; ";
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

  for(int i = 0; i < round(1 / 0.05); ++i){
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

  int hdSize = m_parameter.latticeSize / sqrt(m_parameter.numberHousehold);
  hdSize = hdSize * hdSize;

  for(int i = 0; i <= hdSize - 1; ++i){
    file << i << "; " << 100 * result->hdDistribution[i] / m_parameter.nRun;
    if(round(param) != -1)
      file << "; " << param;
    file << endl;
  }
}

void Data::write_variety_distribution(Result* result, float param){
  if(need_header)
    write_header("value; varDist", param);

  for(int i = 0; i < m_parameter.numberHousehold; ++i){
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
    file << 100 * (pow(10,i * 0.2)) / pow(m_parameter.latticeSize, 2) << "; ";
    file << 100 * result->varietyQuantity[i] / m_parameter.nRun;
    if(round(param) != -1)
      file << "; " << param;
    file << endl;
  }
}

void Data::write_fixed_points(Result* result, float param){
  if(need_header){
    std::string header ("param; nVar; meanHD; totalPunctuation; ");
    header.append("productivityPunctuation; qualityPunctuation; ");
    header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
    header.append("bergerHD; simpsonHD; shannonHD");
    write_header(header, -1);
  }

  int hdSize = m_parameter.latticeSize / sqrt(m_parameter.numberHousehold);
  hdSize = hdSize * hdSize;

  file << param << "; ";
  file << (float) result->numberVariety[0] / m_parameter.nRun << "; ";
  file << (float) result->meanVarietyHD[0] / m_parameter.nRun << "; ";
  file << result->totalPunctuation[0] / m_parameter.nRun << "; ";
  file << result->productivityPunctuation[0] / m_parameter.nRun << "; ";
  file << (result->totalPunctuation[0] -
      result->productivityPunctuation[0]) / m_parameter.nRun << "; ";
  file << result->bergerParkerCommunity[0] / (hdSize * m_parameter.nRun);
  file << "; " << result->simpsonCommunity[0] / m_parameter.nRun << "; ";
  file << result->shannonCommunity[0] / m_parameter.nRun << "; ";
  file << result->bergerParkerHD[0] / (hdSize * m_parameter.nRun) << "; ";
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

std::string Data::BASE_NAME = "test/" + to_string(time(NULL)) + '_';

#endif
