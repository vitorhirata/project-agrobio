#ifndef STRUCTS_H
#define STRUCTS_H

/*
Defines two smaller model's structs.
*/

// Struct containing proprieties of a variety
struct VarietyData{
  std::vector<float> halfSaturation;
  float quality;
};

// Struct with a set of parameters that need to be saved on the household.
struct HDParameter{
  float alpha;
  float probabilityNewVar;
  float percentageNewRandomVar;
  float outsideTradeLimit;
  int selectionStrength;
  HDParameter(){}
  HDParameter(Parameter parameter)
  : alpha(parameter.alpha)
  , probabilityNewVar(parameter.probabilityNewVar)
  , percentageNewRandomVar(parameter.percentageNewRandomVar)
  , outsideTradeLimit(parameter.outsideTradeLimit)
  , selectionStrength(parameter.selectionStrength) {}
};
#endif
