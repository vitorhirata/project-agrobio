class propriety{
private:
  float probDistSigma(float x);
  std::vector<int> speciesChoosen;
public:
  std::vector<int> availableSpecie;
  std::vector<float> speciesPunctuation;
  std::vector<int> proprietyConnection;
  float probInovation;
  int maxSpecie;
  float maxSpeciePunc;
  int LimN;
  int LimS;
  int LimL;
  int LimO;
  void chooseSpecie(void);
  boost::dynamic_bitset<> plantSpecie(void);
  void setSpecie(std::vector<int> sp, std::vector<float> spPunc);
};

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
  for(int i = 0; i < speciesPunctuation.size(); i++)
    speciesPunctuation[i] = probDistSigma(speciesPunctuation[i]);

  int idxMax = distance(speciesPunctuation.begin(), max_element(speciesPunctuation.begin(), speciesPunctuation.end()));
  maxSpecie = availableSpecie[idxMax];
  maxSpeciePunc = speciesPunctuation[idxMax];
}

float propriety::probDistSigma(float x){
  // A = B*u/(1+u), B = 1/(u/(1+u)+(m-u)/(1+m-u)), u=2.5, m=5.
  return  0.5+0.7*(5*x-2.5)/(1+abs(5*x-2.5));
}

boost::dynamic_bitset<> propriety::plantSpecie(void){
  boost::dynamic_bitset<> cSp(NSPECIEBYTES, speciesChoosen.back());
  speciesChoosen.pop_back();
  if (uniFLOAT(rand64) < MUTATIONPROB){
    int temp = uniIntNSPB(rand64);
    cSp[temp] = !cSp[temp];
  }
  return cSp;
}
