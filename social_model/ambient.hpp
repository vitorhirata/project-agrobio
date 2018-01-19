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
  void setGraph(void);
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
  setGraph();
}

void ambient::setGraph(void){
  bool adjMatrix[NPROPRIETY*NPROPRIETY];

  for (int i = 0; i < NPROPRIETY; i++) {
    for (int j = 0; j < i; j++){
      if (uniFLOAT(rand64) < CONNECTIONPROB)
        adjMatrix[i*NPROPRIETY+j] = true;
    }
    adjMatrix[i*NPROPRIETY+i] = false;
  }
  for (int i = 0; i < NPROPRIETY; i++)
    for (int j = i+1; j < NPROPRIETY; j++)
      adjMatrix[i*NPROPRIETY+j] = adjMatrix[j*NPROPRIETY+i];

  for (int i = 0; i < NPROPRIETY; i++) {
    std::vector<int> propConnection;
    for (int j = 0; j < NPROPRIETY; j++)
      if (adjMatrix[i*NPROPRIETY+j] == true)
        propConnection.push_back(j);
    proprietyList[i].proprietyConnection = propConnection;
  }
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
    for(int Col = 0; Col < propRadixR; Col++){
      for(int Lin = 0; Lin < propRadixR; Lin++){
        proprietyList[Col*propRadixR+Lin].LimO = Lin*propSizeRadix;
        proprietyList[Col*propRadixR+Lin].LimL = Lin*propSizeRadix + propSizeRadix - 1;
        proprietyList[Col*propRadixR+Lin].LimN = Col*propSizeRadix;
        proprietyList[Col*propRadixR+Lin].LimS = Col*propSizeRadix + propSizeRadix - 1;
        proprietyList[Col*propRadixR+Lin].probInovation = gauss(rand64) - 0.5;
      }
    }
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
  if (LATTICESIZE % NRESOURCEDIST != 0 && LATTICESIZE*LATTICESIZE % NRESOURCEDIST != 0){
    cout << "ERROR: only accepted number of resources that are divisible by LATTICESIZE or LATTICESIZE^2'." << endl;
    exit(-1);
  }


  if (NRESOURCEDIST == LATTICESIZE*LATTICESIZE){
    std::vector<float> res(NRESOURCE);
    for (int i = 0; i < LATTICESIZE; i++){
      for (int j = 0; j < LATTICESIZE; j++){
        for(int k=0; k < NRESOURCE; k++)
          res[k] = uniFLOAT(rand64);
        grid[i*LATTICESIZE+j].initializePt(res, boost::dynamic_bitset<> (NSPECIEBYTES,uniIntSP(rand64)));
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
        grid[i*LATTICESIZE+j].initializePt(res[idx], boost::dynamic_bitset<> (NSPECIEBYTES,uniIntSP(rand64)));
    }
  }
  else if (LATTICESIZE*LATTICESIZE % NRESOURCEDIST == 0){
    vector<vector<float> > res(NRESOURCEDIST, vector<float>(NRESOURCE));
    for (int i=0; i < NRESOURCEDIST; i++)
      for (int j=0; j < NRESOURCE; j++)
        res[i][j] = uniFLOAT(rand64);

    int idxX, idxY;
    for (int i = 0; i < LATTICESIZE; i++){
      idxX = i / (LATTICESIZE/(NRESOURCEDIST/2));
      for (int j = 0; j < LATTICESIZE; j++){
        idxY = j / (LATTICESIZE/(NRESOURCEDIST/2));
        grid[i*LATTICESIZE+j].initializePt(res[idxX*(NRESOURCEDIST/2)+idxY], boost::dynamic_bitset<> (NSPECIEBYTES,uniIntSP(rand64)));
      }
    }
  }
}

void ambient::UpdatePropriety(int prop){
  std::vector<int> totalSpecie(NMAXSPECIE+1,0);
  std::vector<float> speciePunctuation(NMAXSPECIE+1,0);

  for(int i = proprietyList[prop].LimN; i < proprietyList[prop].LimS+1; i++)
    for(int j = proprietyList[prop].LimO; j < proprietyList[prop].LimL+1; j++){
      int sp=grid[i*LATTICESIZE+j].specie.to_ulong();
      totalSpecie[sp]++;
      speciePunctuation[sp] += grid[i*LATTICESIZE+j].punctuation();
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

void ambient::proprietyTrade(void){
  int x, size, prop, maxSp, maxValue;
  std::vector<int> x_list(NPROPRIETY);

  // Initialize the list over which the program will pass updating the program.
  for(int i = 0; i < NPROPRIETY; i++)
    x_list[i] = i;
  std::random_shuffle(x_list.begin(),x_list.end());


  for(int i = 0; i < NPROPRIETY; i++){
    x = x_list[i];
    if (uniFLOAT(rand64) < proprietyList[i].probInovation){
      // Select the specie with better punctuation along the proprietyConnection list
      maxValue = 0;
      size = proprietyList[i].proprietyConnection.size();
      for(int j = 0; j < size; j++){
        prop = proprietyList[i].proprietyConnection[j];
        if (proprietyList[prop].maxSpeciePunc > maxValue){
          maxValue = proprietyList[prop].maxSpeciePunc;
          maxSp = proprietyList[prop].maxSpecie;
        }
      }

      // Add specie and punctuation
      proprietyList[i].addSpecie(maxSp, maxValue);
    }
  }
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

  for(int i=0;i<LATTICESIZE;i++)
    for(int j=0;j<LATTICESIZE;j++)
      if(uniFLOAT(rand64) < DEATHPROB)
        grid[i*LATTICESIZE+j].kill();

  for(int i = 0; i < NPROPRIETY; i++){
    UpdatePropriety(i);
    proprietyList[i].chooseSpecie();
  }
  proprietyTrade();
  replant();
}


// Count the number of observed species at the actual time, on the grid.
int ambient::countSpecie(void){
  int nDifferentS=0;
  std::vector<bool> alreadyExist(NMAXSPECIE+1,false);

  for(int i=0;i<LATTICESIZE;i++)
    for(int j=0;j<LATTICESIZE;j++){
      int sp=grid[i*LATTICESIZE+j].specie.to_ulong();
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
      int sp  = grid[i*LATTICESIZE+j].specie.to_ulong();
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
