#include "header_bio.hpp"


int Run_standart(void);
int Run_varParam(char param, std::vector<float> paramList);

// Run the standart model, saving a txt with the evolution of the number of species.
int Run_standart(void){
  std::vector<int> result(MAXTIME/TIMEINTERVAL,0);
  fstream arquivo;
  arquivo.open("test/standart.csv",ios::out);
  int i, j;

  // Rodo NRUN rodadas
  for (int run=0; run < NRUN; run++){
    ambient model;
    for (i = 0; i < NSPECIE; i++)
      for (j = 0; j < NRESOURCE; j++)
        K[i*NRESOURCE+j] = gauss(rand64);

    clock_t tStart = clock();
    for (int t=0; t < MAXTIME; t++){
      model.iterate();
      if (t % TIMEINTERVAL == 0)
        result[t/TIMEINTERVAL] += model.countSpecie();
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  }

  arquivo << "time; nSpecie" << endl;
  for (int t = 0; t < MAXTIME/TIMEINTERVAL; t++)
    arquivo << t*TIMEINTERVAL << "; " << result[t]/NRUN << endl;
  arquivo.close();
  return 0;
}


// Run the model for some parameter varying, saving a txt with the evolution of the number of species for each parameter value.
int Run_varParam(char param, std::vector<float> paramList){
  if (param != 'm' && param != 'u' && param != 'L'){
    cout << "Invalid parameter, only m, u and L can vary." << endl;
    return -1;
  }

  std::vector<int> result((MAXTIME/TIMEINTERVAL)*paramList.size(),0);
  fstream arquivo;
  arquivo.open(std::string ("test/varParam_")+param+".csv",ios::out);
  uint i, j, idxParam;

  for (idxParam=0; idxParam < paramList.size(); idxParam++){
    switch (param){
      case 'm':
        MUTATIONPROB=paramList[idxParam];
        break;
      case 'u':
        DEATHPROB=paramList[idxParam];
        break;
      case 'L':
        LATTICESIZE=paramList[idxParam];
        break;
    }

  // Run NRUN rounds
    clock_t tStart = clock();
    for (int run=0; run < NRUN; run++){
      ambient model;
      for (i = 0; i < NSPECIE; i++)
        for (j = 0; j < NRESOURCE; j++)
          K[i*NRESOURCE+j] = gauss(rand64);

      for (int t=0; t < MAXTIME; t++){
        model.iterate();
        if (t % TIMEINTERVAL == 0)
          result[(MAXTIME/TIMEINTERVAL)*idxParam+t/TIMEINTERVAL] += model.countSpecie();
      }
    }
    cout << "Finish " << param << " = " << paramList[idxParam] << ". Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  }

  arquivo << "time; nSpecie; param" << endl;
  for (idxParam=0; idxParam < paramList.size(); idxParam++)
    for (int t = 0; t < MAXTIME/TIMEINTERVAL; t++)
      arquivo << t*TIMEINTERVAL << "; " << result[(MAXTIME/TIMEINTERVAL)*idxParam+t]/NRUN << "; " << paramList[idxParam] << endl;

  arquivo.close();
  return 0;
}

int main(){
  //Run_standart();
  Run_varParam('u', {0.1,0.11});
  return 0;
}
