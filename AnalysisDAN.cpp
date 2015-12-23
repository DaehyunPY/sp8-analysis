///////////////////////////////////////////////////////////////////////////
//  User defined analysis part called from cobold main program
///////////////////////////////////////////////////////////////////////////

#include "Analysis/AnalysisTools.h"

Analysis::Unit *pUnit;
Analysis::JSONReader *pJSONReader;
Analysis::EventDataReader *pEventDataReader;
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

CDAN_API LPCTSTR
AnalysisGetInformationString() {
  return LPCTSTR("Analysis");
}

/////////////////////////////////////////////////////////////////////////////
// Analysis functions

CDAN_API BOOL
AnalysisInitialize(CDoubleArray
*pEventData,
CDoubleArray *pParameters,
    CDoubleArray
*pWeighParameter) {
logFile.open("log.txt", std::fstream::out);
logFile << "The path is setted here. " <<
std::endl;

pUnit = new Analysis::Unit;
pJSONReader = new Analysis::JSONReader("parameters.json");
pAnalysisTools = new Analysis::AnalysisTools(*pUnit, *pJSONReader);
pIons = new Analysis::Ions(*pUnit, *pJSONReader, numberOfHitsUsed);
pElectrons = new Analysis::Electrons(*pUnit, *pJSONReader, numberOfHitsUsed);
logFile << "Initialization is successed. " <<
std::endl;

{
{
const int &n = pIons->getNumberOfObjects();
for (
int i = 0;
i<n;
i++) {
const std::string name = getObjectName(0);
const double t = pAnalysisTools->calculateTOF(*pUnit,
                                              pIons->getIon(i),
                                              0e0);
logFile << "TOF of " + name + "ion: " << t <<
std::endl;
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
logFile << "TOF of electron: " << t1 <<
std::endl;
logFile << "Period of cycle of electron: " << t2 <<
std::endl;
}
}
return
TRUE;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray * pEventData,
                                   CDoubleArray * pParameters,
                                   CDoubleArray * pWeighParameter) {
  {
    pEventDataReader = NULL;
    Analysis::EventData eventData;
    const int &nTDC = eventData.d1;
    const int &nCH = eventData.d2;
    const int &nHit = eventData.d3;
    for (int iTDC = 0; iTDC < nTDC; iTDC++) {
      for (int iCH = 0; iCH < nCH; iCH++) {
        for (int iHit = 0; iHit < nHit; iHit++) {
          eventData.array[iTDC][iCH][iHit] = pEventData->GetAt(iHit + iCH * (nHit + 1) + iTDC * (nHit + 1) * nCH) / 1e3;
        }
      }
    }
    pEventDataReader = new Analysis::EventDataReader(eventData);
  }
  
  pIons->resetEventData();
  pElectrons->resetEventData();
  int ionMasterFlag = 0;
  int electronMasterFlag = 0;
  pAnalysisTools->loadEventDataInputer(*pIons, *pUnit, *pEventDataReader);
  pAnalysisTools->loadEventDataInputer(*pElectrons, *pUnit, *pEventDataReader);

//  logFile << pIons->getNumberOfDeadRealOrDummyObjects() << ", " <<
//      pElectrons->getNumberOfDeadRealOrDummyObjects() << std::endl;
  if (pIons->isAllDeadRealAndDummyObjects()
      || pElectrons->isAllDeadRealAndDummyObjects()) {
    pIons->resetEventData();
    pElectrons->resetEventData();
    ionMasterFlag = -10;
    electronMasterFlag = -10;
    if (pIons->isAllDeadRealAndDummyObjects()) { ionMasterFlag = -11; }
    if (pElectrons->isAllDeadRealAndDummyObjects()) {
      electronMasterFlag = -11;
    }
    goto output;
  }


//  ion:
  if (pIons->existDeadObject()) {
    ionMasterFlag = -20;
    goto electron;
  }
  if (!pIons->isAllWithinMasterRegion()) {
    ionMasterFlag = -21;
    goto electron;
  }
  pAnalysisTools->loadMomentumCalculator(*pIons);
  ionMasterFlag = 1;


  electron:
  if (pElectrons->existDeadObject()) {
    electronMasterFlag = -20;
    goto output;
  }
  if (!pElectrons->isAllWithinMasterRegion()) {
    electronMasterFlag = -21;
    goto output;
  }
  pAnalysisTools->loadMomentumCalculator(*pElectrons);
  electronMasterFlag = 1;


  output:
// write ion data
  {
    const int &n = pIons->getNumberOfObjects();
    const int &m = pIons->getNumberOfRealOrDummyObjects();
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
  pEventData->SetAt(128, pIons->getLocationX(*pUnit));
  pEventData->SetAt(129, pIons->getLocationY(*pUnit));
  pEventData->SetAt(205, pIons->getSumOfTOF(*pUnit,0,1));
  pEventData->SetAt(206, pIons->getSumOfTOF(*pUnit,0,2));
  pEventData->SetAt(207, pIons->getSumOfTOF(*pUnit,0,3));
  pEventData->SetAt(208, pIons->getSumOfTOF(*pUnit,1,2));
  pEventData->SetAt(209, pIons->getSumOfTOF(*pUnit,1,3));
  pEventData->SetAt(210, pIons->getSumOfTOF(*pUnit,2,3));
//  pEventData->SetAt(130, 0e0); //dCOM_z/unit_milli);
  pEventData->SetAt(88, pIons->getMomentumX(*pUnit));
  pEventData->SetAt(89, pIons->getMomentumY(*pUnit));
  pEventData->SetAt(90, pIons->getMomentumZ(*pUnit));
  pEventData->SetAt(75, pIons->getEnergy(*pUnit));
//  pEventData->SetAt(95,dIon_nPx);                 // Ion_Pnx
//  pEventData->SetAt(96,dIon_nPy);                 // Ion_Pny
//  pEventData->SetAt(97,dIon_nPz);                 // Ion_Pnz
//  pEventData->SetAt(91, 0e0); //dIon_theta*180.0/pi);
  pEventData->SetAt(92, pIons->getIon(0).getMotionalDirectionXY(*pUnit));
  pEventData->SetAt(93, pIons->getIon(0).getMotionalDirectionZY(*pUnit));
  pEventData->SetAt(94, pIons->getIon(0).getMotionalDirectionZX(*pUnit));
//
// write electron data
  {
    const int &n = pElectrons->getNumberOfObjects();
    const int &m = pElectrons->getNumberOfRealOrDummyObjects();
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
  pEventData->SetAt(131, pElectrons->getLocationX(*pUnit));
  pEventData->SetAt(132, pElectrons->getLocationY(*pUnit));
//  pEventData->SetAt(119,dElectronPx_norm[0]);         // Normalized electron Px
//  pEventData->SetAt(120,dElectronPy_norm[0]);         // Normalized electron Py
//  pEventData->SetAt(121,dElectronPz_norm[0]);         // Normalized electron Pz
  pEventData->SetAt(122, pElectrons->getMomentum(*pUnit));
  pEventData->SetAt(137, pElectrons->getElectron(0).getLocationalDirectionXY(*pUnit));
  pEventData->SetAt(198, pElectrons->getElectron(0).getLocation(*pUnit));
  pEventData->SetAt(136, pElectrons->getElectron(0).getMotionalDirectionZ(*pUnit));
  pEventData->SetAt(123, pElectrons->getElectron(0).getMotionalDirectionXY(*pUnit));
  pEventData->SetAt(124, pElectrons->getElectron(0).getMotionalDirectionZY(*pUnit));
  pEventData->SetAt(125, pElectrons->getElectron(0).getMotionalDirectionZX(*pUnit));
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
//
//  write ion & electron data
  {
    const int &n = pElectrons->getNumberOfObjects();
    for (int i = 0; i < n; i++) {
      pEventData->SetAt(199 + i,
                        pIons->getEnergy(*pUnit)
                            + pElectrons->getElectron(i).getEnergy(*pUnit));
    }
  }
//
// write flag data
  pEventData->SetAt(126, ionMasterFlag);
  pEventData->SetAt(127, electronMasterFlag);
//
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
  return;
}

CDAN_API void AnalysisFinalize(CDoubleArray * pEventData,
                               CDoubleArray * pParameters,
                               CDoubleArray * pWeighParameter) {
  logFile.close();
  return;
}
