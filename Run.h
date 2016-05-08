//
// Created by Daehyun You on 12/30/15.
//

#ifndef BL17ANALYSIS_RUN_H
#define BL17ANALYSIS_RUN_H

#define H1_DEGREE 1800, -180, 180
#define H2_DEGREE 180, -180, 180
#define H3_DEGREE 45, -180, 180

#define H1_ION_LOCATION 1000, -50, 50
#define H2_ION_LOCATION 100, -50, 50
#define H3_ION_LOCATION 25, -50, 50
#define H1_ION_TOF 1000, -2000, 13000
#define H2_ION_TOF 100, 0, 10000
#define H3_ION_TOF 25, 0, 10000
#define H1_ION_SUMOFTOF 4000, 0, 30000
#define H2_ION_SUMOFTOF 400, 0, 30000
#define H3_ION_SUMOFTOF 100, 0, 30000
#define H1_ION_MOMENTUM 1000, -250, 250
#define H2_ION_MOMENTUM 100, -250, 250
#define H3_ION_MOMENTUM 25, -250, 250
#define H1_ION_ENERGY 1000, 0, 50
#define H2_ION_ENERGY 100, 0, 50
#define H3_ION_ENERGY 25, 0, 50
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
  void processEvent(const long raw);
  const Analysis::Unit &getUnit() const;
  const Analysis::Ions &getIons() const;
  const Analysis::Electrons &getElectrons() const;
  const int &getNumberOfHitsUsed() const;
  const long getEntries() const;

 private:
  OutputHist *pHist;
  //
  const char *dirNameOfIonHist = "Ion";
  enum IonHists {
    histID_1stHitIonLocXY = 0,
    histID_2ndHitIonLocXY,
    histID_3rdHitIonLocXY,
    histID_4thHitIonLocXY,
    histID_COMOfIonsLocXY,
    histID_1stHitIonTOF,
    histID_2ndHitIonTOF,
    histID_3rdHitIonTOF,
    histID_4thHitIonTOF,
    histID_1stAnd2ndHitIonTOF,
    histID_2ndAnd3rdHitIonTOF,
    histID_3rdAnd4thHitIonTOF,
    histID_SumOf1stAnd2ndHitIonTOFAnd3rdHitIonTOF,
    histID_1stHitIonTOFAndSumOf2ndAnd3rdHitIonTOF,
    histID_SumOf2ndAnd3rdHitIonTOFAnd4thHitIonTOF,
    histID_2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF,
    histID_SumOf1st2ndAnd3rdHitIonTOFAnd4thHitIonTOF,
    histID_SumOf1stAnd2ndHitIonTOFAndSumOf3rdAnd4thHitIonTOF,
    histID_1stHitIonTOFAndSumOf2nd3rdAnd4thHitIonTOF,
    histID_1stHitIonPxPy,
    histID_2ndHitIonPxPy,
    histID_3rdHitIonPxPy,
    histID_4thHitIonPxPy,
    histID_1stHitIonPz,
    histID_2ndHitIonPz,
    histID_3rdHitIonPz,
    histID_4thHitIonPz,
    histID_1stHistIonPxPyPz,
    histID_1stHistIonPxPyPz_underMasterCondition,
    histID_1stHitIonE,
    histID_2ndHitIonE,
    histID_3rdHitIonE,
    histID_4thHitIonE,
    histID_SumOfIonsE,
    numberOfIonHists
  };
  void createIonHists();
  void fillIonHists();
  //
  const char *dirNameOfElecHists = "Electron";
  enum ElecHists {
    hist2ID_1stAnd2ndHitElecE_underMasterCondition = numberOfIonHists,
    numberOfElecHists
  };
  //
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
  //
  const int numberOfHists = numberOfNatureHists;
  void flushHist();
  //
  // delete blows;
  void fillFlags();
  void fillIonBasicData();
  void fillIonMomentumData();
  void fillElectronBasicData();
  void fillElectronMomentumData();
//  void fillIonAndElectronBasicData();
  void fillIonAndElectronMomentumData();
  void writeFlags();
  void writeIonBasicData();
  void writeIonMomentumData();
  void writeElectronBasicData();
  void writeElectronMomentumData();
//  void writeIonAndElectronBasicData();
  void writeIonAndElectronMomentumData();
  // todo: add master spectra
  // ion
  // location
  TH2F root2DHistogramOf1stHitIonLocationX_LocationY
      {"i1HitX_Y",
       "1st Hit Ion Location Image;Location X [mm];Location Y [mm]",
       H2_ION_LOCATION,
       H2_ION_LOCATION};
  TH2F root2DHistogramOf2ndHitIonLocationX_LocationY
      {"i2HitX_Y",
       "2nd Hit Ion Location Image;Location X [mm];Location Y [mm]",
       H2_ION_LOCATION,
       H2_ION_LOCATION};
  TH2F root2DHistogramOf3rdHitIonLocationX_LocationY
      {"i3HitX_Y",
       "3rd Hit Ion Location Image;Location X [mm];Location Y [mm]",
       H2_ION_LOCATION,
       H2_ION_LOCATION};
  TH2F root2DHistogramOf4thHitIonLocationX_LocationY
      {"i4HitX_Y",
       "4th Hit Ion Location Image;Location X [mm];Location Y [mm]",
       H2_ION_LOCATION,
       H2_ION_LOCATION};
  TH2F root2DHistogramOfIonsCOMLocationX_LocationY
      {"iCOMX_Y",
       "Ions COM Location Image;Location X [mm];Location Y [mm]",
       H2_ION_LOCATION,
       H2_ION_LOCATION};
  // TOF
  TH1F root1DHistogramOf1stHitIonTOF
      {"i1HitTOF",
       "1st Hit Ion TOF;TOF [ns];Count [1]",
       H1_ION_TOF};
  TH1F root1DHistogramOf2ndHitIonTOF
      {"i2HitTOF",
       "2nd Hit Ion TOF;TOF [ns];Count [1]",
       H1_ION_TOF};
  TH1F root1DHistogramOf3rdHitIonTOF
      {"i3HitTOF",
       "3rd Hit Ion TOF;TOF [ns];Count [1]",
       H1_ION_TOF};
  TH1F root1DHistogramOf4thHitIonTOF
      {"i4HitTOF",
       "4th Hit Ion TOF;TOF [ns];Count [1]",
       H1_ION_TOF};
  TH2F root2DHistogramOf1stHitIonTOF_2ndHitIonTOF
      {"i1Hit_2HitTOF",
       "1st Hit and 2nd Hit Ion PIPICO;1st Hit Ion TOF [ns];2nd Hit Ion TOF [ns]",
       H2_ION_TOF,
       H2_ION_TOF};
  TH2F root2DHistogramOf2ndHitIonTOF_3rdHitIonTOF
      {"i2Hit_3HitTOF",
       "2nd Hit and 3rd Hit Ion PIPICO;2nd Hit Ion TOF [ns];3rd Hit Ion TOF [ns]",
       H2_ION_TOF,
       H2_ION_TOF};
  TH2F root2DHistogramOf2ndHitIonTOF_3rdHitIonTOF_under1stHitIonMaster
      {"i2Hit_3HitTOF_under1HitMaster",
       "2nd Hit and 3rd Hit Ion PIPICO under 1st Hit Ion is in Master Region;2nd Hit Ion TOF [ns];3rd Hit Ion TOF [ns]",
       H2_ION_TOF,
       H2_ION_TOF};
  TH2F root2DHistogramOf3rdHitIonTOF_4thHitIonTOF
      {"i3Hit_4HitTOF",
       "3rd Hit Ion 4th Hit Ion PIPICO;3rd Hit Ion TOF [ns];4th Hit Ion TOF [ns]",
       H2_ION_TOF,
       H2_ION_TOF};
  TH2F root2DHistogramOf1stHitIonTOFPlus2ndHitIonTOF_3rdHitIonTOF
      {"i1HitPlus2Hit_3HitTOF",
       "1st Hit, 2nd Hit and 3rd Hit Ion PIPIPICO;1st Hit Ion TOF + 2nd Hit Ion TOF [ns];3rd Hit Ion TOF [ns]",
       H2_ION_SUMOFTOF,
       H2_ION_TOF};
  TH2F root2DHistogramOf2ndHitIonTOFPlus3rdHitIonTOF_4thHitIonTOF
      {"i2HitPlus3Hit_4HitTOF",
       "2nd Hit, 3rd Hit and 4th Hit Ion PIPIPICO;2nd Hit Ion TOF + 3rd Hit Ion TOF [ns];4th Hit Ion TOF [ns]",
       H2_ION_SUMOFTOF,
       H2_ION_TOF};
  // momentum
  TH2F root2DHistogramOf1stHitIonMomentumXY
      {"i1HitPx_Py",
       "1st Hit Ion Momentum XY Image;Momentum X [au];Momentum Y [au]",
       H2_ION_MOMENTUM,
       H2_ION_MOMENTUM};
  TH2F root2DHistogramOf2ndHitIonMomentumXY
      {"i2HitPx_Py",
       "2nd Hit Ion Momentum XY Image;Momentum X [au];Momentum Y [au]",
       H2_ION_MOMENTUM,
       H2_ION_MOMENTUM};
  TH2F root2DHistogramOf3rdHitIonMomentumXY
      {"i3HitPx_Py",
       "3rd Hit Ion Momentum XY Image;Momentum X [au];Momentum Y [au]",
       H2_ION_MOMENTUM,
       H2_ION_MOMENTUM};
  TH2F root2DHistogramOf4thHitIonMomentumXY
      {"i4HitPx_Py",
       "4th Hit Ion Momentum XY Image;Momentum X [au];Momentum Y [au]",
       H2_ION_MOMENTUM,
       H2_ION_MOMENTUM};
  TH1F root1DHistogramOf1stHitIonMomentumZ
      {"i1HitPz",
       "1st Hit Ion Momentum Z;Momentum Z [au];Count [1]",
       H1_ION_MOMENTUM};
  TH1F root1DHistogramOf2ndHitIonMomentumZ
      {"i2HitPz",
       "2nd Hit Ion Momentum Z;Momentum Z [au];Count [1]",
       H1_ION_MOMENTUM};
  TH1F root1DHistogramOf3rdHitIonMomentumZ
      {"i3HitPz",
       "3rd Hit Ion Momentum Z;Momentum Z [au];Count [1]",
       H1_ION_MOMENTUM};
  TH1F root1DHistogramOf4thHitIonMomentumZ
      {"i4HitPz",
       "4rd Hit Ion Momentum Z;Momentum Z [au];Count [1]",
       H1_ION_MOMENTUM};
  // energy
  TH1F root1DHistogramOf1stHitIonEnergy
      {"i1HitE",
       "1st Hit Ion Energy;Energy [eV];Count [1]",
       H1_ION_ENERGY};
  TH1F root1DHistogramOf2ndHitIonEnergy
      {"i2HitE",
       "2nd Hit Ion Energy;Energy [eV];Count [1]",
       H1_ION_ENERGY};
  TH1F root1DHistogramOf3rdHitIonEnergy
      {"i3HitE",
       "3rd Hit Ion Energy;Energy [eV];Count [1]",
       H1_ION_ENERGY};
  TH1F root1DHistogramOf4thHitIonEnergy
      {"i4HitE",
       "4th Hit Ion Energy;Energy [eV];Count [1]",
       H1_ION_ENERGY};
  TH1F root1DHistogramOfIonsTotalEnergy
      {"iTotalE",
       "Ions Total Energy;Energy [eV];Count [1]",
       H1_Ion_TOTALENERGY};

  // electron
  // location
  TH2F root2DHistogramOf1stHitElectronLocationX_LocationY
      {"e1HitX_Y",
       "1st Hit Electron Location Image;Location X [mm];Location Y [mm]",
       H2_ELECTRON_LOCATION,
       H2_ELECTRON_LOCATION};
  TH2F root2DHistogramOf2ndHitElectronLocationX_LocationY
      {"e2HitX_Y",
       "2nd Hit Electron Location Image;Location X [mm];Location Y [mm]",
       H2_ELECTRON_LOCATION,
       H2_ELECTRON_LOCATION};
  TH2F root2DHistogramOf3rdHitElectronLocationX_LocationY
      {"e3HitX_Y",
       "3rd Hit Electron Location Image;Location X [mm];Location Y [mm]",
       H2_ELECTRON_LOCATION,
       H2_ELECTRON_LOCATION};
  TH2F root2DHistogramOf4thHitElectronLocationX_LocationY
      {"e4HitX_Y",
       "4th Hit Electron Location Image;Location X [mm];Location Y [mm]",
       H2_ELECTRON_LOCATION,
       H2_ELECTRON_LOCATION};
  TH2F root2DHistogramOfElectronsCOMLocationX_LocationY
      {"eCOMX_Y",
       "Electrons COM Location Image;Location X [mm];Location Y [mm]",
       H2_ELECTRON_LOCATION,
       H2_ELECTRON_LOCATION};
  TH2F root2DHistogramOfElectronsCOMLocationalDirectionXY_Radius
      {"eCOMThetaXY_R",
       "Electrons COM Locational Direction and Radius;Locational Direction XY [degree];Radius [mm]",
       H2_DEGREE,
       H2_ELECTRON_RADIUS};
  // TOF
  TH1F root1DHistogramOf1stHitElectronTOF
      {"e1HitTOF",
       "1st Hit Electron TOF;TOF [ns];Count [1]",
       H1_ELECTRON_TOF};
  TH1F root1DHistogramOf2ndHitElectronTOF
      {"e2HitTOF",
       "2nd Hit Electron TOF;TOF [ns];Count [1]",
       H1_ELECTRON_TOF};
  TH1F root1DHistogramOf3rdHitElectronTOF
      {"e3HitTOF",
       "3rd Hit Electron TOF;TOF [ns];Count [1]",
       H1_ELECTRON_TOF};
  TH1F root1DHistogramOf4thHitElectronTOF
      {"e4HitTOF",
       "4th Hit Electron TOF;TOF [ns];Count [1]",
       H1_ELECTRON_TOF};
  // momentum
  TH2F root2DHistogramOf1stHitElectronMomentumXY
      {"e1HitPx_Py",
       "1st Hit Electron Momentum XY Image;Momentum X [au];Momentum Y [au]",
       H2_ELECTRON_MOMENTUM,
       H2_ELECTRON_MOMENTUM};
  TH2F root2DHistogramOf2ndHitElectronMomentumXY
      {"e2HitPx_Py",
       "2nd Hit Electron Momentum XY Image;Momentum X [au];Momentum Y [au]",
       H2_ELECTRON_MOMENTUM,
       H2_ELECTRON_MOMENTUM};
  TH2F root2DHistogramOf3rdHitElectronMomentumXY
      {"e3HitPx_Py",
       "3rd Hit Electron Momentum XY Image;Momentum X [au];Momentum Y [au]",
       H2_ELECTRON_MOMENTUM,
       H2_ELECTRON_MOMENTUM};
  TH2F root2DHistogramOf4thHitElectronMomentumXY
      {"e4HitPx_Py",
       "4rd Hit Electron Momentum XY Image;Momentum X [au];Momentum Y [au]",
       H2_ELECTRON_MOMENTUM,
       H2_ELECTRON_MOMENTUM};
  TH1F root1DHistogramOf1stHitElectronMomentumZ
      {"e1HiPz",
       "1st Hit Electron Momentum Z;Momentum Z [au];Count [1]",
       H1_ELECTRON_MOMENTUM};
  TH1F root1DHistogramOf2ndHitElectronMomentumZ
      {"e2HiPz",
       "2nd Hit Electron Momentum Z;Momentum Z [au];Count [1]",
       H1_ELECTRON_MOMENTUM};
  TH1F root1DHistogramOf3rdHitElectronMomentumZ
      {"e3HiPz",
       "3rd Hit Electron Momentum Z;Momentum Z [au];Count [1]",
       H1_ELECTRON_MOMENTUM};
  TH1F root1DHistogramOf4thHitElectronMomentumZ
      {"e4HiPz",
       "4th Hit Electron Momentum Z;Momentum Z [au];Count [1]",
       H1_ELECTRON_MOMENTUM};
  TH2F root2DHistogramOfElectronsTotalMotionalDirectionXY_Momentum
      {"eTotalThetaXY_P",
       "Electrons Total Motional Direction and Momentum;Motional Direction XY [degree];Momentum [au]",
       H2_DEGREE,
       H2_ELECTRON_NORMOFMOMENTUM};
  TH2F root2DHistogramOfElectronsTotalMotionalDirectionZX_Momentum
      {"eTotalThetaZX_P",
       "Electrons Total Motional Direction and Momentum;Motional Direction ZX [degree];Momentum [au]",
       H2_DEGREE,
       H2_ELECTRON_NORMOFMOMENTUM};
  TH2F root2DHistogramOfElectronsTotalMotionalDirectionZY_Momentum
      {"eTotalThetaZY_P",
       "Electrons Total Motional Direction and Momentum;Motional Direction ZY [degree];Momentum [au]",
       H2_DEGREE,
       H2_ELECTRON_NORMOFMOMENTUM};
  // energy
  TH1F root1DHistogramOf1stHitElectronEnergy
      {"e1HitE",
       "1st Hit Electron Energy;Energy [eV];Count [1]",
       H1_ELECTRON_ENERGY};
  TH1F root1DHistogramOf2ndHitElectronEnergy
      {"e2HitE",
       "2nd Hit Electron Energy;Energy [eV];Count [1]",
       H1_ELECTRON_ENERGY};
  TH1F root1DHistogramOf3rdHitElectronEnergy
      {"e3HitE",
       "3rd Hit Electron Energy;Energy [eV];Count [1]",
       H1_ELECTRON_ENERGY};
  TH1F root1DHistogramOf4thHitElectronEnergy
      {"e4HitE",
       "4th Hit Electron Energy;Energy [eV];Count [1]",
       H1_ELECTRON_ENERGY};
  TH2F root2DHistogramOf1stHitElectronEnergy_2ndHitElectronEnergy
      {"e1Hit_2HitE",
       "1st Hit Electron and 2nd Hit Electron Energy Coincidence;1st Hit Electron Energy [eV];2nd Hit Electron Energy [eV]",
       H2_ELECTRON_ENERGY,
       H2_ELECTRON_ENERGY};
  TH2F root2DHistogramOf2ndHitElectronEnergy_3rdHitElectronEnergy
      {"e2Hit_3HitE",
       "2nd Hit Electron and 3rd Hit Electron Energy Coincidence;2nd Hit Electron Energy [eV];3rd Hit Electron Energy [eV]",
       H2_ELECTRON_ENERGY,
       H2_ELECTRON_ENERGY};
  TH2F root2DHistogramOf3rdHitElectronEnergy_4thHitElectronEnergy
      {"e3Hit_4HitE",
       "3rd Hit Electron and 4th Hit Electron Energy Coincidence;3rd Hit Electron Energy [eV];4th Hit Electron Energy [eV]",
       H2_ELECTRON_ENERGY,
       H2_ELECTRON_ENERGY};
  TH1F root1DHistogramOfElectronsTotalEnergy
      {"eTotalE",
       "Electrons Total Energy;Energy [eV];Count [1]",
       H1_ELECTRON_ENERGY};

  // ion and electron
  // todo: ion KER vs electron energy
  TH2F root2DHistogramOfSumOfIonTOF_1stHitElectronEnergy
      {"iSumTOF_e1HitE",
       "Sum of Ion TOFs vs 1st Hit Electron Energy;Sum of TOFs [ns];Energy [eV]",
       H2_ION_SUMOFTOF,
       H2_ELECTRON_ENERGY};
  TH2F root2DHistogramOfTotalEnergy_1stHitElectronEnergy
      {"iTotalE_e1HitE",
       "Total Ion Energy vs 1st Hit Electron Energy;Total Ion Energy [eV];Electron Energy [eV]",
       H2_Ion_TOTALENERGY,
       H2_ELECTRON_ENERGY};
};
}

#endif //BL17ANALYSIS_BL17ANALYSIS_H
