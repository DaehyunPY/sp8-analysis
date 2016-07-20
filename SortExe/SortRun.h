#ifndef ANALYSIS_SORTRUN_H
#define ANALYSIS_SORTRUN_H

#include <fstream>
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

#include "../resort/resort64c.h"

namespace Analysis {
    class SortRun {
    private:
        char id[5];
        std::string LMFFilename = "";
        std::string rootFilename = "";
        std::string ionSorterFilename = "";
        std::string elecSorterFilename = "";
        std::string ionCalibTableFilename = "";
        std::string elecCalibTableFilename = "";
        TFile *pRootFile;
        TTree *pRootTree;
        int numberOfIons;
        int numberOfElectrons;
        struct DataSet {
            double x, y, t;
            int flag;
        } *pIonDataSet, *pElecDataSet;

    private:
        bool isFileExist(const char *fileName);

    public:
        SortRun(const std::string configFilename);

        ~SortRun();

        void branchRootTree(const int ionNum, const int elecNum);

        void processEvent(const sort_class &ionSort, const sort_class &elecSort);

        char *getLMFFilename() const;

        char *getIonSorterFilename() const;

        char *getElecSorterFilename() const;

        char *getIonCalibTableFilename() const;

        char *getElecCalibTableFilename() const;

        TFile *getRootFile();

    public:
        TFile *RecreateRootFile(char *name, char *comment);

        TCanvas *newCanvas(char *name, char *titel, int xposition, int yposition, int pixelsx, int pixelsy);

        void SetGreyScale(int number_of_contours);

        void color(int color_palette_id, int number_of_contours);

        TF1 *newTF1(char *a, char *b, double aa, double bb);

        TH1D *newTH1D(int number, char *comment, int bins, double xmin, double xmax);

        TH1D *newTH1D(char *name, char *comment, int bins, double xmin, double xmax);

        TH2D *newTH2D(int number, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                      double ymax, char *option);

        TH2D *newTH2D(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                      double ymax, char *option);

        TH2F *newTH2F(int number, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                      double ymax, char *option);

        TH2F *newTH2F(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                      double ymax, char *option);

        TH2I *newTH2I(int number, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                      double ymax, char *option);

        TH2I *newTH2I(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                      double ymax, char *option);

        TH3D *newTH3D(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                      double ymax, int zbins, double zmin, double zmax, char *option);

        TTree *newTTree(char *arg1, char *arg2);
    };
}

#endif
