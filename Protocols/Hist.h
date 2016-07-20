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

namespace Analysis {
class OutputHist {
 private:
  TH1D *pHist1; // pointer to a 1d Histogram
  TH2D *pHist2; // pointer to a 2d Histogram
  TH3D *pHist3; // pointer to a 3d Histogram
  TFile *pRootFile; // pointer to the Rootfile, these Histograms get Stored in
  int arraySize; // the size of the array containing the histograms
  TObject **ppHistArray; // array Containing the histograms
  const bool optionForVerbose; // a flag that tells wether this should do output

 public:
  OutputHist(const bool v, int NbrMaxHistos = 100000);
  ~OutputHist();

  void flushRootFile();
  void resetAll();
  void openRootFile(const TString &RootFileName);
  void linkRootFile(TFile &RootFile);
  const bool isVerbose() const;

  void fill(int id, const char *name,
            double fillX, double fillY, double fillZ,
            const char *titleX, const char *titleY, const char *titleZ,
            int nXbins, double xLow, double xUp,
            int nYbins, double yLow, double yUp,
            int nZbins, double zLow, double zUp,
            const char *dir = "",
            double weight = 1); // fill function for 3d histos
  TH1 *create3d(int id, const char *name,
                const char *titleX, const char *titleY, const char *titleZ,
                int nXbins, double xLow, double xUp,
                int nYbins, double yLow, double yUp,
                int nZbins, double zLow, double zUp,
                const char *dir = "",
                bool alreadylocked = false); // create 3d histos
  void fill3d(const int id,
              const double fillX, const double fillY, const double fillZ,
              const double weight = 1); // fill fast 3d histos
  void fill3d(const int id,
              const double *pX, const double *pY, const double *pZ,
              const double weight = 1); // fill fast 3d histos
  void plot3d(int id, int binX, int binY, int binZ, double content);
  TH3 *getHist3d(int id) const;

  void fill(int id, const char *name,
            double fillX, double fillY,
            const char *titleX, const char *titleY,
            int nXbins, double xLow, double xUp,
            int nYbins, double yLow, double yUp,
            const char *dir = "",
            double weight = 1); // fill function for 2d histos
  TH1 *create2d(int id, const char *name,
                const char *titleX, const char *titleY,
                int nXbins, double xLow, double xUp,
                int nYbins, double yLow, double yUp,
                const char *dir = "",
                bool alreadylocked = false); // create 2d histos
  void fill2d(const int id,
              const double fillX, const double fillY,
              const double weight = 1); // fill fast 2d histos
  void fill2d(const int id,
              const double *pX, const double *pY,
              const double weight = 1); // fill fast 2d histos
  void plot2d(int id, int binX, int binY, double content); // set bin contents
  TH2 *getHist2d(int id) const;

  void fill(int id, const char *name,
            double fillX,
            const char *titleX,
            int nXbins, double xLow, double xUp,
            const char *dir = "",
            double weight = 1); // fill function for 1d histos
  TH1 *create1d(int id, const char *name,
                const char *titleX,
                int nXbins, double xLow, double xUp,
                const char *dir = "",
                bool alreadylocked = false); // create 1d histos
  void fill1d(const int id,
              const double fillX,
              const double weight = 1); // fill fast 1d histos
  void fill1d(const int id,
              const double *pX,
              const double weight = 1);
  void plot1d(int id, int binX, double content); // set bin contents
  TH1 *getHist1d(int id) const;
};
}

#endif