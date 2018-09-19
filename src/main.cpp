#include "header.hpp"

int main(int argc, char *argv[]){

  if (argc == 1){
    cout << "ERROR: Number of input argument invalid. Enter the kind of model you want to run on executing this program." << endl;
    exit(-1);
  }
  switch (argv[1][0]) {
    case 's':
      if (argc > 2)
        cout << "WARNING: Number of input argument is invalid." << endl;
      cout << "Running standard model" << endl;
      worker::Run_standard();
      break;
    case 'p':
      if (argc > 2)
        cout << "WARNING: Number of input argument is invalid." << endl;
      cout << "Running model with plot" << endl;
      worker::Run_plot();
      break;
    case 'v':{
      char param;
      if (argc < 4) {
        cout << "ERROR: Number of input argument invalid. 'v' mode must have the parameter that will vary and the values it will assume" << endl;
        exit(-1);
      }
      if (argv[2][0] == 'o' || argv[2][0] == 'i' || argv[2][0] == 'L' || argv[2][0] == 'n' || argv[2][0] == 'a')
        param = argv[2][0];
      else{
        cout << "ERROR: Invalid parameter variation." << endl;
        exit(-1);
      }

      std::vector<float> param_list;
      float temp;
      for (int pos = 3; pos < argc; ++pos){
        temp = atof(argv[pos]);
        param_list.push_back(temp);
      }
      cout << "Running variation model, with variable " << param << ". " << endl;
      worker::Run_varParam(param, param_list);
      break;
    }
    default:
      cout << "ERROR: Invalid model type." << endl;
      exit(-1);
  }
  return 0;
}
