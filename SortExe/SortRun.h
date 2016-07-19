#ifndef ANALYSIS_SORTRUN_H
#define ANALYSIS_SORTRUN_H

#include <fstream>
#include <TFile.h>
#include <TTree.h>
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

    private:
        bool isFileExist(const char *fileName);

    public:
        SortRun(const std::string configFilename);
        ~SortRun();

        void branchRootTree(const int ionCommand, sort_class &ionSort, const int elecCommand, sort_class &elecSort);
        void processEvent();

		char *getLMFFilename() const; 

        char *getIonSorterFilename() const;

        char *getElecSorterFilename() const;

        char *getIonCalibTableFilename() const;

        char *getElecCalibTableFilename() const;
    };
}

#endif
