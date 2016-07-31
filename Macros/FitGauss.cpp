//
// Created by Daehyun You on 7/31/16.
//

#include "FitGauss.h"

FitGauss::FitGauss(double reg1, double reg2) {
  obj = pad->GetSelected();
  if (!obj) {
    std::cout << "The object is invalid." << std::endl;
    delete this;
    return;
  }
  std::cout << "Object: " << obj->GetName() << std::endl;

  if (!obj->InheritsFrom("TH1") || obj->InheritsFrom("TH2") || obj->InheritsFrom("TH3")) {
    std::cout << "Select a TH1!" << std::endl;
    delete this;
    return;
  }

  if (reg1==0 && reg2==0) {
    cnvs->SetCrosshair(true);
    cnvs->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)",
                  "Drag", this, "dragReg(Int_t,Int_t,Int_t,TObject*)");
    std::cout << "Drag the region to fit a Gaussian function." << std::endl;
  } else fitGauss(reg1, reg2);
}
FitGauss::~FitGauss() {
  if (obj) obj = nullptr;
}
void FitGauss::runAfterDrag() {
  fitGauss(x1, x2);
}
void FitGauss::fitGauss(double reg1, double reg2) {
  if (reg1 > reg2) {
    double tmp = reg1;
    reg1 = reg2;
    reg2 = tmp;
  }
  std::cout << "region = [" << reg1 << ", " << reg2 << "]" << std::endl;

  TH1 *h1 = (TH2 *) obj;
  h1->Fit("gaus", "w", "", reg1, reg2);
  h1->Draw("same");
  cnvs->Modified();
  cnvs->Update();
  delete this;
}


