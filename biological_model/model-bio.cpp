#include "header_bio.hpp"


int Run_standart(void);
int Run_varParam(char param, std::vector<float> paramList);

// Run the standart model, saving a txt with the evolution of the number of species.
int Run_standart(void){
  std::vector<int> result(T/tic,0);
  fstream arquivo;
  arquivo.open("test/standart.csv",ios::out);
  int i, j;

  // Rodo nRun rodadas
  for (int run=0; run < nRun; run++){
    ambient model(nResource);
    for (i=0;i<k;i++)
      for (j=0;j<n;j++)
        K[i*n+j] = gauss(rand64);

    clock_t tStart = clock();
    for (int t=0;t<T;t++){
      model.iterate();
      if (t % tic == 0)
        result[t/tic] += model.countSpecie();
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  }

  arquivo << "time; nSpecie" << endl;
  for (int t=0;t<T/tic;t++)
    arquivo << t*tic << "; " << result[t]/nRun << endl;
  arquivo.close();
  return 0;
}


// Run the model for some parameter varying, saving a txt with the evolution of the number of species for each parameter value.
int Run_varParam(char param, std::vector<float> paramList){
  if (param != 'm' && param != 'u' && param != 'L'){
    cout << "Invalid parameter, only m, u and L can vary." << endl;
    return -1;
  }

  std::vector<int> result((T/tic)*paramList.size(),0);
  fstream arquivo;
  arquivo.open(std::string ("test/varParam_")+param+".csv",ios::out);
  int i, j, idxParam;

  for (idxParam=0; idxParam < paramList.size(); idxParam++){
    switch (param){
      case 'm':
        m=paramList[idxParam];
        break;
      case 'u':
        u=paramList[idxParam];
        break;
      case 'L':
        L=paramList[idxParam];
        break;
    }

  // Run nRun rounds
    clock_t tStart = clock();
    for (int run=0; run < nRun; run++){
      ambient model(nResource);
      for (i=0;i<k;i++)
        for (j=0;j<n;j++)
          K[i*n+j] = gauss(rand64);

      for (int t=0;t<T;t++){
        model.iterate();
        if (t % tic == 0)
          result[(T/tic)*idxParam+t/tic] += model.countSpecie();
      }
    }
    cout << "Finish " << param << " = " << paramList[idxParam] << ". Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  }

  arquivo << "time; nSpecie; param" << endl;
  for (idxParam=0; idxParam < paramList.size(); idxParam++)
    for (int t = 0; t < T/tic; t++)
      arquivo << t*tic << "; " << result[(T/tic)*idxParam+t]/nRun << "; " << paramList[idxParam] << endl;

  arquivo.close();
  return 0;
}

int main(){
  //Run_standart();
  Run_varParam('u', {0.1,0.11});
  return 0;
}
