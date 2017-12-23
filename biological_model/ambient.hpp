class ambient{
private:
  patch* grid;
  bool haveNeighbor(int x, int y, int* x_N, int* y_N);
public:
  void iterate(void);
  int countSpecie(void);
  ambient(int nRes);
};

ambient::ambient(int nRes){
  grid = new (nothrow) patch[L*L];
  if (grid == nullptr){
    cout << "Error: grid alocation not successful." << endl;
    exit(-1);
  }
  if (L % nRes != 0 && L*L % nRes != 0){
    cout << "Error: only accepted number of resources that are divisible by L or L*L." << endl;
    exit(-1);
  }

  if (nRes == L*L){
    std::vector<float> res(n);
    for (int i = 0; i < L; i++){
      for (int j = 0; j < L; j++){
        for(int k=0;k<n;k++)
          res[k] = uniFLOAT(rand64);
        grid[i*L+j].initialize(res, boost::dynamic_bitset<>(nK,uniIntk(rand64)));
      }
    }
  }
  else if(L % nRes == 0){
    vector<vector<float> > res(nRes, vector<float>(n));
    for (int i=0; i < nRes; i++)
      for (int j=0; j < n; j++)
        res[i][j] = uniFLOAT(rand64);

    int idx;
    for (int i = 0; i < L; i++){
      idx = i / (L/nRes);
      for (int j = 0; j < L; j++)
        grid[i*L+j].initialize(res[idx], boost::dynamic_bitset<>(nK,uniIntk(rand64)));
    }
  }
  else if (L*L % nRes == 0){
    vector<vector<float> > res(nRes, vector<float>(n));
    for (int i=0; i < nRes; i++)
      for (int j=0; j < n; j++)
        res[i][j] = uniFLOAT(rand64);

    int idxX, idxY;
    for (int i = 0; i < L; i++){
      idxX = i / (L/(nRes/2));
      for (int j = 0; j < L; j++){
        idxY = j / (L/(nRes/2));
        grid[i*L+j].initialize(res[idxX*(nRes/2)+idxY], boost::dynamic_bitset<>(nK,uniIntk(rand64)));
      }
    }
  }
}

// Receives the grid and iterate one time, passing over all the sites of the grid.
void ambient::iterate(void){
  int x, y, x_Neigh, y_Neigh;
  std::vector<int> x_list(L), y_list(L);

  // Initialize the list over which the program will pass updating the program.
  for(int i=0;i<L;i++)
    x_list[i]=y_list[i]=i;
  std::random_shuffle(x_list.begin(),x_list.end());
  std::random_shuffle(y_list.begin(),y_list.end());


  for(int i=0;i<L;i++)
    for(int j=0;j<L;j++){
      x = x_list[i];
      y = y_list[i];
      if(grid[x*L+y].filed){
        if(uniFLOAT(rand64) < u) // death probability
          grid[x*L+y].kill();
        else
          if(haveNeighbor(x, y, &x_Neigh,&y_Neigh) && uniFLOAT(rand64) < grid[x*L+y].fitness){
            if (uniFLOAT(rand64) < m)
              grid[x_Neigh*L+y_Neigh].fill(grid[x*L+y].specie, true);
            else
              grid[x_Neigh*L+y_Neigh].fill(grid[x*L+y].specie, false);
          }
      }
    }
}

// Receives the grid, one site (x,y), and a pair of pointers (*x_N, *y_N) where the result will be put. Return false if there aren't empty neighboor and true if there is at least one empy neighboor. Sampling one random site if there are more than one.
bool ambient::haveNeighbor(int x, int y, int* x_N, int* y_N){
  int xP = x+1, xM = x-1, yP = y+1, yM = y-1;
  std::vector<int> PossibleX(4), PossibleY(4);
  int nPossible = 0;

  if(x == L - 1)
		xP = 0;
	else if(x == 0)
		xM = L-1;
	if(y == L - 1)
		yP = 0;
	else if(y == 0)
		yM = L-1;


  PossibleX[nPossible] = xP;
  PossibleY[nPossible] = y;
  nPossible += !grid[xP*L+y].filed;

  PossibleX[nPossible] = xM;
  PossibleY[nPossible] = y;
  nPossible += !grid[xM*L+y].filed;

  PossibleX[nPossible] = x;
  PossibleY[nPossible] = yP;
  nPossible += !grid[x*L+yP].filed;

  PossibleX[nPossible] = x;
  PossibleY[nPossible] = yM;
  nPossible += !grid[x*L+yM].filed;

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

// Cont the number of observed species at the actual time, on the grid.
int ambient::countSpecie(void){
  int nDifferentS=0;
  std::vector<bool> alreadyExist(k+1,false);

  for(int i=0;i<L;i++)
    for(int j=0;j<L;j++){
      int sp=grid[i*L+j].specie.to_ulong();
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
