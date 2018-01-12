// Each patch equivales to one produceble site, and contain up to one plantation an the resources present in the soil.
class patch{
private:
  std::vector<float> resource; // each number is the amount of the i'th resource
public:
  bool filed;
  boost::dynamic_bitset<> specie; // binary array
  void initializePt(std::vector<float> res, boost::dynamic_bitset<> sp);
  float punctuation(void);
  void kill(void);
  void fill(boost::dynamic_bitset<> newSpecie);
};

// Constructor, initialize resouce with n-sized vector and uniform distribution [0,1], and specie as an int [0,k], convert to an array of binary (size nK bytes).
void patch::initializePt(std::vector<float> res, boost::dynamic_bitset<> sp){
  resource = res;
  specie = sp;
  filed = true;
}

float patch::punctuation(void){
  float value;
  if (filed == false)
    return 0;
  int sp = specie.to_ulong();
  value = 0.5 * VAR[sp].taste + 0.5 * VAR[sp].calculateFitness(resource);
  return value;
}

// Uses XOR to make specie an empty array of binaries. No output.
void patch::kill(void){
  specie ^= specie;
  filed = false;
}

// Fill the patch with the given specie, if withMutation cause a mutation
void patch::fill(boost::dynamic_bitset<> newSpecie){
  specie = newSpecie;
  filed = specie.any();
}
