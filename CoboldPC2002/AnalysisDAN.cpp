///////////////////////////////////////////////////////////////////////////
//  User defined analysis part called from cobold main program
///////////////////////////////////////////////////////////////////////////

#define FOR_COBOLDPC2002

#include "../include/Analysis/JSONReader.cpp"
#include "../include/Analysis/LMFReader.cpp"
#include "../include/Analysis/Unit.cpp"
#include "../include/Analysis/Flag.cpp"
#include "../include/Analysis/ObjectFlag.cpp"
#include "../include/Analysis/EventDataFlag.cpp"
#include "../include/Analysis/Object.cpp"
#include "../include/Analysis/Objects.cpp"
#include "../include/Analysis/Ion.cpp"
#include "../include/Analysis/Ions.cpp"
#include "../include/Analysis/Electron.cpp"
#include "../include/Analysis/Electrons.cpp"
#include "../include/Analysis/EquipmentParameters.cpp"
#include "../include/Analysis/ObjectParameters.cpp"
#include "../include/Analysis/IonParameters.cpp"
#include "../include/Analysis/ElectronParameters.cpp"
#include "../include/Analysis/AnalysisTools.cpp"

Analysis::Unit *pUnit;
Analysis::JSONReader *pJSONReader;
Analysis::LMFReader *pLMFReader;
Analysis::AnalysisTools *pAnalysisTools;
Analysis::Ions *pIons;
Analysis::Electrons *pElectrons;
const int numberOfHitsUsed = NUMBER_OF_HITS_USED;
std::fstream logFile;

const std::string getObjectName(int i) {
  i += 1;
  assert(i > 0);
  const int firstDigit = i % 10;
  const int secondDigit = (i / 10) % 10;
  std::string str = std::to_string(i);
  if (secondDigit == 1) { return str + "th hit"; }
  else {
    if (firstDigit == 1) { return str + "st hit"; }
    else if (firstDigit == 2) { return str + "nd hit"; }
    else if (firstDigit == 3) { return str + "rd hit"; }
    else { return str + "th hit"; }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Parameter description used in this insert dependent part!
// ---------------------------------------------------------
//
// Hardware
// --------
//
// Software
// --------
//
//

/////////////////////////////////////////////////////////////////////////////
// Analysis information string

CDAN_API LPCTSTR AnalysisGetInformationString() {
  return LPCTSTR("Analysis");
}

/////////////////////////////////////////////////////////////////////////////
// Analysis functions

CDAN_API BOOL AnalysisInitialize(CDoubleArray *pEventData,
                                 CDoubleArray *pParameters,
                                 CDoubleArray *pWeighParameter) {
  logFile.open("log.txt", std::fstream::out);
  logFile << "The path is setted here. " << std::endl;

  pUnit = new Analysis::Unit;
  pJSONReader = new Analysis::JSONReader("parameters.json");
  pAnalysisTools = new Analysis::AnalysisTools(*pUnit, *pJSONReader);
  pIons = new Analysis::Ions(*pUnit, *pJSONReader, numberOfHitsUsed);
  pElectrons = new Analysis::Electrons(*pUnit, *pJSONReader, numberOfHitsUsed);
  logFile << "Initialization is successed. " << std::endl;

  {
    {
      const int &n = pIons->getNumberOfHits();
      for (int i = 0; i < n; i++) {
        const std::string name = getObjectName(0);
        const double t = pAnalysisTools->calculateTOF(*pUnit,
                                                      pIons->getIon(i),
                                                      0e0);
        logFile << "TOF of " + name + "ion: " << t << std::endl;
      }
    }
    {
      const double t1
          = pAnalysisTools->calculateTOF(*pUnit,
                                         pElectrons->getElectron(0),
                                         0e0);
      const double t2
          = pAnalysisTools->calculatePeriodOfCycle(*pUnit,
                                                   pElectrons->getElectron(0));
      logFile << "TOF of electron: " << t1 << std::endl;
      logFile << "Period of cycle of electron: " << t2 << std::endl;
    }
  }
  return TRUE;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray *pEventData,
                                   CDoubleArray *pParameters,
                                   CDoubleArray *pWeighParameter) {
  pLMFReader = NULL;
  pIons->resetEventData();
  pElectrons->resetEventData();

  pLMFReader = new Analysis::LMFReader(*pEventData);
  pAnalysisTools->loadEventDataInputer(*pIons, *pUnit, *pLMFReader);
  pAnalysisTools->loadEventDataInputer(*pElectrons, *pUnit, *pLMFReader);
  pAnalysisTools->loadMomentumCalculator(*pIons);
  pAnalysisTools->loadMomentumCalculator(*pElectrons);

// write ion data
  {
    const int &n = pIons->getNumberOfHits();
    const int &m = pIons->getNumberOfHitsUsed();
    for (int i = 0; i < m; i++) {
      pEventData->SetAt(31 + 28 + i, pIons->getIon(i).getLocationX(*pUnit));
      pEventData->SetAt(31 + 32 + i, pIons->getIon(i).getLocationY(*pUnit));
      pEventData->SetAt(67 + i, pIons->getIon(i).getTOF(*pUnit));
      if (i < n) {
        pEventData->SetAt(76 + i, pIons->getIon(i).getMomentumX(*pUnit));
        pEventData->SetAt(80 + i, pIons->getIon(i).getMomentumY(*pUnit));
        pEventData->SetAt(84 + i, pIons->getIon(i).getMomentumZ(*pUnit));
        pEventData->SetAt(67 + 4 + i, pIons->getIon(i).getEnergy(*pUnit));
      }
    }
  }
  pEventData->SetAt(128, pIons->getLocationXOfCOM(*pUnit));
  pEventData->SetAt(129, pIons->getLocationXOfCOM(*pUnit));
//  pEventData->SetAt(130, 0e0); //dCOM_z/unit_milli);
  pEventData->SetAt(88, pIons->getTotalMomentumX(*pUnit));
  pEventData->SetAt(89, pIons->getTotalMomentumY(*pUnit));
  pEventData->SetAt(90, pIons->getTotalMomentumZ(*pUnit));
  pEventData->SetAt(75, pIons->getTotalEnergy(*pUnit));
  pEventData->SetAt(95, pIons->getDirectionXOfCOM());
  pEventData->SetAt(96, pIons->getDirectionYOfCOM());
  pEventData->SetAt(97, pIons->getDirectionZOfCOM());
//  pEventData->SetAt(91, 0e0); //dIon_theta*180.0/pi);
//  pEventData->SetAt(92, 0e0); //dIon_psai_xy*180.0/pi);
//  pEventData->SetAt(93, 0e0); //dIon_psai_zy*180.0/pi);
//  pEventData->SetAt(94, 0e0); //dIon_psai_zx*180.0/pi);
//  pEventData->SetAt(205, 0e0); //(dtIon[0]+dtIon[1])/unit_nano);
//  pEventData->SetAt(206, 0e0); //(dtIon[0]+dtIon[2])/unit_nano);
//  pEventData->SetAt(207, 0e0); //(dtIon[0]+dtIon[3])/unit_nano);
//  pEventData->SetAt(208, 0e0); //(dtIon[1]+dtIon[2])/unit_nano);
//  pEventData->SetAt(209, 0e0); //(dtIon[1]+dtIon[3])/unit_nano);
//  pEventData->SetAt(210, 0e0); //(dtIon[2]+dtIon[3])/unit_nano);

// write electron data
  {
    const int &n = pElectrons->getNumberOfHits();
    const int &m = pElectrons->getNumberOfHitsUsed();
    for (int i = 0; i < m; i++) {
      pEventData->SetAt(31 + i,
                        pElectrons->getElectron(i).getLocationX(*pUnit));
      pEventData->SetAt(31 + 4 + i,
                        pElectrons->getElectron(i).getLocationY(*pUnit));
      pEventData->SetAt(98 + i,
                        pElectrons->getElectron(i).getTOF(*pUnit));
      if (i < n) {
        pEventData->SetAt(107 + i,
                          pElectrons->getElectron(i).getMomentumX(*pUnit));
        pEventData->SetAt(111 + i,
                          pElectrons->getElectron(i).getMomentumY(*pUnit));
        pEventData->SetAt(115 + i,
                          pElectrons->getElectron(i).getMomentumZ(*pUnit));
        pEventData->SetAt(103 + i,
                          pElectrons->getElectron(i).getEnergy(*pUnit));
      }
    }
  }
  pEventData->SetAt(131, pElectrons->getLocationXOfCOM(*pUnit));
  pEventData->SetAt(132, pElectrons->getLocationYOfCOM(*pUnit));
  pEventData->SetAt(119, pElectrons->getElectron(0).getDirectionX());
  pEventData->SetAt(120, pElectrons->getElectron(0).getDirectionY());
  pEventData->SetAt(121, pElectrons->getElectron(0).getDirectionZ());
  pEventData->SetAt(122, pElectrons->getTotalAbsoluteMomentum(*pUnit));
//  pEventData->SetAt(123, 0e0); //dElectron_psai_xy*180.0/pi);
//  pEventData->SetAt(124, 0e0); //dElectron_psai_zy*180.0/pi);
//  pEventData->SetAt(125, 0e0); //dElectron_psai_zx*180.0/pi);
//  pEventData->SetAt(102, 0e0); //dElectronEnergyXY/dElectron);
//  {
//    double higher, lower;
//    if (dElectronEnergy[0] >= dElectronEnergy[1]) {
//      higher = dElectronEnergy[0];
//      lower = dElectronEnergy[1];
//    } else {
//      higher = dElectronEnergy[1];
//      lower = dElectronEnergy[0];
//    }
//    pEventData->SetAt(203, 0e0); //dElectronEnergyHigher/dElectron);
//    pEventData->SetAt(204, 0e0); //dElectronEnergyLower/dElectron);
//  }

//  write ion & electron data
  {
    const int &n = pElectrons->getNumberOfHits();
    for (int i = 0; i < n; i++) {
      pEventData->SetAt(199 + i,
                        pIons->getTotalEnergy(*pUnit)
                            + pElectrons->getElectron(i).getEnergy(*pUnit));
    }
  }

// write flag data
  {
    int IonMasterFlag, ElectronMasterFlag;
    if (pIons->getIon(0).getObjectFlag().isMasterOnFlag()
        && pIons->getIon(0).getObjectFlag().isValidOnFlag()) {
      IonMasterFlag = 1;
    } else {
      IonMasterFlag = -1;
    }
    if (pElectrons->getElectron(0).getObjectFlag().isMasterOnFlag()) {
      ElectronMasterFlag = 1;
    } else {
      ElectronMasterFlag = -1;
    }
    pEventData->SetAt(126, IonMasterFlag);
    pEventData->SetAt(127, ElectronMasterFlag);
  }

//  what is it?
//  pEventData->SetAt(18, 0e0); //nHexX1);  // 18+0
//  pEventData->SetAt(19, 0e0); //nHexX2);  // 18+1
//  pEventData->SetAt(20, 0e0); //nHexY1);  // 18+2
//  pEventData->SetAt(21, 0e0); //nHexY2);  // 18+3
//  pEventData->SetAt(22, 0e0); //nHexZ1);  // 18+4
//  pEventData->SetAt(23, 0e0); //nHexZ2);  // 18+5
//  pEventData->SetAt(24, 0e0); //nMCPele); // 18+6
//  pEventData->SetAt(25, 0e0); //nMCPion); // 18+7
//  pEventData->SetAt(26, 0e0); //nSqX1);   // 18+8
//  pEventData->SetAt(27, 0e0); //nSqX2);   // 18+9
//  pEventData->SetAt(28, 0e0); //nSqY1);   // 18+10
//  pEventData->SetAt(29, 0e0); //nSqY2);   // 18+11
//  pEventData->SetAt(30, 0e0); //nBunch);  // 18+12
//  pEventData->SetAt(133, 0e0); //dphai*180.0/pi);
//  pEventData->SetAt(134, 0e0); //dEX/unit_milli);
//  pEventData->SetAt(135, 0e0); //dEY/unit_milli);
//  pEventData->SetAt(136, 0e0); //dElectron_theta0[0]*180.0/pi);
//  pEventData->SetAt(137, 0e0); //dElectron_posi_xy*180.0/pi);
//  pEventData->SetAt(138, 0e0); //dIon_Electron_Angle0*180.0/pi);
//  pEventData->SetAt(139, 0e0); //isellect);
//  pEventData->SetAt(140, 0e0); //dtN/unit_nano);
//  pEventData->SetAt(141, 0e0); //dtO/unit_nano);
//  {
//    double dAngle_Width = 10e0;
//    for (int ii = 0; ii < int(180 / dAngle_Width + 0.1); ii++) {
//      pEventData->SetAt(142 + ii * 3, 0e0); //dIon_Angle[ii]*180.0/pi);
//      pEventData->SetAt(142 + ii * 3 + 1, 0e0); //dElectron_Angle[ii]*180.0/pi);
//      pEventData->SetAt(142 + ii * 3 + 2,
//                        0e0); //dIon_Electron_Angle[ii]*180.0/pi);
//    }
//  }
//  pEventData->SetAt(196, 0e0); //d_I_Angle*180.0/pi);
//  pEventData->SetAt(197, 0e0); //d_e_I_Angle*180.0/pi);
//  pEventData->SetAt(198, 0e0); //distancexy/unit_milli);
  return;
}

CDAN_API void AnalysisFinalize(CDoubleArray *pEventData,
                               CDoubleArray *pParameters,
                               CDoubleArray *pWeighParameter) {
  logFile.close();
  return;
}

#pragma clang diagnostic pop