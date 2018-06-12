class propriety{
private:
  std::vector<int> availableSpecie;
  std::vector<float> speciesPunctuation;
  int idxChoose;
public:
  std::vector<int> speciesChoosen;
  std::vector<int> proprietyConnection;
  int LimN;
  int LimS;
  int LimL;
  int LimO;
  float personalPref;
  void chooseSpecie(void);
  int plantSpecie(void);
  void setSpecie(std::vector<int> sp, std::vector<float> spPunc);
  void addSpecie(int sp, float spPunc);
  void proprietyConstruct(int xInit, int yInit);
};

void propriety::proprietyConstruct(int xInit, int yInit){
  int propSizeRadix = LATTICESIZE/sqrt(NPROPRIETY);
  LimO = xInit*propSizeRadix;
  LimL = xInit*propSizeRadix + propSizeRadix - 1;
  LimN = yInit*propSizeRadix;
  LimS = yInit*propSizeRadix + propSizeRadix - 1;
  personalPref = uniFLOAT(rand64);
}

void propriety::chooseSpecie(void){
  int PropSize = (1+LimL-LimO)*(1+LimL-LimO);
  int j;
  float n;
  float acum, sum;
  idxChoose = 0;

  if(speciesPunctuation.empty()){
    speciesChoosen = std::vector<int>(PropSize, uniIntNSP(rand64));
    cout << "ERROR: No specie" << endl;
    return;
  }
  if(availableSpecie.size() == 1){
    speciesChoosen = std::vector<int>(PropSize, availableSpecie[0]);

    return;
  }

/*
// Select only the variety with maximum punctuation
  int idxMax = distance(speciesPunctuation.begin(), std::max_element(speciesPunctuation.begin(), speciesPunctuation.end()));
  speciesChoosen = std::vector<int>(PropSize, availableSpecie[idxMax]);
*/


// Select nomalizing and choose based on that
/*
  speciesChoosen = std::vector<int>(PropSize, 0);
  sum = std::accumulate(speciesPunctuation.begin(), speciesPunctuation.end(), 0.0);
  acum = 0;
  for(int i = 0; i < speciesPunctuation.size(); i++){
    cout << speciesPunctuation[i] << ", ";
    acum += speciesPunctuation[i]/sum;
    speciesPunctuation[i] = acum;
  }
  cout << endl;
  for(int i = 0; i < PropSize; i++){
    n = uniFLOAT(rand64);
    j = 0;
    while(n > speciesPunctuation[j])
      j++;
    speciesChoosen[i] = availableSpecie[j];
    //cout << j << ", ";
  }
  //cout << endl;
*/

// Select random variety and the probability of get planted is given by the punctuation
  uniIntne.param(std::uniform_int_distribution<long>::param_type(0, speciesPunctuation.size()-1));
  speciesChoosen = std::vector<int>(PropSize, 0);

  for(int i = 0; i < PropSize; i++){
    int plantIdx = uniIntne(rand64);
    while(!(uniFLOAT(rand64) < speciesPunctuation[plantIdx])){
      plantIdx = uniIntne(rand64);
    }
    speciesChoosen[i] = availableSpecie[plantIdx];
  }

}

void propriety::setSpecie(std::vector<int> sp, std::vector<float> spPunc){
  availableSpecie = sp;
  speciesPunctuation = spPunc;
}

void propriety::addSpecie(int sp, float spPunc){
  availableSpecie.push_back(sp);
  speciesPunctuation.push_back(spPunc);
}


int propriety::plantSpecie(void){
  int sp = speciesChoosen[idxChoose];
  idxChoose++;
  if (uniFLOAT(rand64) < MUTATIONPROB){
    sp = uniIntNSP(rand64);
  }
  return sp;
}
