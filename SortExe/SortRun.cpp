#include "SortRun.h"

bool Analysis::SortRun::isFileExist(const char *fileName) {
  std::ifstream file(fileName);
  return file.good();
}

Analysis::SortRun::~SortRun() {
  printf("writing root file... ");
  if (pRootTree) pRootTree->Write();
  if (pRootFile) pRootFile->Write();
  printf("ok\n");

  printf("Closing root file... ");
  flushRootFile();
  if (pRootFile) pRootFile->Close();
  printf("ok\n");

  if (pIonDataSet) delete[] pIonDataSet;
  if (pElecDataSet) delete[] pElecDataSet;
  maxNumOfIons = 0;
  maxNumOfElecs = 0;
  eMaker = 0;
  if (pRootTree) delete pRootTree;
  if (pRootFile) delete pRootFile;
  // id = nullptr;
  LMFFilename = "";
  rootFilename = "";
  ionSorterFilename = "";
  elecSorterFilename = "";
  ionCalibTableFilename = "";
  elecCalibTableFilename = "";
}

Analysis::SortRun::SortRun(const std::string configFilename) : Hist(false, numHists) {
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
  maxNumOfIons = 0;
  maxNumOfElecs = 0;
}

void Analysis::SortRun::processEvent(const int ionHitNum, const sort_class *pIonSorter, const int elecHitNum,
                                     const sort_class *pElecSorter, const double eMkr) {
  numOfIons = ionHitNum < maxNumOfIons ? ionHitNum : maxNumOfIons;
  for (int i = 0; i < numOfIons; i++) {
    pIonDataSet[i].x = pIonSorter->output_hit_array[i]->x;
    pIonDataSet[i].y = pIonSorter->output_hit_array[i]->y;
    pIonDataSet[i].t = pIonSorter->output_hit_array[i]->time;
    pIonDataSet[i].flag = pIonSorter->output_hit_array[i]->method;
  }
  numOfElecs = elecHitNum < maxNumOfElecs ? elecHitNum : maxNumOfElecs;
  for (int i = 0; i < numOfIons; i++) {
    pElecDataSet[i].x = pElecSorter->output_hit_array[i]->x;
    pElecDataSet[i].y = pElecSorter->output_hit_array[i]->y;
    pElecDataSet[i].t = pElecSorter->output_hit_array[i]->time;
    pElecDataSet[i].flag = pElecSorter->output_hit_array[i]->method;
  }
  eMaker = eMkr;
  pRootTree->Fill();
}

void Analysis::SortRun::branchRootTree(const int ionNum, const int elecNum) {
  std::string str;
  // Ion setup
  str = "Ion";
  maxNumOfIons = ionNum;
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
  maxNumOfElecs = elecNum;
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

TCanvas *Analysis::SortRun::newCanvas(char *name, char *titel, int xposition, int yposition, int pixelsx, int pixelsy) {
  TCanvas *canvaspointer;
  canvaspointer = new TCanvas(name, titel, xposition, yposition, pixelsx, pixelsy);
  return canvaspointer;
}

TH1D *Analysis::SortRun::newTH1D(char *name, char *comment, int bins, double xmin, double xmax) {
  TH1D *hist;
  hist = new TH1D(name, comment, bins, xmin, xmax);
  return hist;
}

TH2D *Analysis::SortRun::newTH2D(char *name, char *comment, int xbins, double xmin, double xmax, int ybins, double ymin,
                                 double ymax, char *option) {
  TH2D *hist;
  hist = new TH2D(name, comment, xbins, xmin, xmax, ybins, ymin, ymax);
  hist->SetOption(option);
  return hist;
}
void Analysis::SortRun::createHists() {
  create1d(h1_eMaker, TO_TEXT(h1_eMaker), "TOF [ns]", 2000, -5000, 1000);
}
void Analysis::SortRun::fillHists() {
  fill1d(h1_eMaker, eMaker);
}
