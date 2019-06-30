#ifndef VARIETY_H
#define VARIETY_H

class Variety{
public:
  float appearence;
  std::vector<float> halfSaturation;
  int varietyNumber;
  int computeVarietyNumber(void);
  void setRandomVariety(void);
  void setVariety(VarietyData t_data);
  void killVariety(void);
  };

// Set variety with a randomly choosen variety
void Variety::setRandomVariety(void){
  for(uint i = 0; i < halfSaturation.size(); ++i){
    halfSaturation[i] = gaussK(rand64);
    while(!(halfSaturation[i] > 0))
      halfSaturation[i] = gaussK(rand64);
  }
  appearence = uniFLOAT(rand64);
  varietyNumber = computeVarietyNumber();
}

void Variety::killVariety(void){
  varietyNumber = -1;
  appearence = -1;
}

// Set variety to a new one, according to the received value
void Variety::setVariety(VarietyData t_data){
  halfSaturation = t_data.halfSaturation;
  appearence = t_data.appearence;
  varietyNumber = computeVarietyNumber();
}

// Compute the number of the variety, according to the data contained in
// the variety
int Variety::computeVarietyNumber(void){
  int varNumber = 0;
  float step = 0.2;
  int scale = 1 / step;
  int temp;
  int actualScale = scale*scale;
  for(auto i : halfSaturation){
    temp = floor(i / step);
    varNumber += actualScale * temp;
    actualScale *= scale;
  }
  varNumber += floor(appearence / (step * step));

  return varNumber;
}

#endif
