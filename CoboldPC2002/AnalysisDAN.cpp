///////////////////////////////////////////////////////////////////////////
//  User defined analysis part called from cobold main program
///////////////////////////////////////////////////////////////////////////

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

std::fstream logFile;

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
  pLMFReader = NULL;
  pAnalysisTools = new Analysis::AnalysisTools(*pUnit, *pJSONReader);
  pIons = new Analysis::Ions(*pUnit, *pJSONReader);
  pElectrons = new Analysis::Electrons(*pUnit, *pJSONReader);
  logFile << "Initialization is successed. " << std::endl;

  const double TOFOf1stHitIon =
      pAnalysisTools->calculateTOF(*pUnit,
                                   pIons->getIon(0),
                                   0e0);
  const double TOFOf1stHitElectron =
      pAnalysisTools->calculateTOF(*pUnit,
                                   pElectrons->getElectron(0),
                                   0e0);
  const double PeriodOfCycleOfElectron =
      pAnalysisTools->calculatePeriodOfCycle(*pUnit,
                                             pElectrons->getElectron(0));
  logFile << "TOF of 1st Hit ion: " << TOFOf1stHitIon << std::endl;
  logFile << "TOF of 1st Hit electron: " << TOFOf1stHitElectron << std::endl;
  logFile << "Period of cycle of electron: "
      << PeriodOfCycleOfElectron
      << std::endl;
  return TRUE;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray *pEventData,
                                   CDoubleArray *pParameters,
                                   CDoubleArray *pWeighParameter) {
  pLMFReader = NULL;
  pLMFReader = new Analysis::LMFReader(*pParameters);
  pIons->resetEventData();
  pElectrons->resetEventData();
  pAnalysisTools->loadEventDataInputer(*pIons, *pUnit, *pLMFReader);
  pAnalysisTools->loadEventDataInputer(*pElectrons, *pUnit, *pLMFReader);
  pAnalysisTools->loadMomentumCalculator(*pIons);
  pAnalysisTools->loadMomentumCalculator(*pElectrons);
  logFile << "Calculation for one event is done. " << std::endl;

  const int nHit = 4;

//  {
//    double nHexX1, nHexX2, nHexY1, nHexY2, nHexZ1, nHexZ2,
//        nMCPele, nMCPion, nSqX1, nSqX2, nSqY1, nSqY2, nBunch;
//    pEventData->SetAt(18, nHexX1);  // 18+0
//    pEventData->SetAt(19, nHexX2);  // 18+1
//    pEventData->SetAt(20, nHexY1);  // 18+2
//    pEventData->SetAt(21, nHexY2);  // 18+3
//    pEventData->SetAt(22, nHexZ1);  // 18+4
//    pEventData->SetAt(23, nHexZ2);  // 18+5
//    pEventData->SetAt(24, nMCPele); // 18+6
//    pEventData->SetAt(25, nMCPion); // 18+7
//    pEventData->SetAt(26, nSqX1);   // 18+8
//    pEventData->SetAt(27, nSqX2);   // 18+9
//    pEventData->SetAt(28, nSqY1);   // 18+10
//    pEventData->SetAt(29, nSqY2);   // 18+11
//    pEventData->SetAt(30, nBunch);  // 18+12
//  }
//  logFile << "18-30 datas are writen. " << std::endl; 

  {
	  int iHit = 0; 
	  logFile << pElectrons->getElectron(iHit).getLocationX(*pUnit) << std::endl;
	  logFile << pElectrons->getElectron(iHit).getLocationY(*pUnit) << std::endl; 
	  logFile << pIons->getIon(iHit).getLocationX(*pUnit) << std::endl; 
	  logFile << pIons->getIon(iHit).getLocationY(*pUnit) << std::endl; 
	  logFile << pIons->getIon(iHit).getTOF(*pUnit) << std::endl; 
	  logFile << pIons->getIon(iHit).getEnergy(*pUnit) << std::endl; //nan 
	  logFile << pIons->getIon(iHit).getMomentumX(*pUnit) << std::endl; 
	  logFile << pIons->getIon(iHit).getMomentumY(*pUnit) << std::endl; 
	  logFile << pIons->getIon(iHit).getMomentumZ(*pUnit) << std::endl; //nan
	  logFile << pIons->getTotalMomentumX(*pUnit) << std::endl; 
	  logFile << pIons->getTotalMomentumY(*pUnit) << std::endl; 
	  logFile << pIons->getTotalMomentumZ(*pUnit) << std::endl; //nan
  }
  for (int iHit = 0; iHit < nHit; iHit++) {
    pEventData->SetAt(31 + iHit,
                      pElectrons->getElectron(iHit).getLocationX(*pUnit));
    pEventData->SetAt(31 + 4 + iHit,
                      pElectrons->getElectron(iHit).getLocationY(*pUnit));
    pEventData->SetAt(31 + 28 + iHit,
                      pIons->getIon(iHit).getLocationX(*pUnit));
    pEventData->SetAt(31 + 32 + iHit,
                      pIons->getIon(iHit).getLocationY(*pUnit));
    pEventData->SetAt(67 + iHit,
                      pIons->getIon(iHit).getTOF(*pUnit));
    pEventData->SetAt(67 + 4 + iHit,
                      pIons->getIon(iHit).getEnergy(*pUnit));
  }
  pEventData->SetAt(75, pIons->getTotalEnergy(*pUnit));
  for (int iHit = 0; iHit < nHit; iHit++) {
    pEventData->SetAt(76 + iHit, pIons->getIon(iHit).getMomentumX(*pUnit));
    pEventData->SetAt(80 + iHit, pIons->getIon(iHit).getMomentumY(*pUnit));
    pEventData->SetAt(84 + iHit, pIons->getIon(iHit).getMomentumZ(*pUnit));
  }
  pEventData->SetAt(88, pIons->getTotalMomentumX(*pUnit));
  pEventData->SetAt(89, pIons->getTotalMomentumY(*pUnit));
  pEventData->SetAt(90, pIons->getTotalMomentumZ(*pUnit));
  logFile << "31-90 datas are writen. " << std::endl; 

  pEventData->SetAt(91, 0e0); //dIon_theta*180.0/pi);
  pEventData->SetAt(92, 0e0); //dIon_psai_xy*180.0/pi);
  pEventData->SetAt(93, 0e0); //dIon_psai_zy*180.0/pi);
  pEventData->SetAt(94, 0e0); //dIon_psai_zx*180.0/pi);
  pEventData->SetAt(95, pIons->getDirectionXOfCOM());
  pEventData->SetAt(96, pIons->getDirectionYOfCOM());
  pEventData->SetAt(97, pIons->getDirectionZOfCOM());
  pEventData->SetAt(102, 0e0); //dElectronEnergyXY/dElectron);
  for (int iHit = 0; iHit < nHit; iHit++) {
    pEventData->SetAt(98 + iHit,
                      pElectrons->getElectron(iHit).getTOF(*pUnit));
    pEventData->SetAt(103 + iHit,
                      pElectrons->getElectron(iHit).getEnergy(*pUnit));
    pEventData->SetAt(107 + iHit,
                      pElectrons->getElectron(iHit).getMomentumX(*pUnit));
    pEventData->SetAt(111 + iHit,
                      pElectrons->getElectron(iHit).getMomentumY(*pUnit));
    pEventData->SetAt(115 + iHit,
                      pElectrons->getElectron(iHit).getMomentumZ(*pUnit));
    pEventData->SetAt(199 + iHit,
                      pElectrons->getTotalEnergy(*pUnit)
                          + pIons->getTotalEnergy(*pUnit));
  }
  logFile << "91-202 datas are writen. " << std::endl;

//   if(dElectronEnergy[0] >= dElectronEnergy[1]){
//       dElectronEnergyHigher = dElectronEnergy[0];
//       dElectronEnergyLower = dElectronEnergy[1];
//   } else{
//       dElectronEnergyHigher = dElectronEnergy[1];
//       dElectronEnergyLower = dElectronEnergy[0];
//   }
  pEventData->SetAt(203, 0e0); //dElectronEnergyHigher/dElectron);
  pEventData->SetAt(204, 0e0); //dElectronEnergyLower/dElectron);
  pEventData->SetAt(119, pElectrons->getElectron(0).getDirectionX());
  pEventData->SetAt(120, pElectrons->getElectron(0).getDirectionY());
  pEventData->SetAt(121, pElectrons->getElectron(0).getDirectionZ());
  pEventData->SetAt(122, pElectrons->getTotalAbsoluteMomentum(*pUnit));
  pEventData->SetAt(123, 0e0); //dElectron_psai_xy*180.0/pi);
  pEventData->SetAt(124, 0e0); //dElectron_psai_zy*180.0/pi);
  pEventData->SetAt(125, 0e0); //dElectron_psai_zx*180.0/pi);
  logFile << "203-125 datas are writen. " << std::endl;

  int IonMasterFlag;
  if (pIons->getIon(0).getObjectFlag().isMasterOnFlag()
      && pIons->getIon(0).getObjectFlag().isValidOnFlag()) {
    IonMasterFlag = 1;
  } else {
    IonMasterFlag = -1;
  }
  int ElectronMasterFlag;
  if (pElectrons->getElectron(0).getObjectFlag().isMasterOnFlag()
      && pElectrons->getElectron(0).getObjectFlag().isValidOnFlag()) {
    ElectronMasterFlag = 1;
  } else {
    ElectronMasterFlag = -1;
  }
  pEventData->SetAt(126, IonMasterFlag);
  pEventData->SetAt(127, ElectronMasterFlag);
  logFile << "126, 127 datas are writen. " << std::endl;

  pEventData->SetAt(128, pIons->getLocationXOfCOM(*pUnit));
  pEventData->SetAt(129, pIons->getLocationXOfCOM(*pUnit));
  pEventData->SetAt(130, 0e0); //dCOM_z/unit_milli);
  pEventData->SetAt(131, pElectrons->getLocationXOfCOM(*pUnit));
  pEventData->SetAt(132, pElectrons->getLocationYOfCOM(*pUnit));
  logFile << "128-132 datas are writen. " << std::endl;

   pEventData->SetAt(133, 0e0); //dphai*180.0/pi);
   pEventData->SetAt(134, 0e0); //dEX/unit_milli);
   pEventData->SetAt(135, 0e0); //dEY/unit_milli);
   pEventData->SetAt(136, 0e0); //dElectron_theta0[0]*180.0/pi);
   pEventData->SetAt(137, 0e0); //dElectron_posi_xy*180.0/pi);
   pEventData->SetAt(138, 0e0); //dIon_Electron_Angle0*180.0/pi);
   pEventData->SetAt(139, 0e0); //isellect);
   pEventData->SetAt(140, 0e0); //dtN/unit_nano);
   pEventData->SetAt(141, 0e0); //dtO/unit_nano);
   double dAngle_Width = 10e0; 
   for(int ii=0; ii<int(180/dAngle_Width+0.1); ii++){
       pEventData->SetAt(142+ii*3, 0e0); //dIon_Angle[ii]*180.0/pi);
       pEventData->SetAt(142+ii*3+1, 0e0); //dElectron_Angle[ii]*180.0/pi);
       pEventData->SetAt(142+ii*3+2, 0e0); //dIon_Electron_Angle[ii]*180.0/pi);
   }
   pEventData->SetAt(196, 0e0); //d_I_Angle*180.0/pi);
   pEventData->SetAt(197, 0e0); //d_e_I_Angle*180.0/pi);
   pEventData->SetAt(198, 0e0); //distancexy/unit_milli);
   pEventData->SetAt(205, 0e0); //(dtIon[0]+dtIon[1])/unit_nano);
   pEventData->SetAt(206, 0e0); //(dtIon[0]+dtIon[2])/unit_nano);
   pEventData->SetAt(207, 0e0); //(dtIon[0]+dtIon[3])/unit_nano);
   pEventData->SetAt(208, 0e0); //(dtIon[1]+dtIon[2])/unit_nano);
   pEventData->SetAt(209, 0e0); //(dtIon[1]+dtIon[3])/unit_nano);
   pEventData->SetAt(210, 0e0); //(dtIon[2]+dtIon[3])/unit_nano);
   logFile << "133-210 datas are writen. " << std::endl;
  return;
}

CDAN_API void AnalysisFinalize(CDoubleArray *pEventData,
                               CDoubleArray *pParameters,
                               CDoubleArray *pWeighParameter) {
  logFile.close();
  return;
}
