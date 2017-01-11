//
// Created by Daehyun You on 7/30/16.
//

#include "Proj.h"

Proj::Proj(AxisType tp, double reg1, double reg2) : Draw(), type(tp) {
  obj = pad->GetSelected();
  if (!obj) {
    std::cout << "The object is invalid." << std::endl;
    delete this; 
    return;
  }
  std::cout << "Object: " << obj->GetName() << std::endl;

  if (!obj->InheritsFrom("TH2")) {
    std::cout << "Select a TH2!" << std::endl;
    delete this;
    return;
  }
  if (!(tp == x || tp == y)) {
    std::cout << "Select valid axis type!" << std::endl;
    delete this;
    return;
  }

  if (reg1==0 && reg2==0) {
    cnvs->SetCrosshair(true);
    cnvs->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
                  "Draw", this, "dragReg(Int_t,Int_t,Int_t,TObject*)");
    if (type == x) std::cout << "Draw the y region to project." << std::endl;
    else std::cout << "Drag the x region to project." << std::endl; // type == y
  } else project(reg1, reg2);
}
Proj::~Proj() {
  if (obj) obj = nullptr;
  if (newHist) newHist = nullptr;
  if (newCnvs) newCnvs = nullptr;
}
void Proj::runAfterDrag() {
  double reg1, reg2;
  if (type == x) {
    reg1 = y1;
    reg2 = y2;
  } else { // type == y
    reg1 = x1;
    reg2 = x2;
  }
  project(reg1, reg2);
}
void Proj::project(double reg1, double reg2) {
  if (reg1 > reg2) {
    double tmp = reg1;
    reg1 = reg2;
    reg2 = tmp;
  }
  std::cout << "region = [" << reg1 << ", " << reg2 << "]" << std::endl;

  TH2 *h2 = (TH2 *) obj;
  if (type == x) newHist = h2->ProjectionX("_px", h2->GetYaxis()->FindBin(reg1), h2->GetYaxis()->FindBin(reg2), "");
  else newHist = h2->ProjectionY("_py", h2->GetXaxis()->FindBin(reg1), h2->GetXaxis()->FindBin(reg2), ""); // type == y
  newCnvs = new TCanvas("project");
  newCnvs->cd();
  newHist->Draw();
  newCnvs->Modified();
  newCnvs->Update();
  pad->cd();
  delete this;
}
