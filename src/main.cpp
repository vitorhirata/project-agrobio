#include "header.hpp"

int main(int argc, char *argv[]){

  if (argc == 1){
    cout << "ERROR: Number of input argument invalid. ";
    cout << "Enter the kind of model you want to run on executing this ";
    cout << "program." << endl;
    exit(-1);
  }

  ModelRunner modelRunner;
  switch (argv[1][0]) {
    case 's':
      if (argc > 2)
        cout << "WARNING: Number of input argument is invalid." << endl;
      cout << "Running standard model" << endl;

      modelRunner.run_standard();
      break;
    case 'p':
      if (argc > 2)
        cout << "WARNING: Number of input argument is invalid." << endl;
      cout << "Running model with plot" << endl;

      modelRunner.run_plot();
      break;
    case 'v':{
      char param;
      if (argc < 4) {
        cout << "ERROR: Number of input argument invalid. 'v' mode must ";
        cout << "have the parameter that will vary and the values it will ";
        cout << "assume" << endl;
        exit(-1);
      }
      if (is_valid_key(argv[2][0])){
        param = argv[2][0];
      }
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
      cout << "Running variation model, with variable ";
      cout << param << ". " << endl;

      modelRunner.run_var_param(param, param_list);
      break;
    }
    case 'f':{
      char param;
      if (argc != 3) {
        cout << "ERROR: Number of input argument invalid. ";
        cout << "'f' mode must have only the parameter that will vary" << endl;
        exit(-1);
      }
      if (is_valid_key(argv[2][0]))
        param = argv[2][0];
      else{
        cout << "ERROR: Invalid parameter variation." << endl;
        exit(-1);
      }

      cout << "Running variation model fixed points, with variable ";
      cout << param << ". " << endl;

      modelRunner.run_var_param_fixed_points(param);
      break;
    }
    case 'm':{
      if (argc != 3){
        cout << "ERROR: Number of input argument invalid. 'm' mode must ";
        cout << "have only the parameter the number of runs" << endl;
        exit(-1);
      }
      int numRun = std::stoi(argv[2]);
      std::vector<float> param_list(numRun);
      std::iota(param_list.begin(), param_list.end(), 0);
      cout << "Running multiple run model, with ";
      cout << numRun << " runs." << endl;

      modelRunner.run_var_param('m', param_list);
      break;
    }
    default:
      cout << "ERROR: Invalid model type." << endl;
      exit(-1);
  }
  return 0;
}

bool is_valid_key(char parameter_key){
  return std::find(
      Parameter::keys.begin(),
      Parameter::keys.end(),
      parameter_key
  ) != Parameter::keys.end();
}
