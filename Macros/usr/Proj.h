//
// Created by Daehyun You on 7/30/16.
//

#ifndef ANALYSIS_PROJ_H
#define ANALYSIS_PROJ_H


#include <TH2.h>
#include "Draw.h"

class Proj: protected Draw {
  TObject *obj;
 public:
  enum AxisType {
    none, x, y
  };
  Proj(AxisType tp=none, double reg1=0, double reg2=0);
  ~Proj();
 private:
  const AxisType type;

 private:
  TH1 *newHist;
  TCanvas *newCnvs;
  void runAfterDrag();
  void project(double reg1, double reg2);
};


#endif //ANALYSIS_PROJ_H
