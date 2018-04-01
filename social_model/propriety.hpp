class propriety{
private:
  float probDistSigma(float x);
  std::vector<int> speciesChoosen;
  std::vector<int> availableSpecie;
  std::vector<float> speciesPunctuation;
public:
  std::vector<int> proprietyConnection;

  int LimN;
  int LimS;
  int LimL;
  int LimO;
  float probInovation;
  float personalPref;
  float weight[4];
  int maxSpecie;
  float maxSpeciePunc;
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
  probInovation = gauss(rand64) - 0.5;
  personalPref = uniFLOAT(rand64);
  weight[0] = gauss(rand64)-0.75;
  weight[1] = gauss(rand64)-0.75;
  weight[2] = gauss(rand64)-0.75;
  weight[3] = 1 - weight[0] - weight[1] - weight[2];
}

void propriety::chooseSpecie(void){
  int PropSize = (1+LimL-LimO)*(1+LimL-LimO);

  if(speciesPunctuation.empty()){
    speciesChoosen.clear();
    return;
  }
  if(availableSpecie.size() == 1){
    speciesChoosen = std::vector<int>(PropSize, availableSpecie[0]);
    return;
  }

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

  int idxMax = distance(speciesPunctuation.begin(), std::max_element(speciesPunctuation.begin(), speciesPunctuation.end()));
  maxSpecie = availableSpecie[idxMax];
  maxSpeciePunc = speciesPunctuation[idxMax];
}

void propriety::addSpecie(int sp, float spPunc){
  availableSpecie.push_back(sp);
  speciesPunctuation.push_back(spPunc);
}


int propriety::plantSpecie(void){
  int sp = speciesChoosen.back();
  speciesChoosen.pop_back();
  if (uniFLOAT(rand64) < MUTATIONPROB){
    sp = uniIntNSP(rand64);
  }
  return sp;
}
