#include "SortRun.h"

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

Analysis::SortRun::SortRun() {
    pRootTree = new TTree("resortedData", "Resorted Data");
    for (int i = 0; i < 10000; i++) {
        char ch[5];
        sprintf(ch, "%04d", i);
        rootFilename = "resortLess";
        rootFilename += ch;
        rootFilename += ".root";
        if (isFileExist(rootFilename.c_str())) {
            continue;
        }
        break;
    }
    pRootFile = new TFile(rootFilename.c_str(), "new");
    for (int i = 0; i < nTmp; i++) {
        char ch[2];
        sprintf(ch, "%01d", i);
        std::string str;
        str = "Elec";
        pRootTree->Branch((str + "X" + ch).c_str(), &elecDataSet[i].x, (str + "X" + ch + "/D").c_str());
        pRootTree->Branch((str + "Y" + ch).c_str(), &elecDataSet[i].y, (str + "Y" + ch + "/D").c_str());
        pRootTree->Branch((str + "T" + ch).c_str(), &elecDataSet[i].t, (str + "T" + ch + "/D").c_str());
        pRootTree->Branch((str + "Flag" + ch).c_str(), &elecDataSet[i].flag, (str + "Flag" + ch + "/I").c_str());
        str = "Ion";
        pRootTree->Branch((str + "X" + ch).c_str(), &ionDataSet[i].x, (str + "X" + ch + "/D").c_str());
        pRootTree->Branch((str + "Y" + ch).c_str(), &ionDataSet[i].y, (str + "Y" + ch + "/D").c_str());
        pRootTree->Branch((str + "T" + ch).c_str(), &ionDataSet[i].t, (str + "T" + ch + "/D").c_str());
        pRootTree->Branch((str + "Flag" + ch).c_str(), &ionDataSet[i].flag, (str + "Flag" + ch + "/I").c_str());
    }
}

void Analysis::SortRun::processEvent() {
    for (int i = 0; i < nTmp; i++) {
        elecDataSet[i].x = resortLess[0][i];
        elecDataSet[i].y = resortLess[1][i];
        elecDataSet[i].t = resortLess[2][i];
        elecDataSet[i].flag = resortElecFlags[i + 1];
        ionDataSet[i].x = resortLess[3][i];
        ionDataSet[i].y = resortLess[4][i];
        ionDataSet[i].t = resortLess[5][i];
        ionDataSet[i].flag = resortIonFlags[i + 1];
    }
    pRootTree->Fill();
}

