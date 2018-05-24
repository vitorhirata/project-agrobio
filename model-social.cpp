#include "header_social.hpp"

int Run_standard(void);
int Run_varParam(char param, std::vector<float> paramList);
int Run_plot(void);

int Run_standard(void){
  std::vector<int> result(MAXTIME/TIMEINTERVAL,0);
  fstream arquivo;
  arquivo.open("test/standard.csv",ios::out);
  int i, j;

  // Rodo NRUN rodadas
  for (int run=0; run < NRUN; run++){
    ambient model;
    clock_t tStart = clock();
    for (int t=0; t < MAXTIME; t++){
      if (t % TIMEINTERVAL == 0)
        result[t/TIMEINTERVAL] += model.countSpecie();
      model.iterate();
    }
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  }

  arquivo << "### PARAMETERS VALUE ###" << endl;
  arquivo << "### LATTICESIZE = " << LATTICESIZE << ", NSPECIE = " << NSPECIE << ", NRESOURCE = " << NRESOURCE;
  arquivo << ", MUTATIONPROB = " << MUTATIONPROB << ", NRESOURCEDIST = " << NRESOURCEDIST << ", NPROPRIETY = " << NPROPRIETY;
  arquivo << ", MAXTIME = " << MAXTIME << ", TIMEINTERVAL = " << TIMEINTERVAL << ", NRUN = " << NRUN << " ###" << endl << endl;
  arquivo << "time; nSpecie" << endl;
  for (int t = 0; t < MAXTIME/TIMEINTERVAL; t++)
    arquivo << t*TIMEINTERVAL << "; " << result[t]/NRUN << endl;
  arquivo.close();
  return 0;
}

int Run_plot(void){
  std::vector<int> result(MAXTIME/TIMEINTERVAL,0);
  fstream arquivo;
  arquivo.open("test/plot/Runplot.csv",ios::out);
  int i, j;

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


  arquivo << "### PARAMETERS VALUE ###" << endl;
  arquivo << "### LATTICESIZE = " << LATTICESIZE << ", NSPECIE = " << NSPECIE << ", NRESOURCE = " << NRESOURCE;
  arquivo << ", MUTATIONPROB = " << MUTATIONPROB << ", NRESOURCEDIST = " << NRESOURCEDIST << ", NPROPRIETY = " << NPROPRIETY;
  arquivo << ", MAXTIME = " << MAXTIME << ", TIMEINTERVAL = " << TIMEINTERVAL << ", NRUN = " << NRUN << " ###" << endl << endl;
  arquivo << "time; nSpecie" << endl;
  for (int t = 0; t < MAXTIME/TIMEINTERVAL; t++)
    arquivo << t*TIMEINTERVAL << "; " << result[t] << endl;
  arquivo.close();
  return 0;
}

// Run the model for some parameter varying, saving a txt with the evolution of the number of species for each parameter value.
int Run_varParam(char param, std::vector<float> paramList){
  if (param != 'm' && param != 'L' && param != 'n' && param != 'r' && param != 'a'){
    cout << "Invalid parameter, only m, L, n, r and a can vary." << endl;
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
      case 'L':
        LATTICESIZE=paramList[idxParam];
        NRESOURCEDIST = LATTICESIZE*LATTICESIZE;
        NPROPRIETY = LATTICESIZE;
        break;
      case 'n':
        if (paramList[idxParam] > NMAXSPECIE){
          cout << "ERROR: NSPECIE should be smaller than " << NMAXSPECIE << ". You entered the value " << paramList[idxParam] << endl;
          exit(-1);
        }
        NSPECIE=paramList[idxParam];
        uniIntSP.param(std::uniform_int_distribution<long>::param_type(0, NSPECIE));
        break;
      case 'r':
        NRESOURCEDIST = paramList[idxParam];
        break;
      case 'a':
        ALPHA = paramList[idxParam];
        break;
    }

  // Run NRUN rounds
    clock_t tStart = clock();
    for (int run=0; run < NRUN; run++){
      ambient model;
      for (int t=0; t < MAXTIME; t++){
        if (t % TIMEINTERVAL == 0)
          result[(MAXTIME/TIMEINTERVAL)*idxParam+t/TIMEINTERVAL] += model.countSpecie();
        model.iterate();
      }
    }
    cout << "Finish " << param << " = " << paramList[idxParam] << ". Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
  }

  arquivo << "### PARAMETERS VALUE ###" << endl;
  arquivo << "### LATTICESIZE = " << LATTICESIZE << ", NSPECIE = " << NSPECIE << ", NRESOURCE = " << NRESOURCE;
  arquivo << ", MUTATIONPROB = " << MUTATIONPROB << ", NRESOURCEDIST = " << NRESOURCEDIST << ", NPROPRIETY = " << NPROPRIETY;
  arquivo << ", MAXTIME = " << MAXTIME << ", TIMEINTERVAL = " << TIMEINTERVAL << ", NRUN = " << NRUN << " ###" << endl << endl;
  arquivo << "time; nSpecie; param" << endl;
  for (idxParam=0; idxParam < paramList.size(); idxParam++)
    for (int t = 0; t < MAXTIME/TIMEINTERVAL; t++)
      arquivo << t*TIMEINTERVAL << "; " << result[(MAXTIME/TIMEINTERVAL)*idxParam+t]/NRUN << "; " << paramList[idxParam] << endl;

  arquivo.close();
  return 0;
}

int main(int argc, char *argv[]){
  char param;
  std::vector<float> param_value;
  int pos = 3;
  float temp;

  if (argc == 1){
    cout << "ERROR: Number of input argument invalid. Enter the kind of model you want to run on executing this program." << endl;
    exit(-1);
  }
  switch (argv[1][0]) {
    case 's':
      if (argc > 2)
        cout << "WARNING: Number of input argument is invalid." << endl;
      cout << "Running standard model" << endl;
      Run_standard();
      break;
    case 'p':
      if (argc > 2)
        cout << "WARNING: Number of input argument is invalid." << endl;
      cout << "Running model with plot" << endl;
      Run_plot();
      break;
    case 'v':
      if (argc < 4) {
        cout << "ERROR: Number of input argument invalid. 'v' mode must have the parameter that will vary and the values it will assume" << endl;
        exit(-1);
      }
      if (argv[2][0] == 'a' || argv[2][0] == 'm' || argv[2][0] == 'L' || argv[2][0] == 'n' || argv[2][0] == 'r')
        param = argv[2][0];
      else{
        cout << "ERROR: Invalid parameter variation." << endl;
        exit(-1);
      }

      while (pos < argc) {
        temp = atof(argv[pos]);
        param_value.push_back(temp);
        pos++;
      }

      cout << "Running variation model, with variable " << param << ". " << endl;
      Run_varParam(param, param_value);
      break;

    default:
      cout << "ERROR: Invalid model type." << endl;
      exit(-1);
  }
  return 0;
}
