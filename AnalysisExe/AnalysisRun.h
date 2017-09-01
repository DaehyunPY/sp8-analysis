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
#include "../Core/Hist.h"
#include "../Core/Unit.h"
#include "../AnalysisCore/AnalysisTools.h"
#include "../AnalysisCore/LogWriter.h"
#include <numeric>

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
  AnalysisRun(const Analysis::JSONReader &configReader);
  ~AnalysisRun();
  const long getEntries() const;
  void processEvent(const long raw);

 private:
  enum HistList {
#define __IONHISTSET__(X) X ## _always, X ## _iMaster, X ## _master
    // IonImage
    __IONHISTSET__(h2_i1hImage),
    __IONHISTSET__(h2_i2hImage),
    __IONHISTSET__(h2_i3hImage),
    __IONHISTSET__(h2_i4hImage),
    __IONHISTSET__(h2_i5hImage),
    __IONHISTSET__(h2_iCOMImage),
    __IONHISTSET__(h2_i1hImageDirDist),
    __IONHISTSET__(h2_i2hImageDirDist),
    __IONHISTSET__(h2_i3hImageDirDist),
    __IONHISTSET__(h2_i4hImageDirDist),
    __IONHISTSET__(h2_i5hImageDirDist),
    __IONHISTSET__(h2_iCOMImageDirDist),

    // IonTOF
    __IONHISTSET__(h1_i1hTOF),
    __IONHISTSET__(h1_i2hTOF),
    __IONHISTSET__(h1_i3hTOF),
    __IONHISTSET__(h1_i4hTOF),
    __IONHISTSET__(h1_i5hTOF),
    __IONHISTSET__(h1_iTotalTOF),
    __IONHISTSET__(h2_i1h2hPIPICO),
    __IONHISTSET__(h2_i2h3hPIPICO),
    __IONHISTSET__(h2_i3h4hPIPICO),
    __IONHISTSET__(h2_i4h5hPIPICO),
    __IONHISTSET__(h2_i1h2hRotPIPICO),
    __IONHISTSET__(h2_i2h3hRotPIPICO),
    __IONHISTSET__(h2_i3h4hRotPIPICO),
    __IONHISTSET__(h2_i4h5hRotPIPICO),
    __IONHISTSET__(h2_i1h2h3h3PICO),
    __IONHISTSET__(h2_i2h3h4h3PICO),
    __IONHISTSET__(h2_i3h4h5h3PICO),
    __IONHISTSET__(h2_i1h2h3h4h4PICO),
    __IONHISTSET__(h2_i2h3h4h5h4PICO),
    __IONHISTSET__(h2_i1h2h3h4h5h5PICO),

    // IonFish
    __IONHISTSET__(h2_i1hXFish),
    __IONHISTSET__(h2_i1hYFish),
    __IONHISTSET__(h2_i1hRFish),
    __IONHISTSET__(h2_i2hXFish),
    __IONHISTSET__(h2_i2hYFish),
    __IONHISTSET__(h2_i2hRFish),
    __IONHISTSET__(h2_i3hXFish),
    __IONHISTSET__(h2_i3hYFish),
    __IONHISTSET__(h2_i3hRFish),
    __IONHISTSET__(h2_i4hXFish),
    __IONHISTSET__(h2_i4hYFish),
    __IONHISTSET__(h2_i4hRFish),
    __IONHISTSET__(h2_i5hXFish),
    __IONHISTSET__(h2_i5hYFish),
    __IONHISTSET__(h2_i5hRFish),

    // IonMomentum
    __IONHISTSET__(h1_i1hPX),
    __IONHISTSET__(h1_i1hPY),
    __IONHISTSET__(h1_i1hPZ),
    __IONHISTSET__(h2_i1hPXY),
    __IONHISTSET__(h2_i1hPYZ),
    __IONHISTSET__(h2_i1hPZX),
    __IONHISTSET__(h1_i1hP),
    __IONHISTSET__(h1_i2hPX),
    __IONHISTSET__(h1_i2hPY),
    __IONHISTSET__(h1_i2hPZ),
    __IONHISTSET__(h2_i2hPXY),
    __IONHISTSET__(h2_i2hPYZ),
    __IONHISTSET__(h2_i2hPZX),
    __IONHISTSET__(h1_i2hP),
    __IONHISTSET__(h1_i3hPX),
    __IONHISTSET__(h1_i3hPY),
    __IONHISTSET__(h1_i3hPZ),
    __IONHISTSET__(h2_i3hPXY),
    __IONHISTSET__(h2_i3hPYZ),
    __IONHISTSET__(h2_i3hPZX),
    __IONHISTSET__(h1_i3hP),
    __IONHISTSET__(h1_i4hPX),
    __IONHISTSET__(h1_i4hPY),
    __IONHISTSET__(h1_i4hPZ),
    __IONHISTSET__(h2_i4hPXY),
    __IONHISTSET__(h2_i4hPYZ),
    __IONHISTSET__(h2_i4hPZX),
    __IONHISTSET__(h1_i4hP),
    __IONHISTSET__(h1_i5hPX),
    __IONHISTSET__(h1_i5hPY),
    __IONHISTSET__(h1_i5hPZ),
    __IONHISTSET__(h2_i5hPXY),
    __IONHISTSET__(h2_i5hPYZ),
    __IONHISTSET__(h2_i5hPZX),
    __IONHISTSET__(h1_i5hP),
    __IONHISTSET__(h1_iTotalPX),
    __IONHISTSET__(h1_iTotalPY),
    __IONHISTSET__(h1_iTotalPZ),
    __IONHISTSET__(h2_iTotalPXY),
    __IONHISTSET__(h2_iTotalPYZ),
    __IONHISTSET__(h2_iTotalPZX),
    __IONHISTSET__(h1_iTotalP),
    __IONHISTSET__(h2_i1h2hPX),
    __IONHISTSET__(h2_i1h2hPY),
    __IONHISTSET__(h2_i1h2hPZ),
    __IONHISTSET__(h2_i1h2hP),
    __IONHISTSET__(h2_i2h3hPX),
    __IONHISTSET__(h2_i2h3hPY),
    __IONHISTSET__(h2_i2h3hPZ),
    __IONHISTSET__(h2_i2h3hP),
    __IONHISTSET__(h2_i3h4hPX),
    __IONHISTSET__(h2_i3h4hPY),
    __IONHISTSET__(h2_i3h4hPZ),
    __IONHISTSET__(h2_i3h4hP),
    __IONHISTSET__(h2_i4h5hPX),
    __IONHISTSET__(h2_i4h5hPY),
    __IONHISTSET__(h2_i4h5hPZ),
    __IONHISTSET__(h2_i4h5hP),

    // IonMomentumAngDist
    __IONHISTSET__(h2_i1hPDirDistX),
    __IONHISTSET__(h2_i1hPDirDistY),
    __IONHISTSET__(h2_i1hPDirDistZ),
    __IONHISTSET__(h2_i1hPDirDistXY),
    __IONHISTSET__(h2_i1hPDirDistYZ),
    __IONHISTSET__(h2_i1hPDirDistZX),
    __IONHISTSET__(h2_i1hPDirDistXY_PDirZIs90),
    __IONHISTSET__(h2_i1hPDirDistYZ_PDirXIs90),
    __IONHISTSET__(h2_i1hPDirDistZX_PDirYIs90),
    __IONHISTSET__(h2_i1hPDirDist),
    __IONHISTSET__(h2_i2hPDirDistX),
    __IONHISTSET__(h2_i2hPDirDistY),
    __IONHISTSET__(h2_i2hPDirDistZ),
    __IONHISTSET__(h2_i2hPDirDistXY),
    __IONHISTSET__(h2_i2hPDirDistYZ),
    __IONHISTSET__(h2_i2hPDirDistZX),
    __IONHISTSET__(h2_i2hPDirDistZX_PDirYIs90),
    __IONHISTSET__(h2_i2hPDirDistYZ_PDirXIs90),
    __IONHISTSET__(h2_i2hPDirDistXY_PDirZIs90),
    __IONHISTSET__(h2_i2hPDirDist),
    __IONHISTSET__(h2_i3hPDirDistX),
    __IONHISTSET__(h2_i3hPDirDistY),
    __IONHISTSET__(h2_i3hPDirDistZ),
    __IONHISTSET__(h2_i3hPDirDistXY),
    __IONHISTSET__(h2_i3hPDirDistYZ),
    __IONHISTSET__(h2_i3hPDirDistZX),
    __IONHISTSET__(h2_i3hPDirDistXY_PDirZIs90),
    __IONHISTSET__(h2_i3hPDirDistYZ_PDirXIs90),
    __IONHISTSET__(h2_i3hPDirDistZX_PDirYIs90),
    __IONHISTSET__(h2_i3hPDirDist),
    __IONHISTSET__(h2_i4hPDirDistX),
    __IONHISTSET__(h2_i4hPDirDistY),
    __IONHISTSET__(h2_i4hPDirDistZ),
    __IONHISTSET__(h2_i4hPDirDistXY),
    __IONHISTSET__(h2_i4hPDirDistYZ),
    __IONHISTSET__(h2_i4hPDirDistZX),
    __IONHISTSET__(h2_i4hPDirDistXY_PDirZIs90),
    __IONHISTSET__(h2_i4hPDirDistYZ_PDirXIs90),
    __IONHISTSET__(h2_i4hPDirDistZX_PDirYIs90),
    __IONHISTSET__(h2_i4hPDirDist),
    __IONHISTSET__(h2_i5hPDirDistX),
    __IONHISTSET__(h2_i5hPDirDistY),
    __IONHISTSET__(h2_i5hPDirDistZ),
    __IONHISTSET__(h2_i5hPDirDistXY),
    __IONHISTSET__(h2_i5hPDirDistYZ),
    __IONHISTSET__(h2_i5hPDirDistZX),
    __IONHISTSET__(h2_i5hPDirDistXY_PDirZIs90),
    __IONHISTSET__(h2_i5hPDirDistYZ_PDirXIs90),
    __IONHISTSET__(h2_i5hPDirDistZX_PDirYIs90),
    __IONHISTSET__(h2_i5hPDirDist),
    __IONHISTSET__(h2_iTotalPDirDistX),
    __IONHISTSET__(h2_iTotalPDirDistY),
    __IONHISTSET__(h2_iTotalPDirDistZ),
    __IONHISTSET__(h2_iTotalPDirDistXY),
    __IONHISTSET__(h2_iTotalPDirDistYZ),
    __IONHISTSET__(h2_iTotalPDirDistZX),
    __IONHISTSET__(h2_iTotalPDirDistXY_PDirZIs90),
    __IONHISTSET__(h2_iTotalPDirDistYZ_PDirXIs90),
    __IONHISTSET__(h2_iTotalPDirDistZX_PDirYIs90),
    __IONHISTSET__(h2_iTotalPDirDist),

    // IonEnergy
    __IONHISTSET__(h1_i1hE),
    __IONHISTSET__(h1_i2hE),
    __IONHISTSET__(h1_i3hE),
    __IONHISTSET__(h1_i4hE),
    __IONHISTSET__(h1_i5hE),
    __IONHISTSET__(h1_iTotalE),
    __IONHISTSET__(h2_i1h2hE),
    __IONHISTSET__(h2_i2h3hE),
    __IONHISTSET__(h2_i3h4hE),
    __IONHISTSET__(h2_i4h5hE),

#define __ELECHISTSET__(X) X ## _always, X ## _eMaster, X ## _master
    // ElecImage
    __ELECHISTSET__(h2_e1hImage),
    __ELECHISTSET__(h2_e2hImage),
    __ELECHISTSET__(h2_e3hImage),
    __ELECHISTSET__(h2_e4hImage),
    __ELECHISTSET__(h2_e1hImageDirDist),
    __ELECHISTSET__(h2_e2hImageDirDist),
    __ELECHISTSET__(h2_e3hImageDirDist),
    __ELECHISTSET__(h2_e4hImageDirDist),

    // ElecTOF
    __ELECHISTSET__(h1_e1hTOF),
    __ELECHISTSET__(h1_e2hTOF),
    __ELECHISTSET__(h1_e3hTOF),
    __ELECHISTSET__(h1_e4hTOF),
    __ELECHISTSET__(h2_e1h2hPEPECO),
    __ELECHISTSET__(h2_e2h3hPEPECO),
    __ELECHISTSET__(h2_e3h4hPEPECO),

    // ElecFish
    __ELECHISTSET__(h2_e1hXFish),
    __ELECHISTSET__(h2_e1hYFish),
    __ELECHISTSET__(h2_e1hRFish),
    __ELECHISTSET__(h2_e2hXFish),
    __ELECHISTSET__(h2_e2hYFish),
    __ELECHISTSET__(h2_e2hRFish),
    __ELECHISTSET__(h2_e3hXFish),
    __ELECHISTSET__(h2_e3hYFish),
    __ELECHISTSET__(h2_e3hRFish),
    __ELECHISTSET__(h2_e4hXFish),
    __ELECHISTSET__(h2_e4hYFish),
    __ELECHISTSET__(h2_e4hRFish),

    // ElecMomentum
    __ELECHISTSET__(h1_e1hPX),
    __ELECHISTSET__(h1_e1hPY),
    __ELECHISTSET__(h1_e1hPZ),
    __ELECHISTSET__(h2_e1hPXY),
    __ELECHISTSET__(h2_e1hPYZ),
    __ELECHISTSET__(h2_e1hPZX),
    __ELECHISTSET__(h1_e1hP),
    __ELECHISTSET__(h1_e2hPX),
    __ELECHISTSET__(h1_e2hPY),
    __ELECHISTSET__(h1_e2hPZ),
    __ELECHISTSET__(h2_e2hPXY),
    __ELECHISTSET__(h2_e2hPYZ),
    __ELECHISTSET__(h2_e2hPZX),
    __ELECHISTSET__(h1_e2hP),
    __ELECHISTSET__(h1_e3hPX),
    __ELECHISTSET__(h1_e3hPY),
    __ELECHISTSET__(h1_e3hPZ),
    __ELECHISTSET__(h2_e3hPXY),
    __ELECHISTSET__(h2_e3hPYZ),
    __ELECHISTSET__(h2_e3hPZX),
    __ELECHISTSET__(h1_e3hP),
    __ELECHISTSET__(h1_e4hPX),
    __ELECHISTSET__(h1_e4hPY),
    __ELECHISTSET__(h1_e4hPZ),
    __ELECHISTSET__(h2_e4hPXY),
    __ELECHISTSET__(h2_e4hPYZ),
    __ELECHISTSET__(h2_e4hPZX),
    __ELECHISTSET__(h1_e4hP),

    // ElecMomentumAngDist
    __ELECHISTSET__(h2_e1hPDirDistX),
    __ELECHISTSET__(h2_e1hPDirDistY),
    __ELECHISTSET__(h2_e1hPDirDistZ),
    __ELECHISTSET__(h2_e1hPDirDistXY),
    __ELECHISTSET__(h2_e1hPDirDistYZ),
    __ELECHISTSET__(h2_e1hPDirDistZX),
    __ELECHISTSET__(h2_e1hPDirDistXY_PDirZIs90),
    __ELECHISTSET__(h2_e1hPDirDistYZ_PDirXIs90),
    __ELECHISTSET__(h2_e1hPDirDistZX_PDirYIs90),
    __ELECHISTSET__(h2_e1hPDirDist),
    __ELECHISTSET__(h2_e2hPDirDistX),
    __ELECHISTSET__(h2_e2hPDirDistY),
    __ELECHISTSET__(h2_e2hPDirDistZ),
    __ELECHISTSET__(h2_e2hPDirDistXY),
    __ELECHISTSET__(h2_e2hPDirDistYZ),
    __ELECHISTSET__(h2_e2hPDirDistZX),
    __ELECHISTSET__(h2_e2hPDirDistXY_PDirZIs90),
    __ELECHISTSET__(h2_e2hPDirDistYZ_PDirXIs90),
    __ELECHISTSET__(h2_e2hPDirDistZX_PDirYIs90),
    __ELECHISTSET__(h2_e2hPDirDist),
    __ELECHISTSET__(h2_e3hPDirDistX),
    __ELECHISTSET__(h2_e3hPDirDistY),
    __ELECHISTSET__(h2_e3hPDirDistZ),
    __ELECHISTSET__(h2_e3hPDirDistXY),
    __ELECHISTSET__(h2_e3hPDirDistYZ),
    __ELECHISTSET__(h2_e3hPDirDistZX),
    __ELECHISTSET__(h2_e3hPDirDistXY_PDirZIs90),
    __ELECHISTSET__(h2_e3hPDirDistYZ_PDirXIs90),
    __ELECHISTSET__(h2_e3hPDirDistZX_PDirYIs90),
    __ELECHISTSET__(h2_e3hPDirDist),
    __ELECHISTSET__(h2_e4hPDirDistX),
    __ELECHISTSET__(h2_e4hPDirDistY),
    __ELECHISTSET__(h2_e4hPDirDistZ),
    __ELECHISTSET__(h2_e4hPDirDistXY),
    __ELECHISTSET__(h2_e4hPDirDistYZ),
    __ELECHISTSET__(h2_e4hPDirDistZX),
    __ELECHISTSET__(h2_e4hPDirDistXY_PDirZIs90),
    __ELECHISTSET__(h2_e4hPDirDistYZ_PDirXIs90),
    __ELECHISTSET__(h2_e4hPDirDistZX_PDirYIs90),
    __ELECHISTSET__(h2_e4hPDirDist),

    // ElecEnergy
    __ELECHISTSET__(h1_e1hE),
    __ELECHISTSET__(h1_e2hE),
    __ELECHISTSET__(h1_e3hE),
    __ELECHISTSET__(h1_e4hE),
    __ELECHISTSET__(h1_eTotalE),
    __ELECHISTSET__(h2_e1h2hE),
    __ELECHISTSET__(h2_e2h3hE),
    __ELECHISTSET__(h2_e3h4hE),

#define __CORRHISTSET__(X) X ## _always, X ## _master
    // IonElecCorr
    __CORRHISTSET__(h2_iKER_e1hE),

    // Others
    h1_i1hTOF_i2h3hNotDead,
    h2_i2h3hPIPICO_i1hMaster,
    h2_e1hE_iTotalTOF_master,

    // end
    numberOfHists
  };
  void createHists();
  void fillHists();
};
}


#endif
