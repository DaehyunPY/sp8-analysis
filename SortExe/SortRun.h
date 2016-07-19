#ifndef ANALYSIS_SORTRUN_H
#define ANALYSIS_SORTRUN_H

#include <fstream>
#include <TFile.h>
#include <TTree.h>

const int nTmp = 4;
double resortLess[nTmp][nTmp], resortElecFlags[nTmp], resortIonFlags[nTmp];

namespace Analysis {
    class SortRun {
    private:
        std::string rootFilename = "";
        TFile *pRootFile;
        TTree *pRootTree;
        struct DataSet {
            double x, y, t;
            int flag;
        } elecDataSet[nTmp], ionDataSet[nTmp];

    private:
        bool isFileExist(const char *fileName);

    public:
        SortRun();

        ~SortRun();

        void processEvent();
    };
}

#endif
