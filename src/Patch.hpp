#ifndef PATCH_H
#define PATCH_H

// Each patch equivales to one produceble site, and contain up to one
// plantation an the resources present in the soil.
class Patch{
private:
  std::vector<float> m_resource;
  void setLocalResource(std::vector<float> t_resource);
  void computeProductivity(void);
public:
  float productivity;
  Variety variety;
  void initializePatch(std::vector<float> t_resource, VarietyData t_data);
  void setVariety(VarietyData t_data);
  VarietyData giveVarietyData(void);
  void setRandomVariety(void);
  void killVariety(void);
};

// Initialize the patch, setting up the resources and variety according to
// received values
void Patch::initializePatch(std::vector<float> t_resource, VarietyData t_data){
  setLocalResource(t_resource);
  setVariety(t_data);
  computeProductivity();
}


// Compute the productivity, using the Monod Equation, considering variety K
// and the resources available. Store this valuel in productivity.
void Patch::computeProductivity(void){
  if(variety.varietyNumber == -1){
    productivity = 0;
    return;
  }
  productivity = m_resource[0] / (m_resource[0] + variety.halfSaturation[0]);
  for (uint i = 1; i < m_resource.size(); ++i){
    float temp = m_resource[i] / (m_resource[i] + variety.halfSaturation[i]);
    if(temp < productivity)
      productivity = temp;
  }
}

// Substitute actual variety by a randomly choosen variety
void Patch::setRandomVariety(void){
  variety.setRandomVariety();
  computeProductivity();
}

// Kill the variety in the patch
void Patch::killVariety(void){
  variety.killVariety();
  productivity = 0;
}

// Substitute actual variety by variety with the received data
void Patch::setVariety(VarietyData t_data){
  variety.setVariety(t_data);
  computeProductivity();
}

// Return the data about the variety in the patch
VarietyData Patch::giveVarietyData(void){
  VarietyData data;
  data.halfSaturation = variety.halfSaturation;
  data.quality = variety.quality;
  return data;
}

// Set the resource as t_resource
void Patch::setLocalResource(std::vector<float> t_resource){
  m_resource = t_resource;
}

#endif
