///////////////////////////////////////////////////////////////////////////
//  User defined analysis part called from cobold main program
///////////////////////////////////////////////////////////////////////////

#include "Analysis/Analysis.h"

Analysis::Unit* pUnit = nullptr;
Analysis::JSONReader* pJSONReader = nullptr;
Analysis::EventDataReader* pEventDataReader = nullptr;
Analysis::AnalysisTools* pAnalysisTools = nullptr;
Analysis::Ions* pIons = nullptr;
Analysis::Electrons* pElectrons = nullptr;
Analysis::LogWriter* pLogWriter = nullptr;
const int numberOfTDCUsed = 4;
const int numberOfChannelsUsed = 2;
const int numberOfHitsUsed = 3;
bool optionOfSendingOutOfFrame = true;
bool optionOfExportingElectronMomentum = true;
std::fstream exportedFile;

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
	srand((unsigned int)time(nullptr));

	// make unit helper 
assert(pUnit != nullptr);
	pUnit = new Analysis::Unit;

	// make json reader and log writer
assert(pJSONReader != nullptr);
	pJSONReader = new Analysis::JSONReader("Parameters.json");
assert(pLogWriter != nullptr);
	pLogWriter = new Analysis::LogWriter(*pJSONReader); 
	// log it 
	pLogWriter->logJSONReader(*pJSONReader); 

	// make analysis tools, ions, and electrons 
assert(pAnalysisTools != nullptr);
	pAnalysisTools = new Analysis::AnalysisTools(*pUnit, *pJSONReader);
	{
assert(pIons != nullptr);
		pIons = new Analysis::Ions(*pUnit, *pJSONReader, numberOfHitsUsed);
assert(pElectrons != nullptr);
		pElectrons = new Analysis::Electrons(*pUnit, *pJSONReader, numberOfHitsUsed);
	}
	// log it 
	pLogWriter->logAnalysisTools(*pUnit, *pAnalysisTools, *pIons, *pElectrons);

	// output option
	optionOfSendingOutOfFrame = pJSONReader->getBoolAt("output_options.send_out_of_frame");
	optionOfExportingElectronMomentum = pJSONReader->getBoolAt("output_options.export_electron_momentum");
	// log it
	pLogWriter->write() << "Output Options: " << std::endl;
	pLogWriter->write() << "    Send Out of Frame: " << (optionOfSendingOutOfFrame ? "true" : "false") << std::endl;
	pLogWriter->write() << "    Export electron momentum: " << (optionOfExportingElectronMomentum ? "true" : "false") << std::endl;
	pLogWriter->write() << std::endl;

	// export electron momentum
	if (optionOfExportingElectronMomentum) {
		std::string filename; 
		filename = pAnalysisTools->getID();
		if (!(filename == "")) { filename += "-"; }
		filename += pLogWriter->getID(); 
		filename += ".csv";
		exportedFile.open(filename, std::fstream::out);
		exportedFile << "1st hit electron px" << ",";
		exportedFile << "1st hit electron py" << ",";
		exportedFile << "1st hit electron pz" << ",";
		exportedFile << "1st hit electron E"  << ",";
		exportedFile << "2nd hit electron px" << ",";
		exportedFile << "2nd hit electron py" << ",";
		exportedFile << "2nd hit electron pz" << ",";
		exportedFile << "2nd hit electron E"  << ",";
		exportedFile << "3rd hit electron px" << ",";
		exportedFile << "3rd hit electron py" << ",";
		exportedFile << "3rd hit electron pz" << ",";
		exportedFile << "3rd hit electron E"  << ",";
		exportedFile << "4th hit electron px" << ",";
		exportedFile << "4th hit electron py" << ",";
		exportedFile << "4th hit electron pz" << ",";
		exportedFile << "4th hit electron E"  << ",";
		exportedFile << std::endl;
	}

	// initialization is done
	delete pJSONReader; 
	// log it
	pLogWriter->write() << "Initialization is done." << std::endl;
	pLogWriter->write() << std::endl;
	return true;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray* pEventData,
                                   CDoubleArray* pParameters,
                                   CDoubleArray* pWeighParameter)
{
	// make event data reader 
	assert(pEventDataReader != nullptr);
	pEventDataReader = new Analysis::EventDataReader(numberOfTDCUsed,
													 numberOfChannelsUsed,
													 numberOfHitsUsed);
	{
		const int &nTDC = pEventDataReader->getNumberOfTDCUsed();
		const int &nCH = pEventDataReader->getNumberOfChannelsUsed();
		const int &nHit = pEventDataReader->getNumberOfHitsUsed();
		for (int iTDC = 0; iTDC < nTDC; iTDC++)
		{
			for (int iCH = 0; iCH < nCH; iCH++)
			{
				for (int iHit = 0; iHit < nHit; iHit++)
				{
					pEventDataReader->setEventDataAt(iTDC, iCH, iHit,
													 pEventData->GetAt(iHit
																			   + iCH
																					   * (nHit
																							   + 1)
																			   + iTDC
																					   * (nHit
																							   + 1)
																					   * nCH)
															 / 1e3);
				}
			}
		}
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
			if(optionOfSendingOutOfFrame) { pIons->setAllOfRealOrDummyObjectIsOutOfFrameOfBasicDataFlag(); }
			if(optionOfSendingOutOfFrame) { pElectrons->setAllOfRealOrDummyObjectIsOutOfFrameOfBasicDataFlag(); }
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
			if (optionOfSendingOutOfFrame) { pIons->setAllOfRealOrDummyObjectIsOutOfFrameOfMomentumDataFlag(); }
			if (existDeadObject) { ionMasterFlag = -21; }
			if (!areAllWithinMasterRegion) { ionMasterFlag = -22; }
			// don't calculate momentum 
			goto electron;
		}
		// calculate momentum 
		pAnalysisTools->loadMomentumCalculator(*pIons);
		if (existDeadObject) { ionMasterFlag = -30; }
	}
	ionMasterFlag = 1;


electron:
	{
		// if exist a object dead or not within master region
		const bool existDeadObject = pElectrons->existDeadObject();
		const bool areAllWithinMasterRegion = pElectrons->areAllWithinMasterRegion();
		if (existDeadObject || !areAllWithinMasterRegion)
		{
			// don't plot momentum data
			if (optionOfSendingOutOfFrame) { pElectrons->setAllOfRealOrDummyObjectIsOutOfFrameOfMomentumDataFlag(); }
			if (existDeadObject) { electronMasterFlag = -21; }
			if (!areAllWithinMasterRegion) { electronMasterFlag = -22; }
			// don't calculate momentum 
			goto output;
		}
		// calculate momentum 
		pAnalysisTools->loadMomentumCalculator(*pElectrons);
		if (existDeadObject) { electronMasterFlag = -30; }
	}
	electronMasterFlag = 1;


output:
	// export electron momentum
	if (optionOfExportingElectronMomentum) {
		if (ionMasterFlag > 0 && electronMasterFlag > 0) {
			const int& n = pElectrons->getNumberOfObjects();
			for (int i = 0; i < n; i++) {
				exportedFile << pElectrons->getElectron(i).getMomentumX(*pUnit) << ",";
				exportedFile << pElectrons->getElectron(i).getMomentumY(*pUnit) << ",";
				exportedFile << pElectrons->getElectron(i).getMomentumZ(*pUnit) << ",";
				exportedFile << pElectrons->getElectron(i).getEnergy(*pUnit) << ",";
			}
			exportedFile << std::endl;
		}
	}

	// write flag data
	pEventData->SetAt(126, ionMasterFlag);
	pEventData->SetAt(127, electronMasterFlag);

	// write ion data
	// if dummy object, don't plot momentum data 
	if (optionOfSendingOutOfFrame) { pIons->setAllOfDummyOfjectIsOutOfFrameOfMomentumDataFlag(); }
	{
		// plot data of a object 
		const int& m = pIons->getNumberOfRealOrDummyObjects();
		for (int i = 0; i < m; i++)
		{
			// if dead object, don't plot it
			if (optionOfSendingOutOfFrame) {
				if (pIons->getRealOrDummyIon(i).isDead()) {
					pIons->setRealOrDummyIonMembers(i).setFlagMembers().setOutOfFrameOfBaicDataFlag();
				}
			}
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
	if(optionOfSendingOutOfFrame) { pElectrons->setAllOfDummyOfjectIsOutOfFrameOfMomentumDataFlag(); }
	{
		// plot data of a object 
		const int& m = pElectrons->getNumberOfRealOrDummyObjects();
		for (int i = 0; i < m; i++)
		{
			// if dead object, don't plot it
			if(optionOfSendingOutOfFrame) {
				if (pElectrons->setRealOrDummyElectronMembers(i).isDead()) {
					pElectrons->setRealOrDummyElectronMembers(i).setFlagMembers().setOutOfFrameOfBaicDataFlag();
				}
			}
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

	delete pEventDataReader; 
}

///////////////////////
// AnalysisFinalize
///////////////////////
// is called when analysis is stopped (not paused!)
CDAN_API void AnalysisFinalize(CDoubleArray *pEventData, CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
	// finalization
	pLogWriter->write() << "Event count: " << pAnalysisTools->getEventNumber() << std::endl;
	delete pUnit; 
	delete pJSONReader; // for sure 
	delete pEventDataReader; // for sure 
	delete pAnalysisTools; 
	delete pIons; 
	delete pElectrons; 
	if (optionOfExportingElectronMomentum) { exportedFile.close(); }
	// log it
	pLogWriter->write() << "Finalization is done." << std::endl;
	pLogWriter->write() << std::endl;
	delete pLogWriter; 
}
