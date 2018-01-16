#include "header_social.hpp"


int main(){
  std::vector<int> result(MAXTIME/TIMEINTERVAL,0);
  fstream arquivo;
  arquivo.open("test/standart.csv",ios::out);
  int i, j;

  // Rodo NRUN rodadas
  for (int run=0; run < NRUN; run++){
    ambient model;
    clock_t tStart = clock();
    for (int t=0; t < MAXTIME; t++){
      if (t % TIMEINTERVAL == 0){
        result[t/TIMEINTERVAL] += model.countSpecie();
        model.printState(t);
      }
      model.iterate();
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  }

  arquivo << "### PARAMETERS VALUE ###" << endl;
  arquivo << "### LATTICESIZE = " << LATTICESIZE << ", NSPECIE = " << NSPECIE << ", NRESOURCE = " << NRESOURCE;
  arquivo << ", DEATHPROB = " << DEATHPROB << ", MUTATIONPROB = " << MUTATIONPROB << ", NRESOURCEDIST = " << NRESOURCEDIST;
  arquivo << ", MAXTIME = " << MAXTIME << ", TIMEINTERVAL = " << TIMEINTERVAL << ", NRUN = " << NRUN << " ###" << endl << endl;
  arquivo << "time; nSpecie" << endl;
  for (int t = 0; t < MAXTIME/TIMEINTERVAL; t++)
    arquivo << t*TIMEINTERVAL << "; " << result[t]/NRUN << endl;
  arquivo.close();

  return 0;
}
