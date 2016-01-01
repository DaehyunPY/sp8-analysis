//
// Created by Daehyun You on 12/30/15.
//

#ifndef BL17ANALYSIS_RUN_H
#define BL17ANALYSIS_RUN_H


#include <TApplication.h>
#include "TCanvas.h"
#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "Analysis/Analysis.h"
namespace BL17Analysis {
class Run {
  const int numberOfTDCUsed = 3;
  const int numberOfChannelsUsed = 2;
  const int numberOfHitsUsed = 4;
  Analysis::LogWriter *pLogWriter;
  Analysis::Unit *pUnit;
  Analysis::AnalysisTools *pTools;
  Analysis::Ions *pIons;
  Analysis::Electrons *pElectrons;
  bool optionOfSendingOutOfFrame;
  bool optionOfExportingElectronMomentum;
  std::fstream exportedFile;

//  int *argc;
//  char **argv;
//  TApplication rootApp{"Root Application", argc, argv};
  TFile rootFile;
  TH1F root1DHistogramOfIonFlag
      {"iFlag",
          "Ion Flag;Flag [1];Count [1]",
          198, -99, 99};
  TH1F root1DHistogramOfElectronFlag
      {"eFlag",
          "Electron Flag;Flag [1];Count [1]",
          198, -99, 99};
  TH2F root2DHistogramOf1stHitIonLocationX_LocationY
      {"i1HitX_Y",
          "1st Hit Ion Location Image;Location X [mm];Location Y [mm]",
          100, -50, 50, 100, -50, 50};
  TH2F root2DHistogramOf2ndHitIonLocationX_LocationY
      {"i2HitX_Y",
          "2nd Hit Ion Location Image;Location X [mm];Location Y [mm]",
          100, -50, 50, 100, -50, 50};
  TH2F root2DHistogramOf3rdHitIonLocationX_LocationY
      {"i3HitX_Y",
          "3rd Hit Ion Location Image;Location X [mm];Location Y [mm]",
          100, -50, 50, 100, -50, 50};
  TH2F root2DHistogramOf4thHitIonLocationX_LocationY
      {"i4HitX_Y",
          "4th Hit Ion Location Image;Location X [mm];Location Y [mm]",
          100, -50, 50, 100, -50, 50};
  TH2F root2DHistogramOfIonsCOMLocationX_LocationY
      {"iCOMX_Y",
          "Ions COM Location Image;Location X [mm];Location Y [mm]",
          100, -50, 50, 100, -50, 50};
  TH1F root1DHistogramOf1stHitIonTOF
      {"i1HitTOF",
          "1st Hit Ion TOF;TOF [ns];Count [1]",
          1000, -2000, 13000};
  TH1F root1DHistogramOf2ndHitIonTOF
      {"i2HitTOF",
          "2nd Hit Ion TOF;TOF [ns];Count [1]",
          1000, -2000, 13000};
  TH1F root1DHistogramOf3rdHitIonTOF
      {"i3HitTOF",
          "3rd Hit Ion TOF;TOF [ns];Count [1]",
          1000, -2000, 13000};
  TH1F root1DHistogramOf4thHitIonTOF
      {"i4HitTOF",
          "4th Hit Ion TOF;TOF [ns];Count [1]",
          1000, -2000, 13000};
  TH2F root2DHistogramOf1stHitIonTOF_2ndHitIonTOF
      {"i1Hit_2HitTOF",
        "1st Hit and 2nd Hit Ion PIPICO;1st Hit Ion TOF [ns];2nd Hit Ion TOF [ns]",
          100, 0, 10000, 100, 0, 10000};
  TH2F root2DHistogramOf2ndHitIonTOF_3rdHitIonTOF
      {"i2Hit_3HitTOF",
        "2nd Hit and 3rd Hit Ion PIPICO;2nd Hit Ion TOF [ns];3rd Hit Ion TOF [ns]",
          100, 0, 10000, 100, 0, 10000};
  TH2F root2DHistogramOf3rdHitIonTOF_4thHitIonTOF
      {"i3Hit_4HitTOF",
        "3rd Hit Ion 4th Hit Ion PIPICO;3rd Hit Ion TOF [ns];4th Hit Ion TOF [ns]",
          100, 0, 10000, 100, 0, 10000};
  TH2F root2DHistogramOf1stHitIonTOFPlus2ndHitIonTOF_3rdHitIonTOF
      {"i1HitPlus2Hit_3HitTOF",
        "1st Hit, 2nd Hit and 3rd Hit Ion PIPIPICO;1st Hit Ion TOF + 2nd Hit Ion TOF [ns];3rd Hit Ion TOF [ns]",
          200, 0, 20000, 100, 0, 10000};
  TH2F root2DHistogramOf2ndHitIonTOFPlus3rdHitIonTOF_4thHitIonTOF
      {"i2HitPlus3Hit_4HitTOF",
          "2nd Hit, 3rd Hit and 4th Hit Ion PIPIPICO;2nd Hit Ion TOF + 3rd Hit Ion TOF [ns];4th Hit Ion TOF [ns]",
          200, 0, 20000, 100, 0, 10000};
  TH2F root2DHistogramOf1stHitIonMomentumXY
      {"i1HitPX_PY",
          "1st Hit Ion Momentum XY Image;Momentum X [au];Momentum Y [au]",
          100, -500, 500, 100, -500, 500};
  TH2F root2DHistogramOf2ndHitIonMomentumXY
      {"i2HitPX_PY",
          "2nd Hit Ion Momentum XY Image;Momentum X [au];Momentum Y [au]",
          100, -500, 500, 100, -500, 500};
  TH2F root2DHistogramOf3rdHitIonMomentumXY
      {"i3HitPX_PY",
          "3rd Hit Ion Momentum XY Image;Momentum X [au];Momentum Y [au]",
          100, -500, 500, 100, -500, 500};
  TH2F root2DHistogramOf4thHitIonMomentumXY
      {"i4HitPX_PY",
          "4th Hit Ion Momentum XY Image;Momentum X [au];Momentum Y [au]",
          100, -500, 500, 100, -500, 500};
  TH1F root1DHistogramOf1stHitIonMomentumZ
      {"i1HitPZ",
          "1st Hit Ion Momentum Z;Momentum Z [au];Count [1]",
          100, -500, 500};
  TH1F root1DHistogramOf2ndHitIonMomentumZ
      {"i2HitPZ",
          "2nd Hit Ion Momentum Z;Momentum Z [au];Count [1]",
          100, -500, 500};
  TH1F root1DHistogramOf3rdHitIonMomentumZ
      {"i3HitPZ",
          "3rd Hit Ion Momentum Z;Momentum Z [au];Count [1]",
          100, -500, 500};
  TH1F root1DHistogramOf4thHitIonMomentumZ
      {"i4HitPZ",
          "4rd Hit Ion Momentum Z;Momentum Z [au];Count [1]",
          100, -500, 500};
  TH1F root1DHistogramOf1stHitIonEnergy
      {"i1HitE",
          "1st Hit Ion Energy;Energy [eV];Count [1]",
          100, 0, 50};
  TH1F root1DHistogramOf2ndHitIonEnergy
      {"i2HitE",
          "2nd Hit Ion Energy;Energy [eV];Count [1]",
          100, 0, 50};
  TH1F root1DHistogramOf3rdHitIonEnergy
      {"i3HitE",
          "3rd Hit Ion Energy;Energy [eV];Count [1]",
          100, 0, 50};
  TH1F root1DHistogramOf4thHitIonEnergy
      {"i4HitE",
          "4th Hit Ion Energy;Energy [eV];Count [1]",
          100, 0, 50};
  TH1F root1DHistogramOfIonsTotalEnergy
      {"iTotalE",
          "Ions Total Energy;Energy [eV];Count [1]",
          100, 0, 50};
  TH2F root2DHistogramOf1stHitElectronLocationX_LocationY
      {"e1HitX_Y",
          "1st Hit Electron Location Image;Location X [mm];Location Y [mm]",
          100, -75, 75, 100, -75, 75};
  TH2F root2DHistogramOf2ndHitElectronLocationX_LocationY
      {"e2HitX_Y",
          "2nd Hit Electron Location Image;Location X [mm];Location Y [mm]",
          100, -75, 75, 100, -75, 75};
  TH2F root2DHistogramOf3rdHitElectronLocationX_LocationY
      {"e3HitX_Y",
          "3rd Hit Electron Location Image;Location X [mm];Location Y [mm]",
          100, -75, 75, 100, -75, 75};
  TH2F root2DHistogramOf4thHitElectronLocationX_LocationY
      {"e4HitX_Y",
          "4th Hit Electron Location Image;Location X [mm];Location Y [mm]",
          100, -75, 75, 100, -75, 75};
  TH2F root2DHistogramOfElectronsCOMLocationX_LocationY
      {"eCOMX_Y", 
          "Electrons COM Location Image;Location X [mm];Location Y [mm]",
          100, -75, 75, 100, -75, 75};
  TH2F root2DHistogramOfElectronsCOMLocationalDirectionXY_Radius
      {"eCOMTheta_R",
          "Electrons COM Locational Direction and Radius;Locational Direction XY [degree];Radius [mm]",
          180, -180, 180, 100, 0, 75};
  TH1F root1DHistogramOf1stHitElectronTOF
      {"e1HitTOF", 
          "1st Hit Electron TOF;TOF [ns];Count [1]",
          1000, -20, 130};
  TH1F root1DHistogramOf2ndHitElectronTOF
      {"e2HitTOF", 
          "2nd Hit Electron TOF;TOF [ns];Count [1]",
          1000, -20, 130};
  TH1F root1DHistogramOf3rdHitElectronTOF
      {"e3HitTOF", 
          "3rd Hit Electron TOF;TOF [ns];Count [1]",
          1000, -20, 130};
  TH1F root1DHistogramOf4thHitElectronTOF
      {"e4HitTOF", 
          "4th Hit Electron TOF;TOF [ns];Count [1]",
          1000, -20, 130};
  TH2F root2DHistogramOf1stHitElectronMomentumXY
      {"e1HitPX_PY", 
          "1st Hit Electron Momentum XY Image;Momentum X [au];Momentum Y [au]",
          100, -3, 3, 100, -3, 3};
  TH2F root2DHistogramOf2ndHitElectronMomentumXY
      {"e2HitPX_PY", 
          "2nd Hit Electron Momentum XY Image;Momentum X [au];Momentum Y [au]",
          100, -3, 3, 100, -3, 3};
  TH2F root2DHistogramOf3rdHitElectronMomentumXY
      {"e3HitPX_PY", 
          "3rd Hit Electron Momentum XY Image;Momentum X [au];Momentum Y [au]",
          100, -3, 3, 100, -3, 3};
  TH2F root2DHistogramOf4thHitElectronMomentumXY
      {"e4HitPX_PY", 
          "4rd Hit Electron Momentum XY Image;Momentum X [au];Momentum Y [au]",
          100, -3, 3, 100, -3, 3};
  TH1F root1DHistogramOf1stHitElectronMomentumZ
      {"e1HitPZ", 
          "1st Hit Electron Momentum Z;Momentum Z [au];Count [1]",
          1000, -3, 3};
  TH1F root1DHistogramOf2ndHitElectronMomentumZ
      {"e2HitPZ", 
          "2nd Hit Electron Momentum Z;Momentum Z [au];Count [1]",
          1000, -3, 3};
  TH1F root1DHistogramOf3rdHitElectronMomentumZ
      {"e3HitPZ", 
          "3rd Hit Electron Momentum Z;Momentum Z [au];Count [1]",
          1000, -3, 3};
  TH1F root1DHistogramOf4thHitElectronMomentumZ
      {"e4HitPZ", 
          "4th Hit Electron Momentum Z;Momentum Z [au];Count [1]",
          1000, -3, 3};
  TH2F root2DHistogramOfElectronsTotalMotionalDirectionXY_Momentum
      {"eTotalTheta_P", 
          "Electrons Total Motional Direction and Momentum;Motional Direction XY [degree];Momentum [au]",
          180, -180, 180, 100, 0, 3};
  TH2F root2DHistogramOfElectronsTotalMotionalDirectionZX_Momentum
      {"eTotalTheta_P", 
          "Electrons Total Motional Direction and Momentum;Motional Direction ZX [degree];Momentum [au]",
          180, -180, 180, 100, 0, 3};
  TH2F root2DHistogramOfElectronsTotalMotionalDirectionZY_Momentum
      {"eTotalTheta_P", 
          "Electrons Total Motional Direction and Momentum;Motional Direction ZY [degree];Momentum [au]",
          180, -180, 180, 100, 0, 3};
  TH1F root1DHistogramOf1stHitElectronEnergy
      {"e1HitE", 
          "1st Hit Electron Energy;Energy [eV];Count [1]",
          1000, 0, 50};
  TH1F root1DHistogramOf2ndHitElectronEnergy
      {"e2HitE", 
          "2nd Hit Electron Energy;Energy [eV];Count [1]",
          1000, 0, 50};
  TH1F root1DHistogramOf3rdHitElectronEnergy
      {"e3HitE", 
          "3rd Hit Electron Energy;Energy [eV];Count [1]",
          1000, 0, 50};
  TH1F root1DHistogramOf4thHitElectronEnergy
      {"e4HitE", 
          "4th Hit Electron Energy;Energy [eV];Count [1]",
          1000, 0, 50};
  TH2F root2DHistogramOf1stHitElectronEnergy_2ndHitElectronEnergy
      {"e1Hit_2HitE", 
          "1st Hit Electron and 2nd Hit Electron Energy Coincidence;1st Hit Electron Energy [eV];2nd Hit Electron Energy [eV]",
          100, 0, 50, 100, 0, 50};
  TH2F root2DHistogramOf2ndHitElectronEnergy_3rdHitElectronEnergy
      {"e2Hit_3HitE", 
          "2nd Hit Electron and 3rd Hit Electron Energy Coincidence;2nd Hit Electron Energy [eV];3rd Hit Electron Energy [eV]",
          100, 0, 50, 100, 0, 50};
  TH2F root2DHistogramOf3rdHitElectronEnergy_4thHitElectronEnergy
      {"e3Hit_4HitE", 
          "3rd Hit Electron and 4th Hit Electron Energy Coincidence;3rd Hit Electron Energy [eV];4th Hit Electron Energy [eV]",
          100, 0, 50, 100, 0, 50};
  TH1F root1DHistogramOfElectronsTotalEnergy
      {"eTotalE", 
          "Electrons Total Energy;Energy [eV];Count [1]", 
          1000, 0, 50};

 public:
  Run();
  ~Run();
  void ProcessEvent(Analysis::EventDataReader &reader,
                    int &ionFlag,
                    int &ElectronFlag);
  const Analysis::Unit &getUnit() const;
  const Analysis::Ions &getIons() const;
  const Analysis::Electrons &getElectrons() const;
  const int &getNumberOfTDCUsed() const; 
  const int &getNumberOfChannelsUsed() const; 
  const int &getNumberOfHitsUsed() const; 
};
}

#endif //BL17ANALYSIS_BL17ANALYSIS_H
