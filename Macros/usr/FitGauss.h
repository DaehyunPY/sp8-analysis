//
// Created by Daehyun You on 7/31/16.
//

#ifndef ANALYSIS_FITGAUSS_H
#define ANALYSIS_FITGAUSS_H


#include <TH1.h>
#include <TH2.h>
#include "Draw.h"

class FitGauss: protected Draw {
  TObject *obj;
 public:
  FitGauss(double reg1=0, double reg2=0);
  ~FitGauss();

 private:
  void runAfterDrag();
  void fitGauss(double reg1, double reg2);
};


#endif //ANALYSIS_FITGAUSS_H
