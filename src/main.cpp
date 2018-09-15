#include "header.hpp"


int main(){

  Parameter parameter;

  clock_t tStart = clock();
  std::vector<int> result(parameter.maxTime/parameter.timeInterval, 0);
  std::vector<int> temp;
  for(int run = 0; run < parameter.nRun; ++run){
    Model model(parameter);
    temp = model.runStandard();
    std::transform(result.begin(), result.end(), temp.begin(), result.begin(), std::plus<float>());
  }

  fstream arquivo;
  arquivo.open("test/standard.csv",ios::out);
  arquivo << "time; nVar; param" << endl;
  for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
    arquivo << i*parameter.timeInterval << "; " << (float) temp[i] << endl;
  cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  arquivo.close();

  return 0;
}
