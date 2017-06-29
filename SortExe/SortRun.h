#ifndef ANALYSIS_SORTRUN_H
#define ANALYSIS_SORTRUN_H

#include <unistd.h>
#include <string>
#include <map>
#include <fstream>
#include <ctime>
#include <math.h>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TColor.h>
#include <TStyle.h>
#include <TF1.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH2F.h>
#include <TH3D.h>
#include <TChain.h>
#include <RooDataProjBinding.h>
#include "resort64c.h"
#include "../Core/Hist.h"
#include "../Core/JSONReader.h"

namespace Analysis {
template <typename T>
struct Region {
  T fr, to;
  bool isIn(T value) const {
    return (fr <= value) && (value <= to);
  }
};

template <typename T>
struct Regions {
  std::vector<Region<T>> regions;
  bool isIn(const T value) const {
    for(auto r : regions) if(r.isIn(value)) return true;
    return false;
  }
  bool isIn(const T *pV) const {
    if (pV == nullptr) return true;
    return isIn(*pV);
  }
};

Regions<double> readBunchMaskRm(const Analysis::JSONReader &reader, const std::string prefix);

class SortRun: public Hist {
 private:
  char id[5];
  const std::string prefix;
  std::string rootFilename;

 private:
  const int maxNumOfIons, maxNumOfElecs;
  int numOfIons, numOfElecs;
  bool isFileExist(const char *fileName);
  TCanvas *createCanvas(std::string name, std::string titel, int xposition, int yposition, int pixelsx, int pixelsy);
 public:
  SortRun(const std::string pref, const int iNum, const int eNum);
  ~SortRun();


 private:
  TCanvas *pC1 = nullptr, *pC2 = nullptr;
  const bool existC1() const;
  const bool existC2() const;
  void closeC1();
  void closeC2();
 public:
  void createC1();
  void updateC1();
  void updateC1(const bool mdf);
  void createC2();
  void updateC2();
  void updateC2(const bool mdf);

 private:
  TTree *pRootTree = nullptr;
  const bool existTree() const;
  void createTree();
  void closeTree();
 public:
  struct DataSet { double x, y, t; int flag; } *pIonDataSet, *pElecDataSet;
  void fillTree(const int ionHitNum, const DataSet *pIon,
                const int elecHitNum, const DataSet *pElec);
 private:
  const DataSet dumpData = { NAN, NAN, NAN, -1 };

 private:
  void createHists();
 public:
  enum HistList {
    h1_timestamp,
    h1_TDC01, h1_TDC02, h1_TDC03, h1_TDC04, h1_TDC05, h1_TDC06, h1_TDC07, h1_TDC08,
    h1_TDC09, h1_TDC10, h1_TDC11, h1_TDC12, h1_TDC13, h1_TDC14, h1_TDC15, h1_TDC16,
    h1_ionTimesumU_beforeSort, h1_ionTimediffU_beforeSort, h2_ionTimesumDiffU_beforeSort,
    h1_ionTimesumV_beforeSort, h1_ionTimediffV_beforeSort, h2_ionTimesumDiffV_beforeSort,
    h1_ionTimesumW_beforeSort, h1_ionTimediffW_beforeSort, h2_ionTimesumDiffW_beforeSort,
    h1_ionTimesumU_afterSort, h1_ionTimediffU_afterSort, h2_ionTimesumDiffU_afterSort,
    h1_ionTimesumV_afterSort, h1_ionTimediffV_afterSort, h2_ionTimesumDiffV_afterSort,
    h1_ionTimesumW_afterSort, h1_ionTimediffW_afterSort, h2_ionTimesumDiffW_afterSort,
    h2_ionXYRaw, h2_ionXY, h2_ionXYDev,
    h1_elecTimesumU_beforeSort, h1_elecTimediffU_beforeSort, h2_elecTimesumDiffU_beforeSort,
    h1_elecTimesumV_beforeSort, h1_elecTimediffV_beforeSort, h2_elecTimesumDiffV_beforeSort,
    h1_elecTimesumW_beforeSort, h1_elecTimediffW_beforeSort, h2_elecTimesumDiffW_beforeSort,
    h1_elecTimesumU_afterSort, h1_elecTimediffU_afterSort, h2_elecTimesumDiffU_afterSort,
    h1_elecTimesumV_afterSort, h1_elecTimediffV_afterSort, h2_elecTimesumDiffV_afterSort,
    h1_elecTimesumW_afterSort, h1_elecTimediffW_afterSort, h2_elecTimesumDiffW_afterSort,
    h2_elecXYRaw, h2_elecXY, h2_elecXYDev,
    h1_bunchMarker_beforeRm, h1_bunchMarker_afterRm,
    h2_ion1hitXFish, h2_ion1hitYFish, h2_ion1hitXY,
    h2_ion2hitXFish, h2_ion2hitYFish, h2_ion2hitXY,
    h2_ion3hitXFish, h2_ion3hitYFish, h2_ion3hitXY,
    h2_ion4hitXFish, h2_ion4hitYFish, h2_ion4hitXY,
    h2_ion5hitXFish, h2_ion5hitYFish, h2_ion5hitXY,
    h2_ion6hitXFish, h2_ion6hitYFish, h2_ion6hitXY,
    h2_ion7hitXFish, h2_ion7hitYFish, h2_ion7hitXY,
    h2_ion8hitXFish, h2_ion8hitYFish, h2_ion8hitXY,
    h2_ion1hit2hitPIPICO, h2_ion2hit3hitPIPICO, h2_ion3hit4hitPIPICO,
    h2_ion4hit5hitPIPICO, h2_ion5hit6hitPIPICO, h2_ion6hit7hitPIPICO, h2_ion7hit8hitPIPICO,
    h2_elec1hitXFish, h2_elec1hitYFish, h2_elec1hitXY,
    h2_elec2hitXFish, h2_elec2hitYFish, h2_elec2hitXY,
    h2_elec3hitXFish, h2_elec3hitYFish, h2_elec3hitXY,
    h2_elec4hitXFish, h2_elec4hitYFish, h2_elec4hitXY,
    h2_elec5hitXFish, h2_elec5hitYFish, h2_elec5hitXY,
    h2_elec6hitXFish, h2_elec6hitYFish, h2_elec6hitXY,
    h2_elec7hitXFish, h2_elec7hitYFish, h2_elec7hitXY,
    h2_elec8hitXFish, h2_elec8hitYFish, h2_elec8hitXY,
    h2_elec1hit2hitPEPECO, h2_elec2hit3hitPEPECO, h2_elec3hit4hitPEPECO,
    h2_elec4hit5hitPEPECO, h2_elec5hit6hitPEPECO, h2_elec6hit7hitPEPECO, h2_elec7hit8hitPEPECO,
    numHists
  };
};
}

#endif
