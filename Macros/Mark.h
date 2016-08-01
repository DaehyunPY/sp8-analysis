//
// Created by Daehyun You on 8/1/16.
//

#ifndef ANALYSIS_MARK_H
#define ANALYSIS_MARK_H


#include <TGraph.h>
#include "Draw.h"

class Mark: protected Draw {
  TGraph *gr;
  int n;
 public:
  Mark();
  ~Mark();
  void addPoint(double x, double y);
};


#endif //ANALYSIS_MARK_H
