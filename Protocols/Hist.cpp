#include "Hist.h"

Analysis::Hist::Hist(const bool verbose, int size)
    : pRootFile(nullptr), optionForVerbose(verbose) {
  arraySize = size;
  ppHistArray = new TObject *[arraySize];
  for (int i = 0; i < arraySize; ++i) {
    ppHistArray[i] = 0;
  }
}
Analysis::Hist::~Hist() {
  delete[] ppHistArray;
  if (pRootFile) {
    pRootFile->Close();
  }
}
void Analysis::Hist::openRootFile(const TString &name) {
  pRootFile = TFile::Open(name.Data(), "RECREATE");
  if (!pRootFile) {
    std::cout << " could not be opened, please delete the file!" << std::endl;
    exit(1);
  }
  if (optionForVerbose)
    std::cout << "Histograms will be written to: " << name << std::endl;
}
void Analysis::Hist::resetAll() {
  if (optionForVerbose) std::cout << "reset all histos" << std::endl;
  //--write histos to directory--//
  pRootFile->cd();
  for (int i = 0; i < arraySize; ++i) {
    TObject *obj = ppHistArray[i];
    if (obj) {

      //--First change to the right directory--//
      TH1 *h = (TH1 *) obj;
      if (optionForVerbose)
        std::cout << "Resetting Histogram:" << h->GetName() << " Dir:"
            << h->GetDirectory()->GetName() << std::endl;

      //--reset histogram--//
      h->Reset();
    }
  }
}
void Analysis::Hist::flushRootFile() {
  if (optionForVerbose) std::cout << "flushing root file" << std::endl;
  //--write histos to directory--//
  pRootFile->cd();
  for (int i = 0; i < arraySize; ++i) {
    TObject *obj = ppHistArray[i];
    if (obj) {

      //--First change to the right directory--//
      TH1 *h = (TH1 *) obj;
      h->GetDirectory()->cd();
      if (optionForVerbose)
        std::cout << "Writing Histogram:" << h->GetName() << " Dir:"
            << h->GetDirectory()->GetName() << std::endl;

      //--write object to current directory--//
      obj->Write(0, TObject::kOverwrite);
      pRootFile->cd("/");
    }
  }
  //--save directory to root file--//
  pRootFile->SaveSelf();
}
TDirectory *getDir(TFile *rootfile, TString dirName) {
  //first find out wether directory exists
  rootfile->cd("/");
  TDirectory *direc = rootfile->GetDirectory(dirName.Data());
  if (!direc) {

    //if not create it//
    TString lhs;
    TString rhs;
    TString tmp = dirName;
    while (1) {

      //if there is no / then this is the last sub dir
      if (tmp.Index("/") == -1) {
        lhs = tmp;
      }
      else //otherwise split the string to lefthandside and righthandside of "/"
      {
        lhs = tmp(0, tmp.Index("/"));
        rhs = tmp(tmp.Index("/") + 1, tmp.Length());
      }

      //check wether subdir exits//
      direc = gDirectory->GetDirectory(lhs.Data());
      if (direc)
        gDirectory->cd(lhs.Data());//cd into it
      else {
        direc = gDirectory->mkdir(lhs.Data()); //create it
        gDirectory->cd(lhs.Data()); //and cd into it
      }

      //when there is no "/" anymore break here//
      if (tmp.Index("/") == -1)
        break;

      //the new temp is all that is on the right hand side
      tmp = rhs;
    }
  }

  //return to root Path//
  rootfile->cd("/");
  return direc;
}
void Analysis::Hist::fill3d(const int id,
                            const double fillX,
                            const double fillY,
                            const double fillZ,
                            const double weight) {
  dynamic_cast<TH3D *>(ppHistArray[id])->Fill(fillX, fillY, fillZ, weight);
}
TH1 *Analysis::Hist::create3d(int id,
                              const char *name,
                              const char *titleX,
                              const char *titleY,
                              const char *titleZ,
                              int nXbins,
                              double xLow,
                              double xUp,
                              int nYbins,
                              double yLow,
                              double yUp,
                              int nZbins,
                              double zLow,
                              double zUp,
                              const char *dir,
                              bool alreadylocked) {
  //check if hist already exists, if so return it//
  pHist3 = dynamic_cast<TH3D *>(ppHistArray[id]);
  if (pHist3) return pHist3;

  TDirectory
      *saveDir = gDirectory;        //save a pointer to the current directory
  getDir(pRootFile,
         dir)->cd();                //change to directory that this histo need to be created in

  //--create a 3D histogram--//
  pHist3 = new TH3D(name,
                    name,
                    nXbins,
                    xLow,
                    xUp,
                    nYbins,
                    yLow,
                    yUp,
                    nZbins,
                    zLow,
                    zUp);
  pHist3->SetXTitle(titleX);
  pHist3->GetXaxis()->CenterTitle(true);
  pHist3->GetXaxis()->SetTitleOffset(1.5);
  pHist3->SetYTitle(titleY);
  pHist3->GetYaxis()->CenterTitle(true);
  pHist3->GetYaxis()->SetTitleOffset(1.5);
  pHist3->SetZTitle(titleZ);
  pHist3->GetZaxis()->CenterTitle(true);
  pHist3->GetZaxis()->SetTitleOffset(1.5);
  saveDir->cd(); //reset the previously selectet directory

  //--check if there is a histogram with same id but different name--//
  TObject *obj = ppHistArray[id];
  if (obj)
    std::cout << "name doesn't match(" << id << ") is:" << obj->GetName()
        << " should be:" << pHist3->GetName() << std::endl;

  //--now add it to the list--//
  ppHistArray[id] = pHist3;
  if (optionForVerbose)
    std::cout << "create 3D: " << dir << "/" << pHist3->GetName() << std::endl;
  return pHist3;
}

void Analysis::Hist::fill(int id,
                          const char *name,
                          double fillX,
                          double fillY,
                          double fillZ,
                          const char *titleX,
                          const char *titleY,
                          const char *titleZ,
                          int nXbins,
                          double xLow,
                          double xUp,
                          int nYbins,
                          double yLow,
                          double yUp,
                          int nZbins,
                          double zLow,
                          double zUp,
                          const char *dir,
                          double weight) {
  pHist3 = dynamic_cast<TH3D *>(ppHistArray[id]);

  //--if the histo does not exist create it first--//
  if (!pHist3) {
    create3d(id, name,
             titleX, titleY, titleZ,
             nXbins, xLow, xUp,
             nYbins, yLow, yUp,
             nZbins, zLow, zUp,
             dir, true);
  }

    //if the histogram exists, but has no entries, we want to resize and rebin it//
  else if ((!pHist3->GetEntries())
      && ((nXbins != pHist3->GetXaxis()->GetNbins())
          || (xLow != pHist3->GetXaxis()->GetXmin())
          || (xUp != pHist3->GetXaxis()->GetXmax()) ||
          (nYbins != pHist3->GetYaxis()->GetNbins())
          || (yLow != pHist3->GetYaxis()->GetXmin())
          || (yUp != pHist3->GetYaxis()->GetXmax()) ||
          (nZbins != pHist3->GetZaxis()->GetNbins())
          || (zLow != pHist3->GetZaxis()->GetXmin())
          || (zUp != pHist3->GetZaxis()->GetXmax())))
    pHist3->SetBins(nXbins, xLow, xUp, nYbins, yLow, yUp, nZbins, zLow, zUp);

    //--if the histogram exists, check also if given name is the one of the histogram--//
  else if (strcmp(pHist3->GetName(), name))
    std::cout << "name doesn't match(" << id << ") is:" << pHist3->GetName()
        << " should be:" << name << std::endl;

  //--now fill it--//
  pHist3->Fill(fillX, fillY, fillZ, weight);

}
void Analysis::Hist::fill2d(const int id,
                            const double fillX,
                            const double fillY,
                            const double weight) {
  dynamic_cast<TH2D *>(ppHistArray[id])->Fill(fillX, fillY, weight);
}
void Analysis::Hist::plot2d(int id, int binX, int binY, double content) {
  dynamic_cast<TH2D *>(ppHistArray[id])->SetBinContent(binX, binY, content);
}
TH1 *Analysis::Hist::create2d(int id, const char *name,
                              const char *titleX, const char *titleY,
                              int nXbins, double xLow, double xUp,
                              int nYbins, double yLow, double yUp,
                              const char *dir, bool alreadylocked) {
  //check if hist already exists, if so return it//
  pHist2 = dynamic_cast<TH2D *>(ppHistArray[id]);
  if (pHist2) return pHist2;

  TDirectory
      *saveDir = gDirectory;        //save a pointer to the current directory
  getDir(pRootFile,
         dir)->cd();                //change to directory that this histo need to be created in

  //--create a 2D histogram--//
  pHist2 = new TH2D(name, name, nXbins, xLow, xUp, nYbins, yLow, yUp);
  pHist2->SetOption("colz");
  pHist2->SetXTitle(titleX);
  pHist2->GetXaxis()->CenterTitle(true);
  pHist2->SetYTitle(titleY);
  pHist2->GetYaxis()->CenterTitle(true);
  pHist2->GetYaxis()->SetTitleOffset(1.5);
  saveDir->cd();                            //reset the previously selectet directory

  //--check if there is a histogram with same id but different name--//
  TObject *obj = ppHistArray[id];
  if (obj)
    std::cout << "name doesn't match(" << id << ") is:" << obj->GetName()
        << " should be:" << pHist2->GetName() << std::endl;

  //--now add it to the list--//
  ppHistArray[id] = pHist2;
  if (optionForVerbose)
    std::cout << "create 2D: " << dir << "/" << pHist2->GetName() << std::endl;
  return pHist2;
}
void Analysis::Hist::fill(int id,
                          const char *name,
                          double fillX,
                          double fillY,
                          const char *titleX,
                          const char *titleY,
                          int nXbins,
                          double xLow,
                          double xUp,
                          int nYbins,
                          double yLow,
                          double yUp,
                          const char *dir,
                          double weight) {
  pHist2 = dynamic_cast<TH2D *>(ppHistArray[id]);


  //--if the histo does not exist create it first--//
  if (!pHist2) {
    create2d(id, name,
             titleX, titleY,
             nXbins, xLow, xUp,
             nYbins, yLow, yUp,
             dir, true);
  }

    //if the histogram exists, but has no entries, we want to resize and rebin it//
  else if ((!pHist2->GetEntries())
      && ((nXbins != pHist2->GetXaxis()->GetNbins())
          || (xLow != pHist2->GetXaxis()->GetXmin())
          || (xUp != pHist2->GetXaxis()->GetXmax()) ||
          (nYbins != pHist2->GetYaxis()->GetNbins())
          || (yLow != pHist2->GetYaxis()->GetXmin())
          || (yUp != pHist2->GetYaxis()->GetXmax())))
    pHist2->SetBins(nXbins, xLow, xUp, nYbins, yLow, yUp);

    //--if the histogram exists, check also if given name is the one of the histogram--//
  else if (strcmp(pHist2->GetName(), name))
    std::cout << "name doesn't match(" << id << ") is:" << pHist2->GetName()
        << " should be:" << name << std::endl;

  //--now fill it--//
  pHist2->Fill(fillX, fillY, weight);
}
void Analysis::Hist::fill1d(const int id,
                            const double fillX,
                            const double weight) {
  dynamic_cast<TH1D *>(ppHistArray[id])->Fill(fillX, weight);
}
void Analysis::Hist::fill1d(const int id,
                            const double *pX,
                            const double weight) {
  if (pX != nullptr) {
    fill1d(id, *pX, weight);
  }
}
void Analysis::Hist::plot1d(int id, int binX, double content) {
  dynamic_cast<TH1D *>(ppHistArray[id])->SetBinContent(binX, content);
}
TH1 *Analysis::Hist::create1d(int id, const char *name,
                              const char *titleX,
                              int nXbins, double xLow, double xUp,
                              const char *dir, bool alreadylocked) {
  //check if hist already exists, if so return it//
  pHist1 = dynamic_cast<TH1D *>(ppHistArray[id]);
  if (pHist1) return pHist1;

  TDirectory
      *saveDir = gDirectory;        //save a pointer to the current directory
  getDir(pRootFile,
         dir)->cd();                //change to directory that this histo need to be created in

  //--create a 1D histogram--//
  pHist1 = new TH1D(name, name, nXbins, xLow, xUp);
  pHist1->SetXTitle(titleX);
  pHist1->GetXaxis()->CenterTitle(true);
  saveDir->cd();                            //reset the previously selectet directory

  //--check if there is a histogram with same id but different name--//
  TObject *obj = ppHistArray[id];
  if (obj)
    std::cout << "name doesn't match(" << id << ") is:" << obj->GetName()
        << " should be:" << pHist1->GetName() << std::endl;

  //--now add it to the list--//
  ppHistArray[id] = pHist1;
  if (optionForVerbose)
    std::cout << "create 1D: " << dir << "/" << pHist1->GetName() << std::endl;
  return pHist1;
}
void Analysis::Hist::fill(int id,
                          const char *name,
                          double fillX,
                          const char *titleX,
                          int nXbins,
                          double xLow,
                          double xUp,
                          const char *dir,
                          double weight) {
  pHist1 = dynamic_cast<TH1D *>(ppHistArray[id]);

  //--if the histo does not exist create it first--//
  if (!pHist1) {
    create1d(id, name,
             titleX,
             nXbins, xLow, xUp,
             dir, true);
  }

    //if the histogram exists, but has no entries, we want to resize and rebin it//
  else if ((!pHist1->GetEntries())
      && ((nXbins != pHist1->GetXaxis()->GetNbins())
          || (xLow != pHist1->GetXaxis()->GetXmin())
          || (xUp != pHist1->GetXaxis()->GetXmax())))
    pHist1->SetBins(nXbins, xLow, xUp);

    //--if the histogram exists, check also if given name is the one of the histogram--//
  else if (strcmp(pHist1->GetName(), name))
    std::cout << "name doesn't match(" << id << ") is:" << pHist1->GetName()
        << " should be:" << name << std::endl;

  //--now fill it--//
  pHist1->Fill(fillX, weight);
}
void Analysis::Hist::linkRootFile(TFile &RootFile) {
  pRootFile = &RootFile;
}
void Analysis::Hist::plot3d(int id,
                            int binX,
                            int binY,
                            int binZ,
                            double content) {
  dynamic_cast<TH3D *>(ppHistArray[id])->SetBinContent(binX,
                                                       binY,
                                                       binZ,
                                                       content);
}
const bool Analysis::Hist::isVerbose() const {
  return optionForVerbose;
}
TH3 *Analysis::Hist::getHist3d(int id) const {
  return dynamic_cast<TH3 *>(ppHistArray[id]);
}
TH2 *Analysis::Hist::getHist2d(int id) const {
  return dynamic_cast<TH2 *>(ppHistArray[id]);
}
TH1 *Analysis::Hist::getHist1d(int id) const {
  return dynamic_cast<TH1 *>(ppHistArray[id]);
}
void Analysis::Hist::fill2d(const int id,
                            const double *pX,
                            const double *pY,
                            const double weight) {
  if (pX != nullptr && pY != nullptr) {
    fill2d(id, *pX, *pY, weight);
  }
}
void Analysis::Hist::fill3d(const int id,
                            const double *pX,
                            const double *pY,
                            const double *pZ,
                            const double weight) {
  if (pX != nullptr && pY != nullptr && pZ != nullptr) {
    fill3d(id, *pX, *pY, *pZ, weight);
  }
}
