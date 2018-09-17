#ifndef PATCH_H
#define PATCH_H

// Each patch equivales to one produceble site, and contain up to one plantation an the resources present in the soil.
class Patch{
private:
  std::vector<float> m_resource;
public:
  int plantedVariety;
  float fitness;
  void computeLocalFitness(const Variety* variety);
  void setVariety(int t_var);
  void setLocalResource(std::vector<float> t_resource);
};

// Compute the fitness, using the Monod Equation, considering variety K and the resources available. Store this value
// in fitness.
void Patch::computeLocalFitness(const Variety* variety){
  float fitnessTemp = m_resource[0] / (m_resource[0] + variety[plantedVariety].K[0]);;
  for (uint i = 1; i < m_resource.size(); ++i){
    float temp = m_resource[i] / (m_resource[i] + variety[plantedVariety].K[i]);
    if(temp < fitnessTemp)
      fitnessTemp = temp;
  }
  fitness = fitnessTemp;
}

// Set the plantedVariety as var
void Patch::setVariety(int t_var){
  plantedVariety = t_var;
}

// Set the resource as t_resource
void Patch::setLocalResource(std::vector<float> t_resource){
  m_resource = t_resource;
}

#endif
