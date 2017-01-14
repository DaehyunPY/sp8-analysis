#ifndef ANALYSIS_SORTRUN_H
#define ANALYSIS_SORTRUN_H

#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#else
#include <unistd.h>
#endif
#include <fstream>
#include <ctime>
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
class SortRun: public Hist {
 private:
  char id[5];
  const std::string prefix;
  std::string rootFilename;

 public:
  struct RmBunch {
    bool isOn;
    int ch;
    double region1, region2;
    bool isInTheRegion(const double bunch) const {
      if (isOn) {
        return (region1 <= bunch) && (bunch <= region2);
      } else return false;
    }
  };

 private:
  const int maxNumOfIons, maxNumOfElecs;
  int numOfIons, numOfElecs;
  bool isFileExist(const char *fileName);
  TCanvas *createCanvas(char *name, char *titel, int xposition, int yposition, int pixelsx, int pixelsy);
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
  void createHists();
 public:
  enum HistList {
    h2_ion1hitXFish, h2_ion1hitYFish, h2_ion1hitXY,
    h2_ion2hitXFish, h2_ion2hitYFish, h2_ion2hitXY,
    h2_ion3hitXFish, h2_ion3hitYFish, h2_ion3hitXY,
    h2_ion4hitXFish, h2_ion4hitYFish, h2_ion4hitXY,
    h2_ion1hit2hitPIPICO, h2_ion2hit3hitPIPICO, h2_ion3hit4hitPIPICO,
    h2_elec1hitXFish, h2_elec1hitYFish, h2_elec1hitXY,
    h2_elec2hitXFish, h2_elec2hitYFish, h2_elec2hitXY,
    h2_elec3hitXFish, h2_elec3hitYFish, h2_elec3hitXY,
    h2_elec4hitXFish, h2_elec4hitYFish, h2_elec4hitXY,
    h2_elec1hit2hitPEPECO, h2_elec2hit3hitPEPECO, h2_elec3hit4hitPEPECO,
    h1_ionTimesumU, h1_ionU, h2_ionTimesumDiffU,
    h1_ionTimesumV, h1_ionV, h2_ionTimesumDiffV,
    h1_ionTimesumW, h1_ionW, h2_ionTimesumDiffW,
    h2_ionXYRaw, h2_ionXY, h2_ionXYDev,
    h1_elecTimesumU, h1_elecU, h2_elecTimesumDiffU,
    h1_elecTimesumV, h1_elecV, h2_elecTimesumDiffV,
    h1_elecTimesumW, h1_elecW, h2_elecTimesumDiffW,
    h2_elecXYRaw, h2_elecXY, h2_elecXYDev,
    h1_eMarker,
    numHists
  };
};
}

#endif
