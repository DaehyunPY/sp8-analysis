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
#include "../Protocols/Hist.h"
#include "../Protocols/Unit.h"
#include "../AnalysisCore/AnalysisTools.h"
#include "../AnalysisCore/LogWriter.h"

namespace Analysis {
class AnalysisRun: Hist {
  int maxNumOfIonHits;
  int maxNumOfElecHits;
  TChain *pEventChain;
  Analysis::AnalysisTools *pTools;
  Analysis::Objects *pIons;
  Analysis::Objects *pElectrons;
  Analysis::EventDataReader *pEventReader;
  Analysis::LogWriter *pLogWriter;

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
    h2_i1hImageDirDist, h2_i1hImageDirDist_master,
    h2_i2hImageDirDist, h2_i2hImageDirDist_master,
    h2_i3hImageDirDist, h2_i3hImageDirDist_master,
    h2_i4hImageDirDist, h2_i4hImageDirDist_master,
    h2_iCOMImageDirDist, h2_iCOMImageDirDist_master,

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
        h1_i1hPX, h1_i1hPX_master,
    h1_i1hPY, h1_i1hPY_master,
    h1_i1hPZ, h1_i1hPZ_master,
    h2_i1hPXY, h2_i1hPXY_master,
    h2_i1hPYZ, h2_i1hPYZ_master,
    h2_i1hPZX, h2_i1hPZX_master,
    h1_i1hP, h1_i1hP_master,
    h1_i2hPX, h1_i2hPX_master,
    h1_i2hPY, h1_i2hPY_master,
    h1_i2hPZ, h1_i2hPZ_master,
    h2_i2hPXY, h2_i2hPXY_master,
    h2_i2hPYZ, h2_i2hPYZ_master,
    h2_i2hPZX, h2_i2hPZX_master,
    h1_i2hP, h1_i2hP_master,
    h1_i3hPX, h1_i3hPX_master,
    h1_i3hPY, h1_i3hPY_master,
    h1_i3hPZ, h1_i3hPZ_master,
    h2_i3hPXY, h2_i3hPXY_master,
    h2_i3hPYZ, h2_i3hPYZ_master,
    h2_i3hPZX, h2_i3hPZX_master,
    h1_i3hP, h1_i3hP_master,
    h1_i4hPX, h1_i4hPX_master,
    h1_i4hPY, h1_i4hPY_master,
    h1_i4hPZ, h1_i4hPZ_master,
    h2_i4hPXY, h2_i4hPXY_master,
    h2_i4hPYZ, h2_i4hPYZ_master,
    h2_i4hPZX, h2_i4hPZX_master,
    h1_i4hP, h1_i4hP_master,
    h1_iTotalPX, h1_iTotalPX_master,
    h1_iTotalPY, h1_iTotalPY_master,
    h1_iTotalPZ, h1_iTotalPZ_master,
    h2_iTotalPXY, h2_iTotalPXY_master,
    h2_iTotalPYZ, h2_iTotalPYZ_master,
    h2_iTotalPZX, h2_iTotalPZX_master,
    h1_iTotalP, h1_iTotalP_master,
    h2_i1hPDirDistX, h2_i1hPDirDistX_master,
    h2_i1hPDirDistY, h2_i1hPDirDistY_master,
    h2_i1hPDirDistZ, h2_i1hPDirDistZ_master,
    h2_i1hPDirDistXY, h2_i1hPDirDistXY_master,
    h2_i1hPDirDistYZ, h2_i1hPDirDistYZ_master,
    h2_i1hPDirDistZX, h2_i1hPDirDistZX_master,
    h2_i1hPDirDist, h2_i1hPDirDist_master,
    h2_i2hPDirDistX, h2_i2hPDirDistX_master,
    h2_i2hPDirDistY, h2_i2hPDirDistY_master,
    h2_i2hPDirDistZ, h2_i2hPDirDistZ_master,
    h2_i2hPDirDistXY, h2_i2hPDirDistXY_master,
    h2_i2hPDirDistYZ, h2_i2hPDirDistYZ_master,
    h2_i2hPDirDistZX, h2_i2hPDirDistZX_master,
    h2_i2hPDirDist, h2_i2hPDirDist_master,
    h2_i3hPDirDistX, h2_i3hPDirDistX_master,
    h2_i3hPDirDistY, h2_i3hPDirDistY_master,
    h2_i3hPDirDistZ, h2_i3hPDirDistZ_master,
    h2_i3hPDirDistXY, h2_i3hPDirDistXY_master,
    h2_i3hPDirDistYZ, h2_i3hPDirDistYZ_master,
    h2_i3hPDirDistZX, h2_i3hPDirDistZX_master,
    h2_i3hPDirDist, h2_i3hPDirDist_master,
    h2_i4hPDirDistX, h2_i4hPDirDistX_master,
    h2_i4hPDirDistY, h2_i4hPDirDistY_master,
    h2_i4hPDirDistZ, h2_i4hPDirDistZ_master,
    h2_i4hPDirDistXY, h2_i4hPDirDistXY_master,
    h2_i4hPDirDistYZ, h2_i4hPDirDistYZ_master,
    h2_i4hPDirDistZX, h2_i4hPDirDistZX_master,
    h2_i4hPDirDist, h2_i4hPDirDist_master,
    h2_iTotalPDirDistX, h2_iTotalPDirDistX_master,
    h2_iTotalPDirDistY, h2_iTotalPDirDistY_master,
    h2_iTotalPDirDistZ, h2_iTotalPDirDistZ_master,
    h2_iTotalPDirDistXY, h2_iTotalPDirDistXY_master,
    h2_iTotalPDirDistYZ, h2_iTotalPDirDistYZ_master,
    h2_iTotalPDirDistZX, h2_iTotalPDirDistZX_master,
    h2_iTotalPDirDist, h2_iTotalPDirDist_master,
    h2_i1h2hP, h2_i1h2hP_master,
    h2_i2h3hP, h2_i2h3hP_master,
    h2_i3h4hP, h2_i3h4hP_master,

    // IonEnergy
        h1_i1hE, h1_i1hE_master,
    h1_i2hE, h1_i2hE_master,
    h1_i3hE, h1_i3hE_master,
    h1_i4hE, h1_i4hE_master,
    h1_iTotalE, h1_iTotalE_master,
    h2_i1hEDirDistXY, h2_i1hEDirDistXY_master,
    h2_i1hEDirDistYZ, h2_i1hEDirDistYZ_master,
    h2_i1hEDirDistZX, h2_i1hEDirDistZX_master,
    h2_i2hEDirDistXY, h2_i2hEDirDistXY_master,
    h2_i2hEDirDistYZ, h2_i2hEDirDistYZ_master,
    h2_i2hEDirDistZX, h2_i2hEDirDistZX_master,
    h2_i3hEDirDistXY, h2_i3hEDirDistXY_master,
    h2_i3hEDirDistYZ, h2_i3hEDirDistYZ_master,
    h2_i3hEDirDistZX, h2_i3hEDirDistZX_master,
    h2_i4hEDirDistXY, h2_i4hEDirDistXY_master,
    h2_i4hEDirDistYZ, h2_i4hEDirDistYZ_master,
    h2_i4hEDirDistZX, h2_i4hEDirDistZX_master,
    h2_iTotalEDirDistXY, h2_iTotalEDirDistXY_master,
    h2_iTotalEDirDistYZ, h2_iTotalEDirDistYZ_master,
    h2_iTotalEDirDistZX, h2_iTotalEDirDistZX_master,
    h2_i1h2hE, h2_i1h2hE_master,
    h2_i2h3hE, h2_i2h3hE_master,
    h2_i3h4hE, h2_i3h4hE_master,

    // ElecImage
        h2_e1hImage, h2_e1hImage_master,
    h2_e2hImage, h2_e2hImage_master,
    h2_e3hImage, h2_e3hImage_master,
    h2_e4hImage, h2_e4hImage_master,
    h2_e1hImageDirDist, h2_e1hImageDirDist_master,
    h2_e2hImageDirDist, h2_e2hImageDirDist_master,
    h2_e3hImageDirDist, h2_e3hImageDirDist_master,
    h2_e4hImageDirDist, h2_e4hImageDirDist_master,

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
        h1_e1hPX, h1_e1hPX_master,
    h1_e1hPY, h1_e1hPY_master,
    h1_e1hPZ, h1_e1hPZ_master,
    h2_e1hPXY, h2_e1hPXY_master,
    h2_e1hPYZ, h2_e1hPYZ_master,
    h2_e1hPZX, h2_e1hPZX_master,
    h1_e1hP, h1_e1hP_master,
    h1_e2hPX, h1_e2hPX_master,
    h1_e2hPY, h1_e2hPY_master,
    h1_e2hPZ, h1_e2hPZ_master,
    h2_e2hPXY, h2_e2hPXY_master,
    h2_e2hPYZ, h2_e2hPYZ_master,
    h2_e2hPZX, h2_e2hPZX_master,
    h1_e2hP, h1_e2hP_master,
    h1_e3hPX, h1_e3hPX_master,
    h1_e3hPY, h1_e3hPY_master,
    h1_e3hPZ, h1_e3hPZ_master,
    h2_e3hPXY, h2_e3hPXY_master,
    h2_e3hPYZ, h2_e3hPYZ_master,
    h2_e3hPZX, h2_e3hPZX_master,
    h1_e3hP, h1_e3hP_master,
    h1_e4hPX, h1_e4hPX_master,
    h1_e4hPY, h1_e4hPY_master,
    h1_e4hPZ, h1_e4hPZ_master,
    h2_e4hPXY, h2_e4hPXY_master,
    h2_e4hPYZ, h2_e4hPYZ_master,
    h2_e4hPZX, h2_e4hPZX_master,
    h1_e4hP, h1_e4hP_master,
    h2_e1hPDirDistX, h2_e1hPDirDistX_master,
    h2_e1hPDirDistY, h2_e1hPDirDistY_master,
    h2_e1hPDirDistZ, h2_e1hPDirDistZ_master,
    h2_e1hPDirDistXY, h2_e1hPDirDistXY_master,
    h2_e1hPDirDistYZ, h2_e1hPDirDistYZ_master,
    h2_e1hPDirDistZX, h2_e1hPDirDistZX_master,
    h2_e1hPDirDist, h2_e1hPDirDist_master,
    h2_e2hPDirDistX, h2_e2hPDirDistX_master,
    h2_e2hPDirDistY, h2_e2hPDirDistY_master,
    h2_e2hPDirDistZ, h2_e2hPDirDistZ_master,
    h2_e2hPDirDistXY, h2_e2hPDirDistXY_master,
    h2_e2hPDirDistYZ, h2_e2hPDirDistYZ_master,
    h2_e2hPDirDistZX, h2_e2hPDirDistZX_master,
    h2_e2hPDirDist, h2_e2hPDirDist_master,
    h2_e3hPDirDistX, h2_e3hPDirDistX_master,
    h2_e3hPDirDistY, h2_e3hPDirDistY_master,
    h2_e3hPDirDistZ, h2_e3hPDirDistZ_master,
    h2_e3hPDirDistXY, h2_e3hPDirDistXY_master,
    h2_e3hPDirDistYZ, h2_e3hPDirDistYZ_master,
    h2_e3hPDirDistZX, h2_e3hPDirDistZX_master,
    h2_e3hPDirDist, h2_e3hPDirDist_master,
    h2_e4hPDirDistX, h2_e4hPDirDistX_master,
    h2_e4hPDirDistY, h2_e4hPDirDistY_master,
    h2_e4hPDirDistZ, h2_e4hPDirDistZ_master,
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
