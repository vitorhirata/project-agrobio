#ifndef DATA_H
#define DATA_H

class Data{
public:
  static std::string BASE_NAME;
  Data(std::string name, Parameter t_parameter);
  void write_standard(Result* result);
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

std::string Data::BASE_NAME = "test/" + to_string(time(NULL)) + '_';

#endif
