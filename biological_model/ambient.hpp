class ambient{
private:
  patch* grid;
  bool haveNeighbor(int x, int y, int* x_N, int* y_N);
public:
  void iterate(void);
  int countSpecie(void);
  ambient();
};

// Initialize one ambient, distributting resource according to the parameters and initialize all patches.
ambient::ambient(){
  grid = new (nothrow) patch[LATTICESIZE*LATTICESIZE];
  if (grid == nullptr){
    cout << "Error: grid alocation not successful." << endl;
    exit(-1);
  }
  if (LATTICESIZE % NRESOURCEDIST != 0 && LATTICESIZE*LATTICESIZE % NRESOURCEDIST != 0){
    cout << "Error: only accepted number of resources that are divisible by LATTICESIZE or LATTICESIZE^2'." << endl;
    exit(-1);
  }

  if (NRESOURCEDIST == LATTICESIZE*LATTICESIZE){
    std::vector<float> res(NRESOURCE);
    for (int i = 0; i < LATTICESIZE; i++){
      for (int j = 0; j < LATTICESIZE; j++){
        for(int k=0; k < NRESOURCE; k++)
          res[k] = uniFLOAT(rand64);
        grid[i*LATTICESIZE+j].initialize(res, boost::dynamic_bitset<>(NSPECIEBYTES,uniIntSP(rand64)));
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
        grid[i*LATTICESIZE+j].initialize(res[idx], boost::dynamic_bitset<>(NSPECIEBYTES,uniIntSP(rand64)));
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
        grid[i*LATTICESIZE+j].initialize(res[idxX*(NRESOURCEDIST/2)+idxY], boost::dynamic_bitset<>(NSPECIEBYTES,uniIntSP(rand64)));
      }
    }
  }
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


  for(int i=0;i<LATTICESIZE;i++)
    for(int j=0;j<LATTICESIZE;j++){
      x = x_list[i];
      y = y_list[i];
      if(grid[x*LATTICESIZE+y].filed){
        if(uniFLOAT(rand64) < DEATHPROB) // death probability
          grid[x*LATTICESIZE+y].kill();
        else
          if(haveNeighbor(x, y, &x_Neigh,&y_Neigh) && uniFLOAT(rand64) < grid[x*LATTICESIZE+y].fitness){
            if (uniFLOAT(rand64) < MUTATIONPROB)
              grid[x_Neigh*LATTICESIZE+y_Neigh].fill(grid[x*LATTICESIZE+y].specie, true);
            else
              grid[x_Neigh*LATTICESIZE+y_Neigh].fill(grid[x*LATTICESIZE+y].specie, false);
          }
      }
    }
}

// Receives the grid, one site (x,y), and a pair of pointers (*x_N, *y_N) where the result will be put. Return false if there aren't empty neighboor and true if there is at least one empy neighboor. Sampling one random site if there are more than one.
bool ambient::haveNeighbor(int x, int y, int* x_N, int* y_N){
  int xP = x+1, xM = x-1, yP = y+1, yM = y-1;
  std::vector<int> PossibleX(4), PossibleY(4);
  int nPossible = 0;

  if(x == LATTICESIZE - 1)
		xP = 0;
	else if(x == 0)
		xM = LATTICESIZE-1;
	if(y == LATTICESIZE - 1)
		yP = 0;
	else if(y == 0)
		yM = LATTICESIZE-1;


  PossibleX[nPossible] = xP;
  PossibleY[nPossible] = y;
  nPossible += !grid[xP*LATTICESIZE+y].filed;

  PossibleX[nPossible] = xM;
  PossibleY[nPossible] = y;
  nPossible += !grid[xM*LATTICESIZE+y].filed;

  PossibleX[nPossible] = x;
  PossibleY[nPossible] = yP;
  nPossible += !grid[x*LATTICESIZE+yP].filed;

  PossibleX[nPossible] = x;
  PossibleY[nPossible] = yM;
  nPossible += !grid[x*LATTICESIZE+yM].filed;

  if (nPossible==0)
    return false;
  if (nPossible==1){
    *x_N = PossibleX[0];
    *y_N = PossibleY[0];
    return true;
  }
  else{
    uniIntne.param(std::uniform_int_distribution<long>::param_type(0, nPossible-1));
    int nChoosen = uniIntne(rand64);
    *x_N = PossibleX[nChoosen];
    *y_N = PossibleY[nChoosen];
    return true;
  }
}

// Count the number of observed species at the actual time, on the grid.
int ambient::countSpecie(void){
  int nDifferentS=0;
  std::vector<bool> alreadyExist(NSPECIE+1,false);

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
