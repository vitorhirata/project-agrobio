#include "header.hpp"


int main(){

  const int latticeSize = 49;
  const int nVariety = 50; // Number of species in initialization
  const int numberResources = 3; // Number of existing resources
  const int nResourceDistribution = 1; // Number of different resources in grid
  const int maxTime = 50; // Max time of iteration
  const int timeInterval = 1; // Interval in which metrics are counted
  const int nDomesticUnity = 49; // Number of domestic unities
  const float probabilyConnection = 0.1; // Probability of connection
  const int nRun = 50; // number of rounds
  const float outsideTradeLimit = 0.0003;
  const float insideTradeLimit = 0.0001;

  std::vector<int> result(maxTime/timeInterval, 0);
  std::vector<int> temp;
  for(int run = 0; run < nRun; ++run){
    Model model(latticeSize, nVariety, numberResources, nResourceDistribution, maxTime, timeInterval, nDomesticUnity, probabilyConnection, outsideTradeLimit, insideTradeLimit);
    temp = model.runPlot();
    std::transform(result.begin(), result.end(), temp.begin(), result.begin(), std::plus<float>());
  }

  fstream arquivo;
  arquivo.open("test/standard.csv",ios::out);
  for(int i = 0; i < maxTime/timeInterval; ++i)
    arquivo << i*timeInterval << "; " << (float) result[i]/nRun << endl;
  arquivo.close();

  return 0;
}
