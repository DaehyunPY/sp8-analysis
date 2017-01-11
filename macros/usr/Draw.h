//
// Created by Daehyun You on 7/30/16.
//

#ifndef ANALYSIS_DRAW_H
#define ANALYSIS_DRAW_H


#include <iostream>
#include <TROOT.h>
#include <TQObject.h>
#include <TPad.h>
#include <TCanvas.h>

class Draw {
 protected:
  Draw();
  virtual ~Draw();
  TPad *pad;
  TCanvas *cnvs;

 protected:
  double x1, y1, x2, y2;
  bool wasCrosshairOn;
  virtual void runAfterDrag();
 public:
  void dragReg(Int_t event, Int_t x, Int_t y, TObject *selected);
};


#endif //ANALYSIS_DRAG_H
