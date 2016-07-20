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
#include "../resort/resort64c.h"
#include "../JSONHandler/JSONReader.h"
#include "../Protocols/Hist.h"

namespace Analysis {
class SortRun: Hist {
 private:
  char id[5];
  std::string LMFFilename = "";
  std::string rootFilename = "";
  std::string ionSorterFilename = "";
  std::string elecSorterFilename = "";
  std::string ionCalibTableFilename = "";
  std::string elecCalibTableFilename = "";
  TTree *pRootTree;
  int maxNumOfIons;
  int maxNumOfElecs;
  int numOfIons, numOfElecs;
  double eMarker;
  struct DataSet {
    double x, y, t;
    int flag;
  } *pIonDataSet, *pElecDataSet;
  bool isFileExist(const char *fileName);

 private:
  enum HistList {
    h1_eMarker,
    numHists
  };
  void createHists();
  void fillHists();

 public:
  SortRun(const std::string configFilename);
  ~SortRun();
  void branchRootTree(const int ionNum, const int elecNum);
  void processEvent(const int ionHitNum, const sort_class *pIonSorter, const int elecHitNum,
                    const sort_class *pElecSorter, const double eMkr);
  char *getLMFFilename() const;
  char *getIonSorterFilename() const;
  char *getElecSorterFilename() const;
  char *getIonCalibTableFilename() const;
  char *getElecCalibTableFilename() const;
  TCanvas *newCanvas(char *name, char *titel, int xposition, int yposition, int pixelsx, int pixelsy);
  TH1D *newTH1D(char *name, char *comment, int bins, double xmin, double xmax);
  TH2D *newTH2D(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                double ymax, char *option);
};
}

#endif
