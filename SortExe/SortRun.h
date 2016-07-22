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
#include "../Protocols/Hist.h"
#include "../Protocols/JSONReader.h"

namespace Analysis {
class SortRun: public Hist {
 private:
  char id[5];
  const std::string prefix;
  std::string rootFilename;

 private:
  bool isFileExist(const char *fileName);

 private:
	 const int maxNumOfIons, maxNumOfElecs;
  int numOfIons, numOfElecs;
  double eMarker;
  struct DataSet {
    double x, y, t;
    int flag;
  } *pIonDataSet, *pElecDataSet;

private:
	TCanvas *pC1=nullptr, *pC2=nullptr;
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
  TTree *pRootTree=nullptr;
  const bool existTree() const;
  void createTree();
  void fillTree();
  void closeTree();
  void fillHists();
  void createHists();
public:
  enum HistList {
	  h1_ionTimesumU, h1_ionTimesumV, h1_ionTimesumW, 
	  h1_ionU, h1_ionV, h1_ionW, 
	  h2_ionXYRaw, h2_ionXY, h2_ionXYDev,
	  h1_elecTimesumU, h1_elecTimesumV, h1_elecTimesumW, 
	  h1_elecU, h1_elecV, h1_elecW,
	  h2_elecXYRaw, h2_elecXY, h2_elecXYDev,
    h1_eMarker,
    numHists
  };
  void processEvent(const int ionHitNum,
                    const sort_class *pIonSorter,
                    const int elecHitNum,
                    const sort_class *pElecSorter,
                    const double eMkr);

 public:
  SortRun(const std::string pref, const int iNum, const int eNum);
  ~SortRun();
  TCanvas *createCanvas(char *name, char *titel, int xposition, int yposition, int pixelsx, int pixelsy);
};
}

#endif
