#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#else

#include <unistd.h>

#endif

#include <ctime>
#include <TChain.h>
#include <RooDataProjBinding.h>

#include "SortRun.h"
#include "../JSONHandler/JSONReader.h"

bool Analysis::SortRun::isFileExist(const char *fileName) {
    std::ifstream file(fileName);
    return file.good();
}

Analysis::SortRun::~SortRun() {
    delete[] pIonDataSet;
    delete[] pElecDataSet;
    numberOfIons = 0;
    numberOfElectrons = 0;

    pRootTree->Write();
    pRootFile->Close();
    delete pRootTree;
    delete pRootFile;

    // id = nullptr;
    LMFFilename = "";
    rootFilename = "";
    ionSorterFilename = "";
    elecSorterFilename = "";
    ionCalibTableFilename = "";
    elecCalibTableFilename = "";
}

Analysis::SortRun::SortRun(const std::string configFilename) {
    // Setup json file reader
    Analysis::JSONReader configReader(configFilename);

    // Change the working directory
    chdir(configReader.getStringAt("working_directory").c_str());
    LMFFilename = configReader.getStringAt("LMF_file");
    ionSorterFilename = configReader.getStringAt("ion_sorter");
    elecSorterFilename = configReader.getStringAt("electron_sorter");
    ionCalibTableFilename = configReader.getStringAt("ion_calibration_table");
    elecCalibTableFilename = configReader.getStringAt("electron_calibration_table");

    // Create id
    pRootTree = new TTree("resortedData", "Resorted Data");
    for (int i = 0; i < 10000; i++) {
        sprintf(id, "%04d", i);
        rootFilename = "resortLess";
        rootFilename += id;
        rootFilename += ".root";
        if (isFileExist(rootFilename.c_str())) {
            continue;
        }
        break;
    }
    pRootFile = new TFile(rootFilename.c_str(), "new");

    // Setup ions and electrons
    numberOfIons = 0;
    numberOfElectrons = 0;
}

void Analysis::SortRun::processEvent(const sort_class &ionSort, const sort_class &elecSort) {
    for (int i = 0; i < numberOfIons; i++) {
        pIonDataSet[i].x = ionSort.output_hit_array[i]->x;
        pIonDataSet[i].y = ionSort.output_hit_array[i]->y;
        pIonDataSet[i].t = ionSort.output_hit_array[i]->time;
        pIonDataSet[i].flag = ionSort.output_hit_array[i]->method;
    }
    for (int i = 0; i < numberOfElectrons; i++) {
        pElecDataSet[i].x = elecSort.output_hit_array[i]->x;
        pElecDataSet[i].y = elecSort.output_hit_array[i]->y;
        pElecDataSet[i].t = elecSort.output_hit_array[i]->time;
        pElecDataSet[i].flag = elecSort.output_hit_array[i]->method;
    }
    pRootTree->Fill();
}

void Analysis::SortRun::branchRootTree(const int ionNum, const int elecNum) {
    std::string str;

    // Ion setup
    str = "Ion";
    numberOfIons = ionNum;
    pIonDataSet = new DataSet[numberOfIons];
    for (int i = 0; i < numberOfIons; i++) {
        char ch[2];
        sprintf(ch, "%01d", i);
        pRootTree->Branch((str + "X" + ch).c_str(), &pIonDataSet[i].x, (str + "X" + ch + "/D").c_str());
        pRootTree->Branch((str + "Y" + ch).c_str(), &pIonDataSet[i].y, (str + "Y" + ch + "/D").c_str());
        pRootTree->Branch((str + "T" + ch).c_str(), &pIonDataSet[i].t, (str + "T" + ch + "/D").c_str());
        pRootTree->Branch((str + "Flag" + ch).c_str(), &pIonDataSet[i].flag, (str + "Flag" + ch + "/I").c_str());
    }

    // Electron setup
    str = "Elec";
    numberOfElectrons = elecNum;
    pElecDataSet = new DataSet[numberOfIons];
    for (int i = 0; i < numberOfElectrons; i++) {
        char ch[2];
        sprintf(ch, "%01d", i);
        pRootTree->Branch((str + "X" + ch).c_str(), &pElecDataSet[i].x, (str + "X" + ch + "/D").c_str());
        pRootTree->Branch((str + "Y" + ch).c_str(), &pElecDataSet[i].y, (str + "Y" + ch + "/D").c_str());
        pRootTree->Branch((str + "T" + ch).c_str(), &pElecDataSet[i].t, (str + "T" + ch + "/D").c_str());
        pRootTree->Branch((str + "Flag" + ch).c_str(), &pElecDataSet[i].flag, (str + "Flag" + ch + "/I").c_str());
    }
}

char *Analysis::SortRun::getIonSorterFilename() const {
    return (char *) ionSorterFilename.c_str();
}

char *Analysis::SortRun::getElecSorterFilename() const {
    return (char *) elecSorterFilename.c_str();
}

char *Analysis::SortRun::getIonCalibTableFilename() const {
    return (char *) ionCalibTableFilename.c_str();
}

char *Analysis::SortRun::getElecCalibTableFilename() const {
    return (char *) elecCalibTableFilename.c_str();
}

char *Analysis::SortRun::getLMFFilename() const {
    return (char *) LMFFilename.c_str();
}

TFile *Analysis::SortRun::getRootFile() {
    return pRootFile;
}
