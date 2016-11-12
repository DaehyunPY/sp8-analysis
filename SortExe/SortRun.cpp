#include "SortRun.h"

bool Analysis::SortRun::isFileExist(const char *fileName) {
  std::ifstream file(fileName);
  return file.good();
}

Analysis::SortRun::~SortRun() {
  printf("writing root file... ");
  closeC1();
  closeC2();
  closeTree();
  flushRootFile();
  printf("ok\n");

  if (pIonDataSet) delete[] pIonDataSet;
  if (pElecDataSet) delete[] pElecDataSet;
}

Analysis::SortRun::SortRun(const std::string prfx, const int iNum, const int eNum)
    : Hist(false, numHists),
      prefix(prfx), maxNumOfIons(iNum), maxNumOfElecs(eNum) {
  // Create id
  for (int i = 0; i < 10000; i++) {
    sprintf(id, "%04d", i);
    rootFilename = prfx;
    rootFilename += id;
    rootFilename += ".root";
    if (isFileExist(rootFilename.c_str())) {
      continue;
    }
    break;
  }

  // Setup ROOT
  openRootFile(rootFilename.c_str(), "NEW");
  createTree();
  createHists();
}

void Analysis::SortRun::fillTree(const int ionHitNum, const DataSet *pIons,
                                 const int elecHitNum, const DataSet *pElecs) {
  DataSet emptySet = {0, 0, 0, 0};
  numOfIons = ionHitNum < maxNumOfIons ? ionHitNum : maxNumOfIons;
  for (int i = 0; i < numOfIons; i++) pIonDataSet[i] = pIons[i];
  for (int i = numOfIons; i < maxNumOfIons; i++) pIonDataSet[i] = emptySet;
  numOfElecs = elecHitNum < maxNumOfElecs ? elecHitNum : maxNumOfElecs;
  for (int i = 0; i < numOfElecs; i++) pElecDataSet[i] = pElecs[i];
  for (int i = numOfElecs; i < maxNumOfElecs; i++) pElecDataSet[i] = emptySet;
  if (existTree()) pRootTree->Fill();
}
const bool Analysis::SortRun::existTree() const {
  return pRootTree != nullptr;
}
void Analysis::SortRun::createTree() {
  closeTree();
  pRootTree = new TTree("resortedData", "Resorted Data");
  std::string str;
  // Ion setup
  str = "Ion";
  pIonDataSet = new DataSet[maxNumOfIons];
  pRootTree->Branch((str + "Num").c_str(), &numOfIons, (str + "Num/I").c_str());
  for (int i = 0; i < maxNumOfIons; i++) {
    char ch[2];
    sprintf(ch, "%01d", i);
    pRootTree->Branch((str + "X" + ch).c_str(), &pIonDataSet[i].x, (str + "X" + ch + "/D").c_str());
    pRootTree->Branch((str + "Y" + ch).c_str(), &pIonDataSet[i].y, (str + "Y" + ch + "/D").c_str());
    pRootTree->Branch((str + "T" + ch).c_str(), &pIonDataSet[i].t, (str + "T" + ch + "/D").c_str());
    pRootTree->Branch((str + "Flag" + ch).c_str(), &pIonDataSet[i].flag, (str + "Flag" + ch + "/I").c_str());
  }
  // Electron setup
  str = "Elec";
  pElecDataSet = new DataSet[maxNumOfIons];
  pRootTree->Branch((str + "Num").c_str(), &numOfElecs, (str + "Num/I").c_str());
  for (int i = 0; i < maxNumOfElecs; i++) {
    char ch[2];
    sprintf(ch, "%01d", i);
    pRootTree->Branch((str + "X" + ch).c_str(), &pElecDataSet[i].x, (str + "X" + ch + "/D").c_str());
    pRootTree->Branch((str + "Y" + ch).c_str(), &pElecDataSet[i].y, (str + "Y" + ch + "/D").c_str());
    pRootTree->Branch((str + "T" + ch).c_str(), &pElecDataSet[i].t, (str + "T" + ch + "/D").c_str());
    pRootTree->Branch((str + "Flag" + ch).c_str(), &pElecDataSet[i].flag, (str + "Flag" + ch + "/I").c_str());
  }
}
TCanvas *Analysis::SortRun::createCanvas(char *name,
                                         char *titel,
                                         int xposition,
                                         int yposition,
                                         int pixelsx,
                                         int pixelsy) {
  TCanvas *canvaspointer;
  canvaspointer = new TCanvas(name, titel, xposition, yposition, pixelsx, pixelsy);
  return canvaspointer;
}
void Analysis::SortRun::createHists() {
#define __ION_FISH_TITLE_BIN_REGION__ "TIME [ns]", "Location [mm]", 500, -3000, 12000, 200, -100, 100
#define __ION_XY_TITLE_BIN_REGION__ "Location X [mm]", "Location Y [mm]", 200, -100, 100, 200, -100, 100
#define __ION_PIPICO_TITLE_BIN_REGION__ "Time 1 [ns]", "Time 2 [ns]", 500, -3000, 12000, 500, -3000, 12000
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion1hitXFish), __ION_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion1hitYFish), __ION_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion1hitXY), __ION_XY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion2hitXFish), __ION_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion2hitYFish), __ION_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion2hitXY), __ION_XY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion3hitXFish), __ION_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion3hitYFish), __ION_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion3hitXY), __ION_XY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion4hitXFish), __ION_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion4hitYFish), __ION_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion4hitXY), __ION_XY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion1hit2hitPIPICO), __ION_PIPICO_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion2hit3hitPIPICO), __ION_PIPICO_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ion3hit4hitPIPICO), __ION_PIPICO_TITLE_BIN_REGION__);

#define __ELEC_FISH_TITLE_BIN_REGION__ "TIME [ns]", "Location [mm]", 1200, -100, 500, 200, -100, 100
#define __ELEC_XY_TITLE_BIN_REGION__ "Location X [mm]", "Location Y [mm]", 200, -100, 100, 200, -100, 100
#define __ELEC_PIPICO_TITLE_BIN_REGION__ "Time 1 [ns]", "Time 2 [ns]", 1200, -100, 500, 1200, -100, 500
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec1hitXFish), __ELEC_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec1hitYFish), __ELEC_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec1hitXY), __ELEC_XY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec2hitXFish), __ELEC_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec2hitYFish), __ELEC_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec2hitXY), __ELEC_XY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec3hitXFish), __ELEC_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec3hitYFish), __ELEC_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec3hitXY), __ELEC_XY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec4hitXFish), __ELEC_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec4hitYFish), __ELEC_FISH_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec4hitXY), __ELEC_XY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec1hit2hitPEPECO), __ELEC_PIPICO_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec2hit3hitPEPECO), __ELEC_PIPICO_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elec3hit4hitPEPECO), __ELEC_PIPICO_TITLE_BIN_REGION__);

#define __TIMESUM_TITLE_BIN_REGION__ "Time [ns]", 5000, -250, 250
#define __TIMEDELAY_TITLE_BIN_REGION__ "Time [ns]", 1000, -250, 250
#define __TIMESUMDIFF_TITLE_BIN_REGION__ "Time diff [ns]", "Time sum [ns]", 1000, -250, 250, 1000, -250, 250
#define __AFTERCALIB_TITLE_BIN_REGION__(X) "Time1 [ns]", "Time2 [ns]", X*2, -X, X, X*2, -X, X
  create1d(SAME_TITLE_WITH_VALNAME(h1_ionTimesumU), __TIMESUM_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_ionTimesumV), __TIMESUM_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_ionTimesumW), __TIMESUM_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_ionU), __TIMEDELAY_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_ionV), __TIMEDELAY_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_ionW), __TIMEDELAY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ionTimesumDiffU), __TIMESUMDIFF_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ionTimesumDiffV), __TIMESUMDIFF_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ionTimesumDiffW), __TIMESUMDIFF_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_ionXYRaw), __AFTERCALIB_TITLE_BIN_REGION__(60));
  create2d(SAME_TITLE_WITH_VALNAME(h2_ionXY), __AFTERCALIB_TITLE_BIN_REGION__(60));
  create2d(SAME_TITLE_WITH_VALNAME(h2_ionXYDev), __AFTERCALIB_TITLE_BIN_REGION__(100));

  create1d(SAME_TITLE_WITH_VALNAME(h1_elecTimesumU), __TIMESUM_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_elecTimesumV), __TIMESUM_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_elecTimesumW), __TIMESUM_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_elecU), __TIMEDELAY_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_elecV), __TIMEDELAY_TITLE_BIN_REGION__);
  create1d(SAME_TITLE_WITH_VALNAME(h1_elecW), __TIMEDELAY_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elecTimesumDiffU), __TIMESUMDIFF_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elecTimesumDiffV), __TIMESUMDIFF_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elecTimesumDiffW), __TIMESUMDIFF_TITLE_BIN_REGION__);
  create2d(SAME_TITLE_WITH_VALNAME(h2_elecXYRaw), __AFTERCALIB_TITLE_BIN_REGION__(60));
  create2d(SAME_TITLE_WITH_VALNAME(h2_elecXY), __AFTERCALIB_TITLE_BIN_REGION__(60));
  create2d(SAME_TITLE_WITH_VALNAME(h2_elecXYDev), __AFTERCALIB_TITLE_BIN_REGION__(100));

#define __EMARKER_TITLE_BIN_REGION__ "Time [ns]", 2000, -7500, 2500
  create1d(SAME_TITLE_WITH_VALNAME(h1_eMarker), __EMARKER_TITLE_BIN_REGION__);
}
void Analysis::SortRun::createC1() {
  closeC1();
  pC1 = createCanvas("ion_canvas", "ion_canvas", 10, 10, 910, 910);
  pC1->Divide(3, 3);
  pC1->cd(1);
  getHist1d(h1_ionTimesumU)->Draw();
  pC1->cd(2);
  getHist1d(h1_ionTimesumV)->Draw();
  pC1->cd(3);
  getHist1d(h1_ionTimesumW)->Draw();
  pC1->cd(4);
  getHist1d(h1_ionU)->Draw();
  pC1->cd(5);
  getHist1d(h1_ionV)->Draw();
  pC1->cd(6);
  getHist1d(h1_ionW)->Draw();
  pC1->cd(7);
  getHist2d(h2_ionXYRaw)->Draw();
  pC1->cd(8);
  getHist2d(h2_ionXY)->Draw();
  pC1->cd(9);
  getHist2d(h2_ionXYDev)->Draw();
}
void Analysis::SortRun::createC2() {
  closeC2();
  pC1 = createCanvas("elec_canvas", "elec_canvas", 10, 10, 910, 910);
  pC1->Divide(3, 3);
  pC1->cd(1);
  getHist1d(h1_elecTimesumU)->Draw();
  pC1->cd(2);
  getHist1d(h1_elecTimesumV)->Draw();
  pC1->cd(3);
  getHist1d(h1_elecTimesumW)->Draw();
  pC1->cd(4);
  getHist1d(h1_elecU)->Draw();
  pC1->cd(5);
  getHist1d(h1_elecV)->Draw();
  pC1->cd(6);
  getHist1d(h1_elecW)->Draw();
  pC1->cd(7);
  getHist2d(h2_elecXYRaw)->Draw();
  pC1->cd(8);
  getHist2d(h2_elecXY)->Draw();
  pC1->cd(9);
  getHist2d(h2_elecXYDev)->Draw();
}
const bool Analysis::SortRun::existC1() const {
  return pC1 ? true : false;
}
const bool Analysis::SortRun::existC2() const {
  return pC2 ? true : false;
}
void Analysis::SortRun::updateC1() {
  if (existC1()) {
    for (int i = 1; i <= 9; i++) {
      pC1->cd(i)->Update();
    }
  }
}
void Analysis::SortRun::updateC1(const bool mdf) {
  if (existC1()) {
    for (int i = 1; i <= 9; i++) {
      pC1->cd(i)->Modified(mdf);
      pC1->cd(i)->Update();
    }
  }
}
void Analysis::SortRun::updateC2() {
  if (existC2()) {
    for (int i = 1; i <= 9; i++) {
      pC2->cd(i)->Update();
    }
  }
}
void Analysis::SortRun::updateC2(const bool mdf) {
  if (existC2()) {
    for (int i = 1; i <= 9; i++) {
      pC2->cd(i)->Modified(mdf);
      pC2->cd(i)->Update();
    }
  }
}
void Analysis::SortRun::closeC1() {
  if (existC1()) {
    updateC1();
    pC1->Close();
    delete pC1;
    pC1 = nullptr;
  }
}
void Analysis::SortRun::closeC2() {
  if (existC2()) {
    updateC1();
    pC2->Close();
    delete pC2;
    pC2 = nullptr;
  }
}
void Analysis::SortRun::closeTree() {
  if (existTree()) {
    pRootTree->Write();
    delete pRootTree;
    pRootTree = nullptr;
  }
}
