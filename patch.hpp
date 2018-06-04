// Each patch equivales to one produceble site, and contain up to one plantation an the resources present in the soil.
class patch{
private:
  std::vector<float> resource; // each number is the amount of the i'th resource
public:
  bool filed;
  int specie; // binary array
  void initializePt(std::vector<float> res, int sp);
  float Fitness(void);
  void kill(void);
  void fill(int newSpecie);
};

// Constructor, initialize resouce with n-sized vector and uniform distribution [0,1], and specie as an int [0,k], convert to an array of binary (size nK bytes).
void patch::initializePt(std::vector<float> res, int sp){
  resource = res;
  specie = sp;
  filed = true;
}

float patch::Fitness(void){
  float value;
  if (specie == 0)
    return 0;
  value = VAR[specie-1].calculateFitness(resource);
  return value;
}

// Uses XOR to make specie an empty array of binaries. No output.
void patch::kill(void){
  specie = 0;
  filed = false;
}

// Fill the patch with the given specie, if withMutation cause a mutation
void patch::fill(int newSpecie){
  specie = newSpecie;
  filed = true;
}
