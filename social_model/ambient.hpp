class ambient{
private:
  propriety* proprietyList;
  patch* grid;
  int propRadix;
  int propSizeRadix;
  void initializeGrid(void);
  void initializeProp(void);
  void UpdatePropriety(int prop);
  void replant(void);
  void proprietyTrade(void);
  bool floatToRGB(int n, int* R, int* G, int* B);
public:
  int countSpecie(void);
  void iterate(void);
  void printState(int t);
  ambient();
};

// Initialize one ambient, distributting resource according to the parameters and initialize all patches.
ambient::ambient(){
  propRadix = sqrt(NPROPRIETY);
  propSizeRadix = LATTICESIZE / propRadix;
  initializeGrid();
  initializeProp();
}


void ambient::initializeProp(void){
  proprietyList = new (nothrow) propriety[NPROPRIETY];
  if (proprietyList == nullptr){
    cout << "ERROR: proprietyList alocation not successful." << endl;
    exit(-1);
  }

  int propRadixR = round(sqrt(NPROPRIETY));
  if(propRadixR*propRadixR == NPROPRIETY && LATTICESIZE % propRadixR == 0){
    int propSizeRadix = LATTICESIZE/propRadixR;
    for(int Col = 0; Col < propRadixR; Col++)
      for(int Lin = 0; Lin < propRadixR; Lin++)
        proprietyList[Col*propRadixR+Lin].proprietyConstruct(Lin, Col);
  }
  else{
    cout << "ERROR: NPROPRIETY should be a perfect square and multiple of LATTICESIZE" << endl;
    exit(-1);
  }
}

void ambient::initializeGrid(void){
  grid = new (nothrow) patch[LATTICESIZE*LATTICESIZE];
  if (grid == nullptr){
    cout << "ERROR: grid alocation not successful." << endl;
    exit(-1);
  }

  if (!(LATTICESIZE % NRESOURCEDIST == 0) && !(pow(sqrt(NRESOURCEDIST),2) == NRESOURCEDIST && LATTICESIZE % ((int) sqrt(NRESOURCEDIST)) == 0)){
    cout << "ERROR: only accepted number of resources that fit LATTICESIZE or LATTICESIZE^2." << endl;
    exit(-1);
  }

  if (NRESOURCEDIST == LATTICESIZE*LATTICESIZE){
    std::vector<float> res(NRESOURCE);
    for (int i = 0; i < LATTICESIZE; i++){
      for (int j = 0; j < LATTICESIZE; j++){
        for(int k=0; k < NRESOURCE; k++)
          res[k] = uniFLOAT(rand64);
        grid[i*LATTICESIZE+j].initializePt(res, uniIntSP(rand64));
      }
    }
  }
  else if (LATTICESIZE % ((int) sqrt(NRESOURCEDIST)) == 0){
    vector<vector<float> > res(NRESOURCEDIST, vector<float>(NRESOURCE));
    for (int i=0; i < NRESOURCEDIST; i++)
      for (int j=0; j < NRESOURCE; j++)
        res[i][j] = uniFLOAT(rand64);

    int idxX, idxY;
    int sizeS = LATTICESIZE / sqrt(NRESOURCEDIST);
    for (int i = 0; i < LATTICESIZE; i++){
      idxX = i / sizeS;
      for (int j = 0; j < LATTICESIZE; j++){
        idxY = j / sizeS;
        grid[i*LATTICESIZE+j].initializePt(res[idxX*sqrt(NRESOURCEDIST)+idxY], uniIntSP(rand64));
      }
    }
  }

  else if(LATTICESIZE % NRESOURCEDIST == 0){
    vector<vector<float> > res(NRESOURCEDIST, vector<float>(NRESOURCE));
    for (int i=0; i < NRESOURCEDIST; i++)
      for (int j=0; j < NRESOURCE; j++)
        res[i][j] = uniFLOAT(rand64);

    int idx;
    for (int i = 0; i < LATTICESIZE; i++){
      idx = i / (LATTICESIZE/NRESOURCEDIST);
      for (int j = 0; j < LATTICESIZE; j++)
        grid[i*LATTICESIZE+j].initializePt(res[idx], uniIntSP(rand64));
    }
  }
}

void ambient::UpdatePropriety(int prop){
  std::vector<int> totalSpecie(NMAXSPECIE+1,0);
  std::vector<float> speciePunctuation(NMAXSPECIE+1,0);

  for(int i = proprietyList[prop].LimN; i < proprietyList[prop].LimS+1; i++)
    for(int j = proprietyList[prop].LimO; j < proprietyList[prop].LimL+1; j++){
      int sp=grid[i*LATTICESIZE+j].specie;

      if(uniFLOAT(rand64) < VAR[sp-1].resistence*(1-grid[i*LATTICESIZE+j].Fitness())){
        grid[i*LATTICESIZE+j].kill();
      }
      else{
        totalSpecie[sp]++;
        speciePunctuation[sp] += (ALPHA*grid[i*LATTICESIZE+j].Fitness() + (1-ALPHA)*(1-abs(proprietyList[prop].personalPref-VAR[sp-1].appearance)));
      }
    }

  std::vector<int> totalSpClear;
  std::vector<float> spPuncClear;

  for(int i = 1; i < NMAXSPECIE+1; i++)
    if(totalSpecie[i] != 0){
      spPuncClear.push_back(speciePunctuation[i] / totalSpecie[i]);
      totalSpClear.push_back(i);
    }
  proprietyList[prop].setSpecie(totalSpClear, spPuncClear);
}

void ambient::replant(void){
  int propNum;

  for(int i = 0; i < LATTICESIZE; i++){
    for(int j = 0; j < LATTICESIZE; j++){
      propNum = (i/propSizeRadix)*propRadix + (j/propSizeRadix);
      grid[i*LATTICESIZE+j].fill(proprietyList[propNum].plantSpecie());
    }
  }
}

// Iterate one time, passing over all the sites of the grid.
void ambient::iterate(void){

  for(int i = 0; i < NPROPRIETY; i++)
    UpdatePropriety(i);
  for(int i = 0; i < NPROPRIETY; i++)
    proprietyList[i].chooseSpecie();

  replant();
}


// Count the number of observed species at the actual time, on the grid.
int ambient::countSpecie(void){
  int nDifferentS=0;
  std::vector<bool> alreadyExist(NMAXSPECIE+1,false);

  for(int i=0;i<LATTICESIZE;i++)
    for(int j=0;j<LATTICESIZE;j++){
      int sp=grid[i*LATTICESIZE+j].specie;
      if(!alreadyExist[sp]){
        alreadyExist[sp] = true;
        nDifferentS++;
      }
    }
  if(alreadyExist[0] == true)
    return nDifferentS-1;
  else
    return nDifferentS;
}


void ambient::printState(int t){
  BMP Image;
  std::string name ("test/plot/standard");
  int SIZE = 10;
  Image.SetSize(LATTICESIZE*SIZE,LATTICESIZE*SIZE);
  Image.SetBitDepth(8);

  name += std::to_string(t);
  name += ".bmp";
  int r, g, b, sp;

  for(int i=0;i<LATTICESIZE;i++){
    for(int j=0;j<LATTICESIZE;j++){
      int sp  = grid[i*LATTICESIZE+j].specie;
      floatToRGB(sp, &r, &g, &b);
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


bool ambient::floatToRGB(int n, int* R, int* G, int* B){
  float r, g, b, x = (float) n / NMAXSPECIE;

  if (x < 0 || x > 1){
    cout << "ERROR" << endl;
    return false;
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
    return false;
  }
  *R = (int) (r * 255);
  *G = (int) (g * 255);
  *B = (int) (b * 255);
  return true;
}
