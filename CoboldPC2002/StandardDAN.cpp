#include "../include/Analysis/AnalysisTools.h"
#include "CDan.h"

Analysis::Unit *pUnit;
Analysis::JSONReader *pJSONReader;
Analysis::LMFReader *pLMFReader;
Analysis::AnalysisTools *pAnalysisTools;
Analysis::Ions *pIons;
Analysis::Electrons *pElectrons;

CDAN_API BOOL AnalysisInitialize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter){
	pUnit = new Analysis::Unit;
	pJSONReader = new Analysis::JSONReader("parameters.json");
	pLMFReader = NULL;
	pAnalysisTools(*pUnit, *pJSONReader);
	pIons = new Analysis::Ions(*pUnit, *pJSONReader);
	pElectrons = new Analysis::Electrons(*pUnit, *pJSONReader);
	return true;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray *pEventData, CDoubleArray *pParameters, CDoubleArray *pWeighParameter) {
	pLMFReader = NULL;
	pLMFReader = new Analysis::LMFReader(*pParameters);
	pIons->setObjectMembers().resetEventData();
	pElectrons->setObjectMembers().resetEventData();
	pAnalysisTools->loadEventDataInputer(*pIons, *pUnit, *pLMFReader);
	pAnalysisTools->loadEventDataInputer(*pElectrons, *pUnit, *pLMFReader);
	pAnalysisTools->loadMomentumCalculator(*pIons);
	pAnalysisTools->loadMomentumCalculator(*pElectrons);

	const int nTDC = 3;
	const int nCH = 2;
	const int nHit = 4;

    for(int iHit=0; iHit<nHit; iHit++){
        pEventData->SetAt(31+iHit,pElectrons->getElectron(iHit).getLocationX(*pUnit));
        pEventData->SetAt(31+4+iHit,pElectrons->getElectron(iHit).getLocationY(*pUnit));
        pEventData->SetAt(31+28+iHit,pIons->getIon(iHit).getLocationX(*pUnit));
        pEventData->SetAt(31+32+iHit,pIons->getIon(iHit).getLocationY(*pUnit));
        pEventData->SetAt(67+iHit,pIons->getIon(iHit).getTOF(*pUnit));
        pEventData->SetAt(67+4+iHit,pIons->getIon(iHit).getEnergy(*pUnit));
    }
    pEventData->SetAt(75, pIons->getTotalEnergy(*pUnit);
    for(int iHit=0; iHit<nHit; iHit++){
        pEventData->SetAt(76+iHit,pIons->getIon(iHit).getMomentumX(*pUnit));
        pEventData->SetAt(80+iHit,pIons->getIon(iHit).getMomentumY(*pUnit));
        pEventData->SetAt(84+iHit,pIons->getIon(iHit).getMomentumZ(*pUnit));
    }
    pEventData->SetAt(88,pIons->getTotalMomentumX(*pUnit));
    pEventData->SetAt(89,pIons->getTotalMomentumY(*pUnit));
    pEventData->SetAt(90,pIons->getTotalMomentumZ(*pUnit));

    // pEventData->SetAt(91,dIon_theta*180.0/pi);
    // pEventData->SetAt(92,dIon_psai_xy*180.0/pi);
    // pEventData->SetAt(93,dIon_psai_zy*180.0/pi);
    // pEventData->SetAt(94,dIon_psai_zx*180.0/pi);
    // pEventData->SetAt(95,dIon_nPx);
    // pEventData->SetAt(96,dIon_nPy);
    // pEventData->SetAt(97,dIon_nPz);

    // pEventData->SetAt(102,dElectronEnergyXY/dElectron);
    for(int iHit=0; iHit<nHit; iHit++){
        pEventData->SetAt(98+iHit,pElectrons->getElectron(iHit).getTOF(*pUnit));
        pEventData->SetAt(103+iHit,pElectrons->getElectron(iHit).getEnergy(*pUnit));
        pEventData->SetAt(107+iHit,pElectrons->getElectron(iHit).getMomentumX(*pUnit));
        pEventData->SetAt(111+iHit,pElectrons->getElectron(iHit).getMomentumY(*pUnit));
        pEventData->SetAt(115+iHit,pElectrons->getElectron(iHit).getMomentumZ(*pUnit));
        pEventData->SetAt(199+iHit,pElectrons->getTotalEnergy(*pUnit)+pIons->getTotalEnergy(*pUnit));
    }

    // if(dElectronEnergy[0] >= dElectronEnergy[1]){
    //     dElectronEnergyHigher = dElectronEnergy[0];
    //     dElectronEnergyLower = dElectronEnergy[1];
    // } else{
    //     dElectronEnergyHigher = dElectronEnergy[1];
    //     dElectronEnergyLower = dElectronEnergy[0];
    // }
    // pEventData->SetAt(203,dElectronEnergyHigher/dElectron);
    // pEventData->SetAt(204,dElectronEnergyLower/dElectron);

    pEventData->SetAt(119,pElectrons->getElectron(iHit).getDirectionX());
    pEventData->SetAt(120,pElectrons->getElectron(iHit).getDirectionY());
    pEventData->SetAt(121,pElectrons->getElectron(iHit).getDirectionZ());
    pEventData->SetAt(122,pElectrons->getTotalAbsoluteMomentum(*pUnit));

    //  pEventData->SetAt(123,dElectron_theta[0]*180.0/pi);
    //  pEventData->SetAt(124,dElectron_psai[0]*180.0/pi);
    //  pEventData->SetAt(125,dElectron_psai_position[0]*180.0/pi);
    // pEventData->SetAt(123,dElectron_psai_xy*180.0/pi);
    // pEventData->SetAt(124,dElectron_psai_zy*180.0/pi);
    // pEventData->SetAt(125,dElectron_psai_zx*180.0/pi);

    int IonMasterFlag;
    if(pIons->getIon(0).getObjectFlag().isMasterOnFlag()
    	&& pIons->getIon(0).getObjectFlag().isValidOnFlag()) {
    	IonMasterFlag = 1;
    } else {
    	IonMasterFlag = -1;
    }
    int ElectronMasterFlag;
    if(pElectrons->getElectron(0).getObjectFlag().isMasterOnFlag()
    	&& pElectrons->getElectron(0).getObjectFlag().isValidOnFlag()) {
    	ElectronMasterFlag = 1;
    } else {
    	ElectronMasterFlag = -1;
    }
    pEventData->SetAt(126,IonMasterFlag);
    pEventData->SetAt(127,ElectronMasterFlag);

    pEventData->SetAt(128,pIons->getLocationXOfCOM(*pUnit));
    pEventData->SetAt(129,pIons->getLocationXOfCOM(*pUnit));
    // pEventData->SetAt(130,dCOM_z/unit_milli);
    pEventData->SetAt(131,pElectrons->getLocationXOfCOM(*pUnit));
    pEventData->SetAt(132,pElectrons->getLocationYOfCOM(*pUnit));

    // pEventData->SetAt(133,dphai*180.0/pi);
    // pEventData->SetAt(134,dEX/unit_milli);
    // pEventData->SetAt(135,dEY/unit_milli);
    // pEventData->SetAt(136,dElectron_theta0[0]*180.0/pi);
    // pEventData->SetAt(137,dElectron_posi_xy*180.0/pi);
    // pEventData->SetAt(138,dIon_Electron_Angle0*180.0/pi);
    // pEventData->SetAt(139,isellect);
    // pEventData->SetAt(140,dtN/unit_nano);
    // pEventData->SetAt(141,dtO/unit_nano);
    //
    // for(int ii=0; ii<int(180/dAngle_Width+0.1); ii++){
    //     pEventData->SetAt(142+ii*3,dIon_Angle[ii]*180.0/pi);
    //     pEventData->SetAt(142+ii*3+1,dElectron_Angle[ii]*180.0/pi);
    //     pEventData->SetAt(142+ii*3+2,dIon_Electron_Angle[ii]*180.0/pi);
    // }
    // pEventData->SetAt(196,d_I_Angle*180.0/pi);
    // pEventData->SetAt(197,d_e_I_Angle*180.0/pi);
    // pEventData->SetAt(198,distancexy/unit_milli);
    // pEventData->SetAt(205,(dtIon[0]+dtIon[1])/unit_nano);
    // pEventData->SetAt(206,(dtIon[0]+dtIon[2])/unit_nano);
    // pEventData->SetAt(207,(dtIon[0]+dtIon[3])/unit_nano);
    // pEventData->SetAt(208,(dtIon[1]+dtIon[2])/unit_nano);
    // pEventData->SetAt(209,(dtIon[1]+dtIon[3])/unit_nano);
    // pEventData->SetAt(210,(dtIon[2]+dtIon[3])/unit_nano);
    return;
}

CDAN_API void AnalysisFinalize(CDoubleArray *pEventData, CDoubleArray *pParameters, CDoubleArray *pWeighParameter) {
    return;
}
