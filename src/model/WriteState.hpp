#ifndef WRITESTATE_H
#define WRITESTATE_H

class WriteState{
public:
  WriteState(Patch* t_grid, int t_latticeSize);
  void printState(int t);
private:
  Patch* m_grid;
  const int m_latticeSize;
  static std::string BASE_NAME;
  static int COLOR_GRANULARITY;
  static int SIZE;
  void floatToRGB(int n, int* R, int* G, int* B);
};

WriteState::WriteState(Patch* t_grid, int t_latticeSize)
  : m_grid(t_grid)
  , m_latticeSize(t_latticeSize) { }


void WriteState::printState(int t){
  BMP Image;

  Image.SetSize(m_latticeSize*SIZE,m_latticeSize*SIZE);
  Image.SetBitDepth(8);
  int r, g, b;

  for(int i = 0; i < m_latticeSize; ++i){
    for(int j = 0; j < m_latticeSize; ++j){
      int var  = m_grid[i*m_latticeSize+j].variety.varietyNumber;
      float alpha = 0.1;
      if(var == -1){
        r = g = b = 0;
        alpha = 0.0;
      }
      else
        floatToRGB(var, &r, &g, &b);
      for(int x = 0; x < SIZE; x++)
        for(int y = 0; y < SIZE; y++){
          Image(j*SIZE+y, i*SIZE+x)->Red = r;
          Image(j*SIZE+y, i*SIZE+x)->Green = g;
          Image(j*SIZE+y, i*SIZE+x)->Blue = b;
          Image(j*SIZE+y, i*SIZE+x)->Alpha = alpha;
        }
    }
  }

  std::string name (BASE_NAME + std::to_string(t) + ".bmp");
  Image.WriteToFile(name.c_str());
}

void WriteState::floatToRGB(int n, int* R, int* G, int* B){
  float r, g, b, x = (float) n / COLOR_GRANULARITY;

  if (x < 0 || x > 1){
    cout << "ERROR: INVALID VARIETY NUMBER." << endl;
    exit(-1);
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

std::string WriteState::BASE_NAME = "test/plot/time";
int WriteState::COLOR_GRANULARITY = 10101020;
int WriteState::SIZE = 10;
#endif
