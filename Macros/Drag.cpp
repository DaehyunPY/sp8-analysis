//
// Created by Daehyun You on 7/30/16.
//

#include "Drag.h"

Drag::Drag() {
  pad = (TPad *) gROOT->GetSelectedPad();
  cnvs = pad->GetCanvas();
  if (!(pad && cnvs)) {
    std::cout << "The pad and the canvas are invalid." << std::endl;
  }
  std::cout << "Pad: " << pad->GetName() << std::endl;
  std::cout << "Canvas: " << cnvs->GetName() << std::endl;

  wasCrosshairOn = (bool) pad->GetCrosshair();
}
Drag::~Drag() {
  if (pad) pad = nullptr;
  if (cnvs) cnvs = nullptr;
}

void Drag::dragReg(Int_t event, Int_t x, Int_t y, TObject *selected) {
  if (!((event == kButton1Down) || (event == kButton1Up))) return;
  if (!(pad && cnvs)) return;
  if (event == kButton1Down) pad->AbsPixeltoXY(x, y, x1, y1);
  else { // event == kButton1Up
    pad->AbsPixeltoXY(x, y, x2, y2);
    cnvs->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", this, "dragReg(Int_t,Int_t,Int_t,TObject*)");
    cnvs->SetCrosshair(wasCrosshairOn);
    runAfterDrag();
  }
}
void Drag::runAfterDrag() {
}
