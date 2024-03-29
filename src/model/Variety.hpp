#ifndef VARIETY_H
#define VARIETY_H

/*
Class that defines a variety. It is defined by its half-saturation constants
and quality.
*/

class Variety{
public:
  float quality;
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
    while(halfSaturation[i] < 0 || halfSaturation[i] > 1)
      halfSaturation[i] = gaussK(rand64);
  }
  quality = gaussQ(rand64);
  while(quality < 0 || quality  > 1)
    quality = gaussQ(rand64);
  varietyNumber = computeVarietyNumber();
}

void Variety::killVariety(void){
  varietyNumber = -1;
  quality = -1;
}

// Set variety to a new one, according to the received value
void Variety::setVariety(VarietyData t_data){
  halfSaturation = t_data.halfSaturation;
  quality = t_data.quality;
  varietyNumber = computeVarietyNumber();
}

// Compute the number of the variety, according to the data contained in
// the variety
int Variety::computeVarietyNumber(void){
  int varNumber = 0;
  float step = 0.1;
  int scale = pow(1 / step, 2);
  int temp;
  int currentScale = scale;

  varNumber += floor(quality / (step / 2));

  for(auto i : halfSaturation){
    temp = floor(i / step);
    varNumber += currentScale * temp;
    currentScale *= scale;
  }

  return varNumber;
}

#endif
