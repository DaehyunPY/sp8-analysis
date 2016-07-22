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
	: Hist(false, numHists), prefix(prfx), maxNumOfIons(iNum), maxNumOfElecs(eNum) {
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

void Analysis::SortRun::processEvent(const int ionHitNum,
                                     const sort_class *pIonSorter,
                                     const int elecHitNum,
                                     const sort_class *pElecSorter,
                                     const double eMkr) {
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
  eMarker = eMkr;
  fillTree();
  fillHists();
}
const bool Analysis::SortRun::existTree() const {
	return pRootTree ? true : false;
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
void Analysis::SortRun::fillTree() {
	if (existTree()) {
		pRootTree->Fill();
	}
}
TCanvas *Analysis::SortRun::createCanvas(char *name, char *titel, int xposition, int yposition, int pixelsx, int pixelsy) {
  TCanvas *canvaspointer;
  canvaspointer = new TCanvas(name, titel, xposition, yposition, pixelsx, pixelsy);
  return canvaspointer;
}
void Analysis::SortRun::createHists() {
  create1d(SAME_TITLE_WITH_VALNAME(h1_eMarker), "Time [ns]", 2000, -5000, 1000);
}
void Analysis::SortRun::fillHists() {
  fill1d(h1_eMarker, eMarker);
}
void Analysis::SortRun::createC1() {
	closeC1();
	create1d(SAME_TITLE_WITH_VALNAME(h1_ionTimesumU), "Time [ns]", 500 * 10, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_ionTimesumV), "Time [ns]", 500 * 10, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_ionTimesumW), "Time [ns]", 500 * 10, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_ionU), "Time [ns]", 500 * 2, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_ionV), "Time [ns]", 500 * 2, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_ionW), "Time [ns]", 500 * 2, -250, 250);
	create2d(SAME_TITLE_WITH_VALNAME(h2_ionXYRaw), "Time [ns]", "Time [ns]", 120 * 4, -60, 60, 120 * 4, -60, 60);
	create2d(SAME_TITLE_WITH_VALNAME(h2_ionXY), "Time [ns]", "Time [ns]", 120 * 4, -60, 60, 120 * 4, -60, 60);
	create2d(SAME_TITLE_WITH_VALNAME(h2_ionXYDev), "Time [ns]", "Time [ns]", 100 * 2, -100, 100, 100 * 2, -100, 100);
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
	create1d(SAME_TITLE_WITH_VALNAME(h1_elecTimesumU), "Time [ns]", 500*10, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_elecTimesumV), "Time [ns]", 500 * 10, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_elecTimesumW), "Time [ns]", 500 * 10, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_elecU), "Time [ns]", 500 * 2, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_elecV), "Time [ns]", 500 * 2, -250, 250);
	create1d(SAME_TITLE_WITH_VALNAME(h1_elecW), "Time [ns]", 500 * 2, -250, 250);
	create2d(SAME_TITLE_WITH_VALNAME(h2_elecXYRaw), "Time [ns]", "Time [ns]", 120 * 4, -60, 60, 120 * 4, -60, 60);
	create2d(SAME_TITLE_WITH_VALNAME(h2_elecXY), "Time [ns]", "Time [ns]", 120 * 4, -60, 60, 120 * 4, -60, 60);
	create2d(SAME_TITLE_WITH_VALNAME(h2_elecXYDev), "Time [ns]", "Time [ns]", 100 * 2, -100, 100, 100 * 2, -100, 100);
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
	if(existC1()) {
		updateC1();
		pC1->Close();
		delete pC1;
		pC1 = nullptr;
	}
}
void Analysis::SortRun::closeC2() {
	if(existC2()) {
		updateC1();
		pC2->Close();
		delete pC2;
		pC2 = nullptr;
	}
}
void Analysis::SortRun::closeTree() {
	if(existTree()) {
		fillTree();
		pRootTree->Write();
		delete pRootTree;
		pRootTree = nullptr;
	}
}
