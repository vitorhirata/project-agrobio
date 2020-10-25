#ifndef STRUCTS_H
#define STRUCTS_H

// Variety struct contain all the proprieties of each variety
struct VarietyData{
  std::vector<float> halfSaturation;
  float quality;
};

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
