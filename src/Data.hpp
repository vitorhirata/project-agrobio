#ifndef DATA_H
#define DATA_H

class Data{
public:
  static std::string BASE_NAME;
  Data(std::string name, Parameter t_parameter);
  void write_standard(Result* result);
  void write_histogram_productivity(Result* result);
  void write_hd_distribution(Result* result);
  void write_variety_distribution(Result* result);
  void write_variety_quantity(Result* result);
private:
  fstream file;
  Parameter m_parameter;
};

Data::Data(std::string name, Parameter t_parameter)
  : m_parameter(t_parameter)
{
  file.open(BASE_NAME + name + ".csv", ios::out);
  m_parameter.write_to_file(file);
}

void Data::write_standard(Result* result){
  std::string header ("time; nVar; meanHD; totalPunctuation; ");
  header.append("productivityPunctuation; qualityPunctuation; ");
  header.append("bergerCommunity; simpsonCommunity; shannonCommunity; ");
  header.append("bergerHD; simpsonHD; shannonHD");

  file << header << endl;

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
    file << result->shannonHD[i] / m_parameter.nRun << endl;
  }
}

void Data::write_histogram_productivity(Result* result){
  file << "value; productivity; quality" << endl;

  for(int i = 0; i < round(1 / 0.05); ++i){
    file << i * 0.05 + 0.025 << "; ";
    file << 100 * result->productivityFrequency[i] / m_parameter.nRun;
    file << "; ";
    file << 100 * result->qualityFrequency[i] / m_parameter.nRun;
    file << endl;
  }
}

void Data::write_hd_distribution(Result* result){
  file << "value; hdDist" << endl;

  int hdSize = m_parameter.latticeSize / sqrt(m_parameter.numberHousehold);
  hdSize = hdSize * hdSize;

  for(int i = 0; i <= hdSize - 1; ++i){
    file << i << "; " << 100 * result->hdDistribution[i] / m_parameter.nRun;
    file << endl;
  }
}

void Data::write_variety_distribution(Result* result){
  file << "value; varDist" << endl;

  for(int i = 0; i < m_parameter.numberHousehold; ++i){
    file << 100.0 * (i + 1.0) / m_parameter.numberHousehold << "; ";
    file << 100 * result->varietyDistribution[i] / m_parameter.nRun << endl;
  }
}

void Data::write_variety_quantity(Result* result){
  file << "quantity; frequency" << endl;

  for(int i = 0; i < result->varietyQuantity.size(); ++i){
    file << 100 * (pow(10,i * 0.2)) / pow(m_parameter.latticeSize, 2) << "; ";
    file << 100 * result->varietyQuantity[i] / m_parameter.nRun << endl;
  }
}

std::string Data::BASE_NAME = "test/" + to_string(time(NULL)) + '_';

#endif
