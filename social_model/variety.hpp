class variety{
private:
  std::vector<float> K;
public:
  float taste;
  float calculateFitness(std::vector<float> resource);
  variety();
};

variety::variety(){
  for (int j = 0; j < NRESOURCE; j++)
    K.push_back(gauss(rand64));
  taste = gauss(rand64)-0.5;
}

// Return the fitness of the population living in the site. Computed using the Monod equation.
float variety::calculateFitness(std::vector<float> resource){
  float min = resource[0]/(K[NRESOURCE]+resource[0]);
  for (int i=1; i < NRESOURCE; i++){
    float temp = resource[i]/(K[i]+resource[i]);
    if (temp < min)
      min = temp;
  }
  return min;
}

variety VAR[NMAXSPECIE];
