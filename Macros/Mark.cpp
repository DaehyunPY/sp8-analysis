//
// Created by Daehyun You on 8/1/16.
//

#include "Mark.h"
Mark::Mark(): Draw() {
  n = 0;
  gr = new TGraph();
  gr->SetMarkerStyle(kStar);
}
Mark::~Mark() {
  if (gr) {
    gr->Draw("PSame");
    delete gr;
    gr = nullptr;
  }
}
void Mark::addPoint(double x, double y) {
  gr->SetPoint(n, x, y);
  n += 1;
  gr->Draw("PSame");
}
