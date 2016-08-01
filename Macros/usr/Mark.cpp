//
// Created by Daehyun You on 8/1/16.
//

#include "Mark.h"
Mark::Mark(): Draw() {
  gr = new TGraph();
  gr->SetMarkerStyle(kStar);
}
Mark::~Mark() {
  if (gr) {
    draw();
    delete gr;
    gr = nullptr;
  }
}
void Mark::mkPoint(int i, double x, double y) {
  gr->SetPoint(i, x, y);
  draw();
}
void Mark::rmPoint(int i) {
  gr->RemovePoint(i);
  draw();
}
void Mark::draw() {
  gr->Draw("PSame");
}
