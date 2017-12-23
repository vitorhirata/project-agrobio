// Each patch equivales to one produceble site, and contain up to one plantation an the resources present in the soil.
class patch{
private:
  std::vector<float> resource; // each number is the amount of the i'th resource
  double calculateFitness();
public:
  bool filed;
  boost::dynamic_bitset<> specie; // binary array
  double fitness;
  void initialize(std::vector<float> res, boost::dynamic_bitset<> sp);
  void kill(void);
  void fill(boost::dynamic_bitset<> newSpecie, bool withMutation);
};

// Constructor, initialize resouce with n-sized vector and uniform distribution [0,1], and specie as an int [0,k], convert to an array of binary (size nK bytes).
void patch::initialize(std::vector<float> res, boost::dynamic_bitset<> sp){
  resource = res;
  specie = sp;
  filed = true;
  fitness = calculateFitness();
}

// Return the fitness of the population living in the site. Computed using the Monod equation.s
double patch::calculateFitness(void){
  double min = resource[0]/(K[(specie.to_ulong()-1)*n]+resource[0]);
  for (int i=1;i<n;i++){
    double temp = resource[i]/(K[(specie.to_ulong()-1)*n+i]+resource[i]);
    if (temp < min)
      min = temp;
  }
  return min;
}

// Uses XOR to make specie an empty array of binaries. No output.
void patch::kill(void){
  specie ^= specie;
  filed = false;
  fitness = 0.0;
}

// Fill the patch with the given specie, if withMutation cause a mutation
void patch::fill(boost::dynamic_bitset<> newSpecie, bool withMutation){
  if (withMutation){
    int temp = uniIntnK(rand64);
    newSpecie[temp] = !newSpecie[temp];
  }
  specie = newSpecie;
  filed = specie.any();
  fitness = calculateFitness();
}
