///////////////////////////////////////////////////////////////////////////
//  User defined analysis part called from cobold main program
///////////////////////////////////////////////////////////////////////////

#include "Analysis/LogWriter.h"

Analysis::Unit* pUnit = nullptr;
Analysis::JSONReader* pJSONReader = nullptr;
Analysis::EventDataReader* pEventDataReader = nullptr;
Analysis::AnalysisTools* pAnalysisTools = nullptr;
Analysis::Ions* pIons = nullptr;
Analysis::Electrons* pElectrons = nullptr;
Analysis::LogWriter* pLogWriter = nullptr;

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
AnalysisGetInformationString()
{
	return LPCTSTR("Analysis");
}

/////////////////////////////////////////////////////////////////////////////
// Analysis functions

CDAN_API BOOL
AnalysisInitialize(CDoubleArray* pEventData, CDoubleArray* pParameters, CDoubleArray* pWeighParameter)
{
	// set seed for random
	srand(std::time(nullptr));

	// make unit helper 
	pUnit = new Analysis::Unit;

	// make json reader and log writer
	pJSONReader = new Analysis::JSONReader("Parameters.json");
	pLogWriter = new Analysis::LogWriter(*pJSONReader); 
	// log it 
	pLogWriter->logJSONReader(*pJSONReader); 

	// make analysis tools, ions, and electrons 
	pAnalysisTools = new Analysis::AnalysisTools(*pUnit, *pJSONReader);
	{
		const int numberOfHitsUsed = NUMBER_OF_HITS_USED; 
		pIons = new Analysis::Ions(*pUnit, *pJSONReader, numberOfHitsUsed);
		pElectrons = new Analysis::Electrons(*pUnit, *pJSONReader, numberOfHitsUsed);
	}
	// log it 
	pLogWriter->logAnalysisTools(*pUnit, *pAnalysisTools, *pIons, *pElectrons); 

	// initialization is done, and log it 
	pJSONReader = nullptr; 
	pLogWriter->write() << "Initialization is done." << std::endl;
	pLogWriter->write() << std::endl;
	return TRUE;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray* pEventData,
                                   CDoubleArray* pParameters,
                                   CDoubleArray* pWeighParameter)
{
	// make event data reader 
	{
		pEventDataReader = nullptr;
		Analysis::EventData eventData;
		const int& nTDC = eventData.d1;
		const int& nCH = eventData.d2;
		const int& nHit = eventData.d3;
		for (int iTDC = 0; iTDC < nTDC; iTDC++)
		{
			for (int iCH = 0; iCH < nCH; iCH++)
			{
				for (int iHit = 0; iHit < nHit; iHit++)
				{
					eventData.array[iTDC][iCH][iHit] = pEventData->GetAt(iHit + iCH * (nHit + 1) + iTDC * (nHit + 1) * nCH) / 1e3;
				}
			}
		}
		pEventDataReader = new Analysis::EventDataReader(eventData);
	}

	// count event 
	pAnalysisTools->loadEventCounter(); 

	// make sure ion and electron data is empty, and reset flags 
	pIons->resetEventData();
	pElectrons->resetEventData();
	int ionMasterFlag = 0;
	int electronMasterFlag = 0;

	// input event data 
	pAnalysisTools->loadEventDataInputer(*pIons, *pUnit, *pEventDataReader);
	pAnalysisTools->loadEventDataInputer(*pElectrons, *pUnit, *pEventDataReader);

	{
		// if all event data is dead
		const bool ionsAreAllDead = pIons->areAllDeadRealAndDummyObjects(); 
		const bool electronsAreeAllDead = pElectrons->areAllDeadRealAndDummyObjects(); 
		if (ionsAreAllDead || electronsAreeAllDead)
		{
			// don't plot momentum data
			pIons->setAllOfRealOrDummyObjectIsOutOfFrameOfBasicDataFlag(); 
			pElectrons->setAllOfRealOrDummyObjectIsOutOfFrameOfBasicDataFlag(); 
			if (ionsAreAllDead) { ionMasterFlag = -10; }
			if (electronsAreeAllDead) { electronMasterFlag = -10; }
			// don't calculate momentum 
			goto output;
		}
	}


	//  ion:
	{
		// if exist a object dead or not within master region
		const bool existDeadObject = pIons->existDeadObject();
		const bool areAllWithinMasterRegion = pIons->areAllWithinMasterRegion();
		if (existDeadObject || !areAllWithinMasterRegion) 
		{
			// don't plot momentum data
			pIons->setAllOfRealOrDummyObjectIsOutOfFrameOfMomentumDataFlag();
			if (existDeadObject) { ionMasterFlag = -21; }
			if (!areAllWithinMasterRegion) { ionMasterFlag = -22; }
			// don't calculate momentum 
			goto electron;
		}		
	}
	// calculate momentum 
	pAnalysisTools->loadMomentumCalculator(*pIons);
	ionMasterFlag = 1;


electron:
	{
		// if exist a object dead or not within master region
		const bool existDeadObject = pElectrons->existDeadObject();
		const bool areAllWithinMasterRegion = pElectrons->areAllWithinMasterRegion();
		if (existDeadObject || !areAllWithinMasterRegion)
		{
			// don't plot momentum data
			pElectrons->setAllOfRealOrDummyObjectIsOutOfFrameOfMomentumDataFlag(); 
			if (existDeadObject) { electronMasterFlag = -21; }
			if (!areAllWithinMasterRegion) { electronMasterFlag = -22; }
			// don't calculate momentum 
			goto output;
		}
	}
	// calculate momentum 
	pAnalysisTools->loadMomentumCalculator(*pElectrons);
	electronMasterFlag = 1;


output:
	// write flag data
	pEventData->SetAt(126, ionMasterFlag);
	pEventData->SetAt(127, electronMasterFlag);

	// write ion data
	// if dummy object, don't plot momentum data 
	pIons->setAllOfDummyOfjectIsOutOfFrameOfMomentumDataFlag(); 
	{
		// plot data of a object 
		const int& m = pIons->getNumberOfRealOrDummyObjects();
		for (int i = 0; i < m; i++)
		{
			// if dead object, don't plot it 
			if (pIons->getRealOrDummyIon(i).isDead()) { pIons->setRealOrDummyIonMembers(i).setFlagMembers().setOutOfFrameOfBaicDataFlag(); }
			// basic data 
			pEventData->SetAt(31 + 28 + i, pIons->getRealOrDummyIon(i).getLocationX(*pUnit));
			pEventData->SetAt(31 + 32 + i, pIons->getRealOrDummyIon(i).getLocationY(*pUnit));
			pEventData->SetAt(67 + i, pIons->getRealOrDummyIon(i).getTOF(*pUnit));
			// momentum data 
			pEventData->SetAt(76 + i, pIons->getRealOrDummyIon(i).getMomentumX(*pUnit));
			pEventData->SetAt(80 + i, pIons->getRealOrDummyIon(i).getMomentumY(*pUnit));
			pEventData->SetAt(84 + i, pIons->getRealOrDummyIon(i).getMomentumZ(*pUnit));
			pEventData->SetAt(67 + 4 + i, pIons->getRealOrDummyIon(i).getEnergy(*pUnit));
		}
	}
	// plot data of objects 
	// basic data 
	pEventData->SetAt(128, pIons->getLocationX(*pUnit));
	pEventData->SetAt(129, pIons->getLocationY(*pUnit));
	pEventData->SetAt(205, pIons->getSumOfTOF(*pUnit, 0, 1));
	pEventData->SetAt(206, pIons->getSumOfTOF(*pUnit, 0, 2));
	pEventData->SetAt(207, pIons->getSumOfTOF(*pUnit, 0, 3));
	pEventData->SetAt(208, pIons->getSumOfTOF(*pUnit, 1, 2));
	pEventData->SetAt(209, pIons->getSumOfTOF(*pUnit, 1, 3));
	pEventData->SetAt(210, pIons->getSumOfTOF(*pUnit, 2, 3));
	// momentum data 
	pEventData->SetAt(88, pIons->getMomentumX(*pUnit));
	pEventData->SetAt(89, pIons->getMomentumY(*pUnit));
	pEventData->SetAt(90, pIons->getMomentumZ(*pUnit));
	pEventData->SetAt(75, pIons->getEnergy(*pUnit));
	pEventData->SetAt(92, pIons->getMotionalDirectionXY(*pUnit));
	pEventData->SetAt(93, pIons->getMotionalDirectionZY(*pUnit));
	pEventData->SetAt(94, pIons->getMotionalDirectionZX(*pUnit));

	// write electron data
	// if dummy object, don't plot momentum data 
	pElectrons->setAllOfDummyOfjectIsOutOfFrameOfMomentumDataFlag(); 
	{
		// plot data of a object 
		const int& m = pElectrons->getNumberOfRealOrDummyObjects();
		for (int i = 0; i < m; i++)
		{
			// if dead object, don't plot it 
			if (pElectrons->setRealOrDummyElectronMembers(i).isDead()) { pElectrons->setRealOrDummyElectronMembers(i).setFlagMembers().setOutOfFrameOfBaicDataFlag(); }
			// basic data 
			pEventData->SetAt(31 + i, pElectrons->setRealOrDummyElectronMembers(i).getLocationX(*pUnit));
			pEventData->SetAt(31 + 4 + i, pElectrons->setRealOrDummyElectronMembers(i).getLocationY(*pUnit));
			pEventData->SetAt(98 + i, pElectrons->setRealOrDummyElectronMembers(i).getTOF(*pUnit));
			// momentum data 
			pEventData->SetAt(107 + i, pElectrons->setRealOrDummyElectronMembers(i).getMomentumX(*pUnit));
			pEventData->SetAt(111 + i, pElectrons->setRealOrDummyElectronMembers(i).getMomentumY(*pUnit));
			pEventData->SetAt(115 + i, pElectrons->setRealOrDummyElectronMembers(i).getMomentumZ(*pUnit));
			pEventData->SetAt(103 + i, pElectrons->setRealOrDummyElectronMembers(i).getEnergy(*pUnit));
		}
	}
	// plot data of objects 
	// basic data 
	pEventData->SetAt(131, pElectrons->getLocationX(*pUnit));
	pEventData->SetAt(132, pElectrons->getLocationY(*pUnit));
	pEventData->SetAt(122, pElectrons->getMomentum(*pUnit));
	pEventData->SetAt(137, pElectrons->getLocationalDirectionXY(*pUnit));
	pEventData->SetAt(198, pElectrons->getLocation(*pUnit));
	// momentum data 
	pEventData->SetAt(136, pElectrons->getMotionalDirectionZ(*pUnit));
	pEventData->SetAt(123, pElectrons->getMotionalDirectionXY(*pUnit));
	pEventData->SetAt(124, pElectrons->getMotionalDirectionZY(*pUnit));
	pEventData->SetAt(125, pElectrons->getMotionalDirectionZX(*pUnit));

	//  ignore these 
//	  pEventData->SetAt(18, 0e0); //nHexX1);  // 18+0
//	  pEventData->SetAt(19, 0e0); //nHexX2);  // 18+1
//	  pEventData->SetAt(20, 0e0); //nHexY1);  // 18+2
//	  pEventData->SetAt(21, 0e0); //nHexY2);  // 18+3
//	  pEventData->SetAt(22, 0e0); //nHexZ1);  // 18+4
//	  pEventData->SetAt(23, 0e0); //nHexZ2);  // 18+5
//	  pEventData->SetAt(24, 0e0); //nMCPele); // 18+6
//	  pEventData->SetAt(25, 0e0); //nMCPion); // 18+7
//	  pEventData->SetAt(26, 0e0); //nSqX1);   // 18+8
//	  pEventData->SetAt(27, 0e0); //nSqX2);   // 18+9
//	  pEventData->SetAt(28, 0e0); //nSqY1);   // 18+10
//	  pEventData->SetAt(29, 0e0); //nSqY2);   // 18+11
//	  pEventData->SetAt(30, 0e0); //nBunch);  // 18+12
//	  pEventData->SetAt(133, 0e0); //dphai*180.0/pi);
//	  pEventData->SetAt(134, 0e0); //dEX/unit_milli);
//	  pEventData->SetAt(135, 0e0); //dEY/unit_milli);
//	  pEventData->SetAt(138, 0e0); //dIon_Electron_Angle0*180.0/pi);
//	  pEventData->SetAt(139, 0e0); //isellect);
//	  pEventData->SetAt(140, 0e0); //dtN/unit_nano);
//	  pEventData->SetAt(141, 0e0); //dtO/unit_nano);
//	  {
//	    double dAngle_Width = 10e0;
//	    for (int ii = 0; ii < int(180 / dAngle_Width + 0.1); ii++) {
//	      pEventData->SetAt(142 + ii * 3, 0e0); //dIon_Angle[ii]*180.0/pi);
//	      pEventData->SetAt(142 + ii * 3 + 1, 0e0); //dElectron_Angle[ii]*180.0/pi);
//	      pEventData->SetAt(142 + ii * 3 + 2,
//	                        0e0); //dIon_Electron_Angle[ii]*180.0/pi);
//	    }
//	  }
//	  pEventData->SetAt(196, 0e0); //d_I_Angle*180.0/pi);
//	  pEventData->SetAt(197, 0e0); //d_e_I_Angle*180.0/pi);
//	  pEventData->SetAt(130, 0e0); //dCOM_z/unit_milli);
//	  pEventData->SetAt(95,dIon_nPx); // Ion_Pnx
//	  pEventData->SetAt(96,dIon_nPy); // Ion_Pny
//	  pEventData->SetAt(97,dIon_nPz); // Ion_Pnz
//	  pEventData->SetAt(91, 0e0); //dIon_theta*180.0/pi);
//	  pEventData->SetAt(119,dElectronPx_norm[0]); // Normalized electron Px
//	  pEventData->SetAt(120,dElectronPy_norm[0]); // Normalized electron Py
//	  pEventData->SetAt(121,dElectronPz_norm[0]); // Normalized electron Pz
//	  pEventData->SetAt(102, 0e0); //dElectronEnergyXY/dElectron);
//	  {
//	    double higher, lower;
//	    if (dElectronEnergy[0] >= dElectronEnergy[1]) {
//	      higher = dElectronEnergy[0];
//	      lower = dElectronEnergy[1];
//	    } else {
//	      higher = dElectronEnergy[1];
//	      lower = dElectronEnergy[0];
//	    }
//	    pEventData->SetAt(203, 0e0); //dElectronEnergyHigher/dElectron);
//	    pEventData->SetAt(204, 0e0); //dElectronEnergyLower/dElectron);
//	  }
//		const int& n = pElectrons->getNumberOfObjects();
//		for (int i = 0; i < n; i++)
//		{
//			pEventData->SetAt(199 + i,
//			                  pIons->getEnergy(*pUnit)
//			                  + pElectrons->getElectron(i).getEnergy(*pUnit));
//		}

	pEventDataReader = nullptr;
	return;
}

CDAN_API void AnalysisFinalize(CDoubleArray* pEventData,
                               CDoubleArray* pParameters,
                               CDoubleArray* pWeighParameter)
{
	pLogWriter->write() << "Event count: " << pAnalysisTools->getEventNumber() << std::endl;
	pLogWriter->write() << "One LMF file is done." << std::endl; 
	pLogWriter->write() << std::endl;
	pUnit = nullptr;
	pJSONReader = nullptr; // for sure 
	pEventDataReader = nullptr; // for sure 
	pAnalysisTools = nullptr;
	pIons = nullptr;
	pElectrons = nullptr;
	pLogWriter = nullptr;
	return;
}
