#include <TH1.h>
#include <TCanvas.h>
#include <TQObject.h>
#include <TROOT.h>

#include "variables.C"

void project() {
	printf("Type 'projectX()' or 'projectY()'\n"); 
}

void projectX() {
   targetObj = gPad->GetSelected(); 
   printf("Selected target object is %s\n", targetObj->GetName()); 
   if (!targetObj) { printf("Can't select!\n"); return; }
   if (!targetObj->InheritsFrom("TH2")) { printf("Select a TH2!\n"); return; }
   TPad *currentPad = (TPad *)gROOT->GetSelectedPad(); 
   if (currentPad) {
      targetCanvas = currentPad->GetCanvas(); 
      printf("Current pad is %s\n", targetCanvas->GetName()); 
   } else {
      printf("Check current pad!\n");
      return; 
   }
   targetAxis = axisX; region1 = 0; region2 = 0; 
   wasCrosshairOn = targetCanvas->GetCrosshair(); 
   targetCanvas->SetCrosshair(true); 
   targetCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 0, 0,
                          "exeProj(Int_t,Int_t,Int_t,TObject*)");
   printf("Drag region to project\n");
}

void projectY() {
   targetObj = gPad->GetSelected(); 
   printf("Selected target object is %s\n", targetObj->GetName()); 
   if (!targetObj) { printf("Can't select!\n"); return; }
   if (!targetObj->InheritsFrom("TH2")) { printf("Select a TH2!\n"); return; }
   TPad *currentPad = (TPad *)gROOT->GetSelectedPad(); 
   if (currentPad) {
      targetCanvas = currentPad->GetCanvas(); 
      printf("Current pad is %s\n", targetCanvas->GetName()); 
   } else {
      printf("Check current pad!\n");
      return; 
   }
   targetAxis = axisY; region1 = 0; region2 = 0; 
   wasCrosshairOn = targetCanvas->GetCrosshair(); 
   targetCanvas->SetCrosshair(true); 
   targetCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 0, 0,
                          "exeProj(Int_t,Int_t,Int_t,TObject*)");
   printf("Drag region to project\n");
}

void exeProj(Int_t event, Int_t x, Int_t y, TObject *selected) {
   TCanvas *c = (TCanvas *) gTQSender;
   // printf("Canvas %s: event=%d, x=%d, y=%d, selected=%s\n", c->GetName(),
   //        event, x, y, selected->IsA()->GetName()); 
   if (!((event==kButton1Down) || (event==kButton1Up))) { return; }
   if (event==kButton1Down) {
      if (targetAxis == axisX) {
         region1 = gPad->AbsPixeltoY(y);
      } else if (targetAxis == axisY) {
         region1 = gPad->AbsPixeltoX(x);
      }
      printf("region1=%f\n", region1);
   } else if (event==kButton1Up) {
      if (targetAxis == axisX) {
         region2 = gPad->AbsPixeltoY(y);
      } else if (targetAxis == axisY) {
         region2 = gPad->AbsPixeltoX(x);
      }
      printf("region2=%f\n", region2);

      TH2 *h2 = (TH2*)targetObj; 
      TH1 *h1; 
      double tmp1 = region1; 
      double tmp2 = region2;
      if(tmp1 > tmp2) {
         region1 = tmp2;
         region2 = tmp1; 
      }
      if (targetAxis == axisX) {
         h1 = h2->ProjectionX("_px", h2->GetYaxis()->FindBin(region1), h2->GetYaxis()->FindBin(region2), "");
      } else if (targetAxis == axisY) {
         h1 = h2->ProjectionY("_py", h2->GetXaxis()->FindBin(region1), h2->GetXaxis()->FindBin(region2), "");
      }
      TCanvas *c1 = new TCanvas("c1"); 
      h1->Draw(); 
      targetCanvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
      targetCanvas->SetCrosshair(wasCrosshairOn); 
   }
}