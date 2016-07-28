//
// Created by Daehyun You on 12/30/15.
//

#ifndef ANALYSIS_ANALYSISRUN_H
#define ANALYSIS_ANALYSISRUN_H


#ifdef _WIN32
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#else
#include <unistd.h>
#endif
#include <string>
#include <ctime>
#include <TFile.h>
#include <TChain.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TChain.h>
#include <RooDataProjBinding.h>
#include "OutputFlag.h"
#include "../Protocols/Hist.h"
#include "../AnalysisCore/Unit.h"
#include "../AnalysisCore/AnalysisTools.h"
#include "../AnalysisCore/Ions.h"
#include "../AnalysisCore/Electrons.h"
#include "../AnalysisCore/LogWriter.h"

namespace Analysis {
class AnalysisRun: Hist {
  int numberOfHits;
  TChain *pEventChain;
  Analysis::Unit *pUnit;
  Analysis::AnalysisTools *pTools;
  Analysis::Ions *pIons;
  Analysis::Electrons *pElectrons;
  Analysis::EventDataReader *pEventReader;
  Analysis::LogWriter *pLogWriter;
  Analysis::OutputFlag flag;

 public:
  AnalysisRun(const std::string configFilename);
  ~AnalysisRun();
  const long getEntries() const;
  void processEvent(const long raw);

 private:
  enum HistList {
    // IonImage
    h2_i1hImage, h2_i1hImage_master,
    h2_i2hImage, h2_i2hImage_master,
    h2_i3hImage, h2_i3hImage_master,
    h2_i4hImage, h2_i4hImage_master,
    h2_iCOMImage, h2_iCOMImage_master,

    // IonTOF
    h1_i1hTOF, h1_i1hTOF_master,
    h1_i2hTOF, h1_i2hTOF_master,
    h1_i3hTOF, h1_i3hTOF_master,
    h1_i4hTOF, h1_i4hTOF_master,
    h1_iTotalTOF, h1_iTotalTOF_master,
    h2_i1h2hPIPICO, h2_i1h2hPIPICO_master,
    h2_i2h3hPIPICO, h2_i2h3hPIPICO_master,
    h2_i3h4hPIPICO, h2_i3h4hPIPICO_master,
    h2_i1h2hRotPIPICO, h2_i1h2hRotPIPICO_master,
    h2_i2h3hRotPIPICO, h2_i2h3hRotPIPICO_master,
    h2_i3h4hRotPIPICO, h2_i3h4hRotPIPICO_master,
    h2_i1h2h3h3PICO, h2_i1h2h3h3PICO_master,
    h2_i2h3h4h3PICO, h2_i2h3h4h3PICO_master,
    h2_i1h2h3h4h4PICO, h2_i1h2h3h4h4PICO_master,

    // IonFish
    h2_i1hXFish, h2_i1hXFish_master,
    h2_i1hYFish, h2_i1hYFish_master,
    h2_i1hRFish, h2_i1hRFish_master,
    h2_i2hXFish, h2_i2hXFish_master,
    h2_i2hYFish, h2_i2hYFish_master,
    h2_i2hRFish, h2_i2hRFish_master,
    h2_i3hXFish, h2_i3hXFish_master,
    h2_i3hYFish, h2_i3hYFish_master,
    h2_i3hRFish, h2_i3hRFish_master,
    h2_i4hXFish, h2_i4hXFish_master,
    h2_i4hYFish, h2_i4hYFish_master,
    h2_i4hRFish, h2_i4hRFish_master,

    // IonMomentum
    h2_i1hPDirDist, h2_i1hPDirDist_master,
    h2_i2hPDirDist, h2_i2hPDirDist_master,
    h2_i3hPDirDist, h2_i3hPDirDist_master,
    h2_i4hPDirDist, h2_i4hPDirDist_master,

    // IonEnergy
    h1_i1hE, h1_i1hE_master,
    h1_i2hE, h1_i2hE_master,
    h1_i3hE, h1_i3hE_master,
    h1_i4hE, h1_i4hE_master,
    h1_iTotalE, h1_iTotalE_master,
    h2_i1h2hE, h2_i1h2hE_master,
    h2_i2h3hE, h2_i2h3hE_master,
    h2_i3h4hE, h2_i3h4hE_master,

    // ElecImage
    h2_eh1Image, h2_eh1Image_master,
    h2_eh2Image, h2_eh2Image_master,
    h2_eh3Image, h2_eh3Image_master,
    h2_eh4Image, h2_eh4Image_master,

    // ElecTOF
    h1_e1hTOF, h1_e1hTOF_master,
    h1_e2hTOF, h1_e2hTOF_master,
    h1_e3hTOF, h1_e3hTOF_master,
    h1_e4hTOF, h1_e4hTOF_master,
    h2_e1h2hPEPECO, h2_e1h2hPEPECO_master,
    h2_e2h3hPEPECO, h2_e2h3hPEPECO_master,
    h2_e3h4hPEPECO, h2_e3h4hPEPECO_master,

    // ElecFish
    h2_e1hXFish, h2_e1hXFish_master,
    h2_e1hYFish, h2_e1hYFish_master,
    h2_e1hRFish, h2_e1hRFish_master,
    h2_e2hXFish, h2_e2hXFish_master,
    h2_e2hYFish, h2_e2hYFish_master,
    h2_e2hRFish, h2_e2hRFish_master,
    h2_e3hXFish, h2_e3hXFish_master,
    h2_e3hYFish, h2_e3hYFish_master,
    h2_e3hRFish, h2_e3hRFish_master,
    h2_e4hXFish, h2_e4hXFish_master,
    h2_e4hYFish, h2_e4hYFish_master,
    h2_e4hRFish, h2_e4hRFish_master,

    // ElecMomentum
    h2_e1hPDirDistXY, h2_e1hPDirDistXY_master,
    h2_e1hPDirDistYZ, h2_e1hPDirDistYZ_master,
    h2_e1hPDirDistZX, h2_e1hPDirDistZX_master,
    h2_e1hPDirDist, h2_e1hPDirDist_master,
    h2_e2hPDirDistXY, h2_e2hPDirDistXY_master,
    h2_e2hPDirDistYZ, h2_e2hPDirDistYZ_master,
    h2_e2hPDirDistZX, h2_e2hPDirDistZX_master,
    h2_e2hPDirDist, h2_e2hPDirDist_master,
    h2_e3hPDirDistXY, h2_e3hPDirDistXY_master,
    h2_e3hPDirDistYZ, h2_e3hPDirDistYZ_master,
    h2_e3hPDirDistZX, h2_e3hPDirDistZX_master,
    h2_e3hPDirDist, h2_e3hPDirDist_master,
    h2_e4hPDirDistXY, h2_e4hPDirDistXY_master,
    h2_e4hPDirDistYZ, h2_e4hPDirDistYZ_master,
    h2_e4hPDirDistZX, h2_e4hPDirDistZX_master,
    h2_e4hPDirDist, h2_e4hPDirDist_master,

    // ElecEnergy
    h1_e1hE, h1_e1hE_master,
    h1_e2hE, h1_e2hE_master,
    h1_e3hE, h1_e3hE_master,
    h1_e4hE, h1_e4hE_master,
    h1_eTotalE, h1_eTotalE_master,

    // IonElecCorr
    h2_iKER_e1hE, h2_iKER_e1hE_master,

    // Others
    h1_i1hTOF_i2h3hNotDead,
    h2_i2h3hPIPICO_i1hMaster,
    h2_e1hE_iTotalTOF_master,

    numberOfHists
  };
  void createHists();
  void fillHists();
};
}


#endif
