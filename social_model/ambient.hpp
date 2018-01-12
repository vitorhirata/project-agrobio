class ambient{
private:
  propriety* proprietyList;
  patch* grid;
  bool adjMatrix[NPROPRIETY*NPROPRIETY];
  void initializeGrid(void);
  void initializeProp(void);
public:
  int countSpecie(void);
  void UpdatePropriety(int prop);
  void iterate(void);
  void replant(void);
  void printState(void);
  ambient();
};

// Initialize one ambient, distributting resource according to the parameters and initialize all patches.
ambient::ambient(){
  initializeGrid();
  initializeProp();
}


void ambient::initializeProp(void){
  proprietyList = new (nothrow) propriety[NPROPRIETY];
  if (proprietyList == nullptr){
    cout << "ERROR: proprietyList alocation not successful." << endl;
    exit(-1);
  }

  int propRadix = round(sqrt(NPROPRIETY));
  if(propRadix*propRadix == NPROPRIETY && LATTICESIZE % propRadix == 0){
    int propSizeRadix = LATTICESIZE/propRadix;
    for(int Col = 0; Col < propRadix; Col++){
      for(int Lin = 0; Lin < propRadix; Lin++){
        proprietyList[Col*propRadix+Lin].LimO = Lin*propSizeRadix;
        proprietyList[Col*propRadix+Lin].LimL = Lin*propSizeRadix + propSizeRadix - 1;
        proprietyList[Col*propRadix+Lin].LimN = Col*propSizeRadix;
        proprietyList[Col*propRadix+Lin].LimS = Col*propSizeRadix + propSizeRadix - 1;
        proprietyList[Col*propRadix+Lin].probInovation = gauss(rand64);
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

  totalSpecie.erase(totalSpecie.begin());
  speciePunctuation.erase(speciePunctuation.begin());

  int erased = 0;
  for(int i = 0; i < NMAXSPECIE; i++){
    if(totalSpecie[i-erased] == 0){
      totalSpecie.erase(totalSpecie.begin()+i-erased);
      speciePunctuation.erase(speciePunctuation.begin()+i-erased);
      erased++;
    }
    else{
      speciePunctuation[i-erased] /= totalSpecie[i-erased];
      totalSpecie[i-erased] = i+1;
    }
  }
  proprietyList[prop].setSpecie(totalSpecie, speciePunctuation);
}

// Iterate one time, passing over all the sites of the grid.
void ambient::iterate(void){
  int x, y, x_Neigh, y_Neigh;
  std::vector<int> x_list(LATTICESIZE), y_list(LATTICESIZE);

  // Initialize the list over which the program will pass updating the program.
  for(int i=0;i<LATTICESIZE;i++)
    x_list[i]=y_list[i]=i;
  std::random_shuffle(x_list.begin(),x_list.end());
  std::random_shuffle(y_list.begin(),y_list.end());


  for(int i=0;i<LATTICESIZE;i++){
    x = x_list[i];
    for(int j=0;j<LATTICESIZE;j++){
      y = y_list[j];
      if(grid[x*LATTICESIZE+y].filed){
        if(uniFLOAT(rand64) < DEATHPROB) // death probability
          grid[x*LATTICESIZE+y].kill();
      }
    }
  }
  for(int i = 0; i < NPROPRIETY; i++){
    UpdatePropriety(i);
    proprietyList[i].chooseSpecie();
  }
  replant();
}

void ambient::replant(void){
  int propRadix = sqrt(NPROPRIETY);
  int propSizeRadix = LATTICESIZE / propRadix;
  int propNum;

  for(int i = 0; i < LATTICESIZE; i++){
    for(int j = 0; j < LATTICESIZE; j++){
      propNum = (i/propSizeRadix)*propRadix + (j/propSizeRadix);
      grid[i*LATTICESIZE+j].fill(proprietyList[propNum].plantSpecie());
    }
  }
}

void ambient::printState(void){
  for(int i=0;i<LATTICESIZE;i++){
    for(int j=0;j<LATTICESIZE;j++){
      cout << grid[i*LATTICESIZE+j].specie.to_ulong() << " ";
    }
    cout << endl;
  }
  cout << endl << endl;
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
