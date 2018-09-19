namespace worker{
  void Run_standard(void){
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
    arquivo << "time; nVar" << endl;
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
      arquivo << i*parameter.timeInterval << "; " << (float) temp[i] << endl;
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    arquivo.close();
  }

  void Run_plot(void){
    Parameter parameter;
    clock_t tStart = clock();
    Model model(parameter);
    std::vector<int> result = model.runStandard();

    fstream arquivo;
    arquivo.open("test/plot.csv",ios::out);
    arquivo << "time; nVar" << endl;
    for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
      arquivo << i*parameter.timeInterval << "; " << result[i] << endl;
    cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;
    arquivo.close();
  }


  void Run_varParam(char param, std::vector<float> paramList){
    Parameter parameter;

    fstream arquivo;
    arquivo.open(std::string ("test/varParam_")+param+".csv",ios::out);
    arquivo << "time; nVar; param" << endl;

    for(auto paramValue : paramList){
      switch (param){
        case 'o':
          parameter.outsideTradeLimit = paramValue;
          break;
        case 'i':
          parameter.insideTradeLimit = paramValue;
          break;
        case 'L':
          if (round(sqrt(paramValue)) * round(sqrt(paramValue)) != paramValue){
            cout << "ERROR: the latticeSize should be a perfect square." << endl;
            exit(-1);
          }
          parameter.latticeSize = paramValue;
          parameter.numberDomesticUnity = paramValue;
          break;
        case 'n':
          parameter.numberVariety = paramValue;
          break;
        case 'a':
          parameter.alpha = paramValue;
          break;
      }
      clock_t tStart = clock();
      std::vector<int> result(parameter.maxTime/parameter.timeInterval, 0);
      std::vector<int> temp;
      for(int run = 0; run < parameter.nRun; ++run){
        Model model(parameter);
        temp = model.runStandard();
        std::transform(result.begin(), result.end(), temp.begin(), result.begin(), std::plus<float>());
      }

      for(int i = 0; i < parameter.maxTime/parameter.timeInterval; ++i)
        arquivo << i*parameter.timeInterval << "; " << (float) temp[i] << "; " << paramValue << endl;
      cout << "Time taken: "<< (double)(clock() - tStart)/CLOCKS_PER_SEC << endl;

    }
    arquivo.close();
  }

}



namespace metrics{

  void printState(int t, Patch* grid, const int t_latticeSize){
    std::string name ("test/plot/standard");
    name += std::to_string(t);
    name += ".bmp";

    BMP Image;
    int SIZE = 10;
    Image.SetSize(t_latticeSize*SIZE,t_latticeSize*SIZE);
    Image.SetBitDepth(8);
    int r, g, b;

    for(int i = 0; i < t_latticeSize; ++i){
      for(int j = 0; j < t_latticeSize; ++j){
        int var  = grid[i*t_latticeSize+j].plantedVariety;
        floatToRGB(var, &r, &g, &b);
        for(int x = 0; x < SIZE; x++)
          for(int y = 0; y < SIZE; y++){
            Image(j*SIZE+y,i*SIZE+x)->Red = r;
            Image(j*SIZE+y,i*SIZE+x)->Green = g;
            Image(j*SIZE+y,i*SIZE+x)->Blue = b;
            Image(j*SIZE+y,i*SIZE+x)->Alpha = 0.1;
          }
      }
    }
    //Image.SetDPI(1000,1000);
    Image.WriteToFile(name.c_str());

  }


  void floatToRGB(int n, int* R, int* G, int* B){
    float r, g, b, x = (float) n / 100;

    if (x < 0 || x > 1){
      cout << "ERROR" << endl;
      exit(-1);
    }
    if (n == 0){
      r = 0;
      g = 0;
      b = 0;
    }
    if (x < 0.125){
      r = 0.5 + 4*x;
      g = 0;
      b = 0;
    }
    else if (x < 0.375){
      r = 1;
      g = -0.5 + 4*x;
      b = 0;
    }
    else if (x < 0.625){
      r = 2.5 - 4*x;
      g = 1;
      b = -1.5 + 4*x;
    }
    else if (x < 0.875){
      r = 0;
      g = 3.5 - 4*x;
      b = 1;
    }
    else{
      r = 0;
      g = 0;
      b = 4.5 - 4*x;
    }

    if (r < 0 || g < 0 || b < 0 || r > 1 || g > 1 || b > 1){
      cout << "ERROR: " << r << ", " << g << ", " << b << endl;
      exit(-1);
    }
    *R = (int) (r * 255);
    *G = (int) (g * 255);
    *B = (int) (b * 255);
  }
}
