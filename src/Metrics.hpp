namespace metrics{
  void printState(int t, Patch* grid, const int t_latticeSize);
  void floatToRGB(int n, int* R, int* G, int* B);


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
            Image(i*SIZE+x,j*SIZE+y)->Red = r;
            Image(i*SIZE+x,j*SIZE+y)->Green = g;
            Image(i*SIZE+x,j*SIZE+y)->Blue = b;
            Image(i*SIZE+x,j*SIZE+y)->Alpha = 0.1;
          }
      }
    }
    //Image.SetDPI(1000,1000);
    Image.WriteToFile(name.c_str());

  }


  void floatToRGB(int n, int* R, int* G, int* B){
    float r, g, b, x = (float) n / 50;

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
