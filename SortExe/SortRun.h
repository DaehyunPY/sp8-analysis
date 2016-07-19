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
    };
}

#endif
