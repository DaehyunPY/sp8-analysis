//
// Created by Daehyun You on 8/1/16.
//

#ifndef ANALYSIS_MARK_H
#define ANALYSIS_MARK_H


#include <TGraph.h>
#include "Draw.h"

class Mark: protected Draw {
  TGraph *gr;
 public:
  Mark();
  ~Mark();
  void mkPoint(int i, double x, double y);
  void rmPoint(int i);
  void draw();
};


#endif //ANALYSIS_MARK_H
