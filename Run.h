//
// Created by Daehyun You on 12/30/15.
//

#ifndef BL17ANALYSIS_RUN_H
#define BL17ANALYSIS_RUN_H

#define SAMETITLEWITH(X) X, #X

#define H1_SINCOS 1000, -1, 1
#define H2_SINCOS 100, -1, 1
#define H3_SINCOS 25, -1, 1
#define H1_DEGREE 1800, -180, 180
#define H2_DEGREE 180, -180, 180
#define H3_DEGREE 45, -180, 180

#define H1_ION_LOCATION 1000, -50, 50
#define H2_ION_LOCATION 100, -50, 50
#define H3_ION_LOCATION 25, -50, 50
#define H1_ION_TOF 2000, 0, 10000
#define H2_ION_TOF 500, 0, 10000
#define H3_ION_TOF 100, 0, 10000
#define H1_ION_SUMOFTOF(X) 1000*X, 0*X, 10000*X
#define H2_ION_SUMOFTOF(X) 100*X, 0*X, 10000*X
#define H3_ION_SUMOFTOF(X) 25*X, 0*X, 10000*X
#define H1_ION_DIFFOFTOF 2000, 0, 10000
#define H2_ION_DIFFOFTOF 500, 0, 10000
#define H3_ION_DIFFOFTOF 100, 0, 10000
#define H1_ION_MOMENTUM 1000, -250, 250
#define H2_ION_MOMENTUM 100, -250, 250
#define H3_ION_MOMENTUM 25, -250, 250
#define H1_ION_ENERGY 1000, 0, 50
#define H2_ION_ENERGY 100, 0, 50
#define H3_ION_ENERGY 25, 0, 50
#define H1_ION_SUMOFENERGY(X) 1000*X, 0*X, 50*X
#define H2_ION_SUMOFENERGY(X) 100*X, 0*X, 50*X
#define H3_ION_SUMOFENERGY(X) 25*X, 0*X, 50*X
#define H1_Ion_TOTALENERGY 4000, 0, 150
#define H2_Ion_TOTALENERGY 400, 0, 150
#define H3_Ion_TOTALENERGY 100, 0, 150

#define H1_ELECTRON_LOCATION 1000, -75, 75
#define H2_ELECTRON_LOCATION 100, -75, 75
#define H3_ELECTRON_LOCATION 25, -75, 75
#define H1_ELECTRON_RADIUS 1000, 0, 75
#define H2_ELECTRON_RADIUS 100, 0, 75
#define H3_ELECTRON_RADIUS 25, 0, 75
#define H1_ELECTRON_TOF 1000, -20, 130
#define H2_ELECTRON_TOF 100, -20, 130
#define H3_ELECTRON_TOF 25, -20, 130
#define H1_ELECTRON_SUMOFTOF(X) 1000*X, -20*X, 130*X
#define H2_ELECTRON_SUMOFTOF(X) 100*X, -20*X, 130*X
#define H3_ELECTRON_SUMOFTOF(X) 25*X, -20*X, 130*X
#define H1_ELECTRON_MOMENTUM 1000, -2.5, 2.5
#define H2_ELECTRON_MOMENTUM 100, -2.5, 2.5
#define H3_ELECTRON_MOMENTUM 25, -2.5, 2.5
#define H1_ELECTRON_NORMOFMOMENTUM 1000, 0, 2.5
#define H2_ELECTRON_NORMOFMOMENTUM 100, 0, 2.5
#define H3_ELECTRON_NORMOFMOMENTUM 25, 0, 2.5
#define H1_ELECTRON_ENERGY 1000, 0, 50
#define H2_ELECTRON_ENERGY 100, 0, 50
#define H3_ELECTRON_ENERGY 25, 0, 50

#include <string>

#include <TFile.h>
#include <TChain.h>
#include <TH1F.h>
#include <TH2F.h>

#include "OutputFlag.h"
#include "Core/Unit.h"
#include "Core/AnalysisTools.h"
#include "Core/Ions.h"
#include "Core/Electrons.h"
#include "Core/LogWriter.h"
#include "OutputHist.h"

namespace Analysis {
class Run {
  int numberOfHits;
  TFile *pRootFile;
  TChain *pEventChain;
  TChain *pAnalyzedChain;
  Analysis::Unit *pUnit;
  Analysis::AnalysisTools *pTools;
  Analysis::Ions *pIons;
  Analysis::Electrons *pElectrons;
  Analysis::EventDataReader *pEventReader;
  Analysis::LogWriter *pLogWriter;
  Analysis::OutputFlag flag;

 public:
  Run(const std::string configFilename = "Parameters.json");
  ~Run();
  const long getEntries() const;
  void processEvent(const long raw);

 private:
  OutputHist *pHist;
  const char *dirNameOfFlagHists = "Flag";
  enum FlagHists {
    numberOfFlagHists = 0
  };
  const char *dirNameOfIonHists = "Ion";
  enum IonHists {
    // Detector image
    hist2ID_1stHitIonLocXY_notDead = numberOfFlagHists,
    hist2ID_2ndHitIonLocXY_notDead,
    hist2ID_3rdHitIonLocXY_notDead,
    hist2ID_4thHitIonLocXY_notDead,
    hist2ID_COMOfIonsLocXY_notDead,
    hist2ID_1stHitIonLocXY_master,
    hist2ID_2ndHitIonLocXY_master,
    hist2ID_3rdHitIonLocXY_master,
    hist2ID_4thHitIonLocXY_master,
    hist2ID_COMOfIonsLocXY_master,
    // TOF
    hist1ID_1stHitIonTOF_raw,
    hist1ID_2ndHitIonTOF_raw,
    hist1ID_3rdHitIonTOF_raw,
    hist1ID_4thHitIonTOF_raw,
    hist1ID_1stHitIonTOF_notDead,
    hist1ID_2ndHitIonTOF_notDead,
    hist1ID_3rdHitIonTOF_notDead,
    hist1ID_4thHitIonTOF_notDead,
    hist1ID_1stHitIonTOF_master,
    hist1ID_2ndHitIonTOF_master,
    hist1ID_3rdHitIonTOF_master,
    hist1ID_4thHitIonTOF_master,
    // PIPICO
    hist2ID_1stAnd2ndHitIonTOF_notDead,
    hist2ID_2ndAnd3rdHitIonTOF_notDead,
    hist2ID_3rdAnd4thHitIonTOF_notDead,
    hist2ID_1stAnd2ndHitIonTOF_rot45NotDead,
    hist2ID_2ndAnd3rdHitIonTOF_rot45NotDead,
    hist2ID_3rdAnd4thHitIonTOF_rot45NotDead,
    hist2ID_1stAnd2ndHitIonTOF_master,
    hist2ID_2ndAnd3rdHitIonTOF_master,
    hist2ID_3rdAnd4thHitIonTOF_master,
    // PIPIPICO
    hist2ID_SumOf1stAnd2ndHitIonTOFsAnd3rdHitIonTOF,
    hist2ID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs,
    hist2ID_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF,
    hist2ID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs,
    hist2ID_SumOf1stAnd2ndHitIonTOFsAnd3rdHitIonTOF_masterCondit,
    hist2ID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOFs_masterCondit,
    hist2ID_SumOf2ndAnd3rdHitIonTOFsAnd4thHitIonTOF_masterCondit,
    hist2ID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOFs_masterCondit,
    // PIPIPIPICO
    hist2ID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF,
    hist2ID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF,
    hist2ID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF,
    hist2ID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF_masterCondit,
    hist2ID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF_masterCondit,
    hist2ID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF_masterCondit,
    // Momentum
    histID_1stHitIonPxPy,
    histID_2ndHitIonPxPy,
    histID_3rdHitIonPxPy,
    histID_4thHitIonPxPy,
    histID_1stHitIonPz,
    histID_2ndHitIonPz,
    histID_3rdHitIonPz,
    histID_4thHitIonPz,
    hist3ID_1stHitIonPxPyPz,
    hist3ID_1stHitIonPxPyPz_underMasterCondition,
    hist2ID_1stHitIonPDirecXYAndCosPDirecZ,
    hist2ID_2ndHitIonPDirecXYAndCosPDirecZ,
    hist2ID_3rdHitIonPDirecXYAndCosPDirecZ,
    hist2ID_4thHitIonPDirecXYAndCosPDirecZ,
    hist1ID_1stHitIonE,
    hist1ID_2ndHitIonE,
    hist1ID_3rdHitIonE,
    hist1ID_4thHitIonE,
    hist1ID_SumOfIonEs,
    numberOfIonHists
  };
  void createIonHists();
  void fillIonHists();
  const char *dirNameOfElecHists = "Electron";
  enum ElecHists {
	// Detector image
	hist2ID_1stHitElecLocXY_notDead = numberOfIonHists,
	hist2ID_2ndHitElecLocXY_notDead,
	hist2ID_3rdHitElecLocXY_notDead,
	hist2ID_4thHitElecLocXY_notDead,
	hist2ID_1stHitElecLocXY_master,
	hist2ID_2ndHitElecLocXY_master,
	hist2ID_3rdHitElecLocXY_master,
	hist2ID_4thHitElecLocXY_master,
	hist2ID_1stHitElecLocDirXY_Loc_notDead, 
    // TOF
    hist1ID_1stHitElecTOF_raw,
    hist1ID_2ndHitElecTOF_raw,
    hist1ID_3rdHitElecTOF_raw,
    hist1ID_4thHitElecTOF_raw,
    hist1ID_1stHitElecTOF_notDead,
    hist1ID_2ndHitElecTOF_notDead,
    hist1ID_3rdHitElecTOF_notDead,
    hist1ID_4thHitElecTOF_notDead,
    hist1ID_1stHitElecTOF_master,
    hist1ID_2ndHitElecTOF_master,
    hist1ID_3rdHitElecTOF_master,
    hist1ID_4thHitElecTOF_master,
    // PIPICO
    hist2ID_1stAnd2ndHitElecTOF_notDead,
    hist2ID_2ndAnd3rdHitElecTOF_notDead,
    hist2ID_3rdAnd4thHitElecTOF_notDead,
    hist2ID_1stAnd2ndHitElecTOF_master,
    hist2ID_2ndAnd3rdHitElecTOF_master,
    hist2ID_3rdAnd4thHitElecTOF_master,
    // FISH 
	// Momentum 
	hist2ID_1stHitElecPxPy_notDead,
	hist2ID_2ndHitElecPxPy_notDead,
	hist2ID_3rdHitElecPxPy_notDead,
	hist2ID_4thHitElecPxPy_notDead,
	hist1ID_1stHitElecPz_notDead,
	hist1ID_2ndHitElecPz_notDead,
	hist1ID_3rdHitElecPz_notDead,
	hist1ID_4thHitElecPz_notDead,
	hist2ID_1stHitElecPxPy_master,
  hist2ID_2ndHitElecPxPy_master,
  hist2ID_3rdHitElecPxPy_master,
  hist2ID_4thHitElecPxPy_master,
  hist1ID_1stHitElecPz_master,
  hist1ID_2ndHitElecPz_master,
  hist1ID_3rdHitElecPz_master,
  hist1ID_4thHitElecPz_master,
  // Momentum Direction
  hist2ID_1stHitElecPDirXYAndPXY_notDead,
  hist2ID_1stHitElecPDirYZAndPYZ_notDead,
  hist2ID_1stHitElecPDirZXAndPZX_notDead,
	hist2ID_1stHitElecPDirXYAndCosPDirZ_notDead,
  hist2ID_1stHitElecPDirXYAndPXY_master,
  hist2ID_1stHitElecPDirYZAndPYZ_master,
  hist2ID_1stHitElecPDirZXAndPZX_master,
  hist2ID_1stHitElecPDirXYAndCosPDirZ_master,
  // Energy 
	hist1ID_1stHitElecE,
	hist1ID_2ndHitElecE,
	hist1ID_3rdHitElecE,
	hist1ID_4thHitElecE,
	hist1ID_SumOfElecEs,
	//
    numberOfElecHists
  };
  void createElecHists(); 
  void fillElecHists();
  const char *dirNameOfNatureHists = "Nature";
  enum NatureHists {
    histID_1stHitIonTOF_under2ndAnd3rdHitIonAreNotDead = numberOfElecHists,
    histID_2ndAnd3rdHitIonTOF_under1stHitIonIsInMasterRegion,
    histID_1stHitElecEAndSumOfIonTOFs_underMasterCondition,
    histID_1stHitElecE_underMasterCondition,
    numberOfNatureHists
  };
  void createNatureHists();
  void fillNatureHists();
  const int numberOfHists = numberOfNatureHists;
  void flushHist();
};
}

#endif //BL17ANALYSIS_BL17ANALYSIS_H
