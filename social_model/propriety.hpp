class propriety{
public:
  std::vector<std::vector<int>> quantSpecie;
  std::vector<std::vector<int>> speciesChoosen;
  std::vector<float> speciesPunctuation;
  float probInovation;
  int LimN;
  int LimS;
  int LimL;
  int LimO;
  void chooseSpecie(void);
  boost::dynamic_bitset<> plantSpecie(void);
};

void propriety::chooseSpecie(void){
  int PropSize = (LimL-LimO)*(LimL-LimO);
  while(quantSpecie.size() > PropSize){
    int eraseIndex = std::min_element(speciesPunctuation.begin(), speciesPunctuation.end()) - speciesPunctuation.begin();
    quantSpecie.erase(quantSpecie.begin() + eraseIndex);
    speciesPunctuation.erase(speciesPunctuation.begin() + eraseIndex);
  }
  // At this first test it will be choosen to plant the same things that are already planted
  speciesChoosen = quantSpecie;
}

boost::dynamic_bitset<> propriety::plantSpecie(void){
  boost::dynamic_bitset<> cSp(NSPECIEBYTES, speciesChoosen[0][0]);
  if (speciesChoosen[0][1] == 1)
    speciesChoosen.erase(speciesChoosen.begin());
  else
    speciesChoosen[0][1]--;

  return cSp;
}
