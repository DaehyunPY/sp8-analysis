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
    pRootTree->Write();
    pRootFile->Close();
    delete pRootTree;
    delete pRootFile;
    rootFilename = "";
}

Analysis::SortRun::SortRun(const std::string configFilename) {
    // Setup json file reader
    Analysis::JSONReader configReader(configFilename);

    // Change the working directory
    chdir(configReader.getStringAt("working_directory").c_str());
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

void Analysis::SortRun::processEvent() {
    pRootTree->Fill();
}

void Analysis::SortRun::branchRootTree(const int ionCommand, sort_class &ionSort, const int elecCommand,
                                       sort_class &elecSort) {
    // Ion setup
    if (ionCommand == 1) {  // sort and write new file
        // sort/reconstruct the detector signals and apply the sum- and NL-correction.
        numberOfIons = ionSort.sort();
        // "numberOfIons" is the number of reconstructed number of particles
    } else {
        numberOfIons = ionSort.run_without_sorting();
    }
    for (int i = 0; i < numberOfIons; i++) {
        char ch[2];
        sprintf(ch, "%01d", i);
        std::string str;
        str = "Ion";
        pRootTree->Branch((str + "X" + ch).c_str(), &(ionSort.output_hit_array[i]->x), (str + "X" + ch + "/D").c_str());
        pRootTree->Branch((str + "Y" + ch).c_str(), &(ionSort.output_hit_array[i]->y), (str + "Y" + ch + "/D").c_str());
        pRootTree->Branch((str + "T" + ch).c_str(), &(ionSort.output_hit_array[i]->time),
                          (str + "T" + ch + "/D").c_str());
        pRootTree->Branch((str + "Flag" + ch).c_str(), &(ionSort.output_hit_array[i]->method),
                          (str + "Flag" + ch + "/I").c_str());
    }

    // Electron setup
    if (elecCommand == 1) {  // sort and write new file
        // sort/reconstruct the detector signals and apply the sum- and NL-correction.
        numberOfElectrons = elecSort.sort();
        // "numberOfElectrons" is the number of reconstructed number of particles
    } else {
        numberOfElectrons = elecSort.run_without_sorting();
    }
    for (int i = 0; i < numberOfElectrons; i++) {
        char ch[2];
        sprintf(ch, "%01d", i);
        std::string str;
        str = "Elec";
        pRootTree->Branch((str + "X" + ch).c_str(), &(elecSort.output_hit_array[i]->x),
                          (str + "X" + ch + "/D").c_str());
        pRootTree->Branch((str + "Y" + ch).c_str(), &(elecSort.output_hit_array[i]->y),
                          (str + "Y" + ch + "/D").c_str());
        pRootTree->Branch((str + "T" + ch).c_str(), &(elecSort.output_hit_array[i]->time),
                          (str + "T" + ch + "/D").c_str());
        pRootTree->Branch((str + "Flag" + ch).c_str(), &(elecSort.output_hit_array[i]->method),
                          (str + "Flag" + ch + "/I").c_str());
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
