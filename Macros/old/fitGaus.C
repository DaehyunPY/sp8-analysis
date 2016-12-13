#include <TH1.h>
#include <TCanvas.h>
#include <TQObject.h>
#include <TROOT.h>

#include "variables.C"

void fitGaus() {
   targetObj = gPad->GetSelected(); 
   printf("Selected target object is %s\n", targetObj->GetName()); 
   if (!targetObj) { printf("Can't select!\n"); return; }
   if (!targetObj->InheritsFrom("TH1")) { printf("Select a TH1!\n"); return; }
   TPad *targetPad = (TPad *)gROOT->GetSelectedPad(); 
   if (targetPad) {
      targetCanvas = targetPad->GetCanvas(); 
      printf("Current pad is %s\n", targetCanvas->GetName()); 
      region1 = 0; region2 = 0; 
   } else {
      printf("Check current pad!\n");
   }
   wasCrosshairOn = targetCanvas->GetCrosshair(); 
   targetCanvas->SetCrosshair(true); 
   targetCanvas->Connect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)", 0, 0,
                          "exeFitGaus(Int_t,Int_t,Int_t,TObject*)");
   printf("Drag region to fit\n");
}

void exeFitGaus(Int_t event, Int_t x, Int_t y, TObject *selected) {
   TCanvas *c = (TCanvas *) gTQSender;
   // printf("Canvas %s: event=%d, x=%d, y=%d, selected=%s\n", c->GetName(),
   //        event, x, y, selected->IsA()->GetName()); 
   if (!((event==kButton1Down) || (event==kButton1Up))) { return; }
   if (event==kButton1Down) {
      region1 = gPad->AbsPixeltoX(x);
      printf("region1=%f\n", region1);
   } else if (event==kButton1Up) {
      region2 = gPad->AbsPixeltoX(x);
      printf("region2=%f\n", region2);

      double tmp1 = region1; 
      double tmp2 = region2;
      if(tmp1 > tmp2) {
         region1 = tmp2;
         region2 = tmp1; 
      }
      TH1 *h = (TH2*)targetObj; 
      h->Fit("gaus", "w", "", region1, region2); 
      targetCanvas->Disconnect("ProcessedEvent(Int_t,Int_t,Int_t,TObject*)");
      targetCanvas->SetCrosshair(wasCrosshairOn); 
   }
}