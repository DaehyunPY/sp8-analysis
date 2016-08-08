#ifndef ANALYSIS_OUTPUTHIST_H
#define ANALYSIS_OUTPUTHIST_H

#include <iostream>

#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TGraph.h>

#define TO_TEXT(X) #X
#define SAME_TITLE_WITH_VALNAME(X) X, TO_TEXT(X)

namespace Analysis {
class Hist {
 private:
  TH1D *pHist1;
  TH2D *pHist2;
  TH3D *pHist3;
  TFile *pRootFile;
  int arraySize;
  TObject **ppHistArray;
  const bool optionForVerbose;

 public:
  Hist(const bool verbose = false, int NbrMaxHistos = 100000);
  virtual ~Hist();

 public:
  void flushRootFile();
  void resetAll();
  void openRootFile(const TString name, const TString arg="RECREATE");
  void linkRootFile(TFile &RootFile);
  const bool isVerbose() const;

  // 1d hist
  TH1 *create1d(int id, const char *name,
                const char *titleX,
                int nXbins, double xLow, double xUp,
                const char *dir = "",
                bool alreadylocked = false);
  void fill1d(const int id,
              const double fillX,
              const double weight = 1);
  void fill1d(const int id,
              const double *pX,
              const double weight = 1);
  void plot1d(int id, int binX, double content);
  TH1 *getHist1d(int id) const;

  // 2d hist
  TH1 *create2d(int id, const char *name,
                const char *titleX, const char *titleY,
                int nXbins, double xLow, double xUp,
                int nYbins, double yLow, double yUp,
                const char *dir = "",
                bool alreadylocked = false);
  void fill2d(const int id,
              const double fillX, const double fillY,
              const double weight = 1);
  void fill2d(const int id,
              const double *pX, const double *pY,
              const double weight = 1);
  void plot2d(int id, int binX, int binY, double content);
  TH2 *getHist2d(int id) const;

  // 3d hist
  TH1 *create3d(int id, const char *name,
                const char *titleX, const char *titleY, const char *titleZ,
                int nXbins, double xLow, double xUp,
                int nYbins, double yLow, double yUp,
                int nZbins, double zLow, double zUp,
                const char *dir = "",
                bool alreadylocked = false);
  void fill3d(const int id,
              const double fillX, const double fillY, const double fillZ,
              const double weight = 1);
  void fill3d(const int id,
              const double *pX, const double *pY, const double *pZ,
              const double weight = 1);
  void plot3d(int id, int binX, int binY, int binZ, double content);
  TH3 *getHist3d(int id) const;
};
}

#endif