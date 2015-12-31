///////////////////////////////////////////////////////////////////////////
//  User defined analysis part called from cobold main program
///////////////////////////////////////////////////////////////////////////

#include "RUN.h"
BL17Analysis::Run *pRun;

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
pRun = new BL17Analysis::Run;
	return true;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray* pEventData,
                                   CDoubleArray* pParameters,
                                   CDoubleArray* pWeighParameter)
{
	// make event data reader 
	Analysis::EventDataReader reader(pRun->getNumberOfTDCUsed(),
	                                 pRun->getNumberOfChannelsUsed(),
	                                 pRun->getNumberOfHitsUsed());
	{
		const int& nTDC = reader.getNumberOfTDCUsed();
		const int& nCH = reader.getNumberOfChannelsUsed();
		const int& nHit = reader.getNumberOfHitsUsed();
		for (int iTDC = 0; iTDC < nTDC; iTDC++)
		{
			for (int iCH = 0; iCH < nCH; iCH++)
			{
				for (int iHit = 0; iHit < nHit; iHit++)
				{
					reader.setEventDataAt(iTDC, iCH, iHit,
					                      pEventData->GetAt(iHit + iCH * (nHit + 1) + iTDC * (nHit + 1) * nCH) / 1e3);
				}
			}
		}
	}
	int ionMasterFlag;
	int electronMasterFlag;

	pRun->ProcessEvent(reader, ionMasterFlag, electronMasterFlag);
	auto &unit = pRun->getUnit();
	auto &ions = pRun->getIons();
	auto &electrons = pRun->getElectrons();

	// write flag data
	pEventData->SetAt(126, ionMasterFlag);
	pEventData->SetAt(127, electronMasterFlag);

	// write ion data
	{
		// plot data of a object 
		const int &m = ions.getNumberOfRealOrDummyObjects();
		for (int i = 0; i < m; i++)
		{
			// basic data
			pEventData->SetAt(31 + 28 + i,
							  ions.getRealOrDummyIon(i).getLocationX(unit));
			pEventData->SetAt(31 + 32 + i,
							  ions.getRealOrDummyIon(i).getLocationY(unit));
			pEventData->SetAt(67 + i, ions.getRealOrDummyIon(i).getTOF(unit));
			// momentum data 
			pEventData->SetAt(76 + i,
							  ions.getRealOrDummyIon(i).getMomentumX(unit));
			pEventData->SetAt(80 + i,
							  ions.getRealOrDummyIon(i).getMomentumY(unit));
			pEventData->SetAt(84 + i,
							  ions.getRealOrDummyIon(i).getMomentumZ(unit));
			pEventData->SetAt(67 + 4 + i,
							  ions.getRealOrDummyIon(i).getEnergy(unit));
		}
	}
	// plot data of objects 
	// basic data 
	pEventData->SetAt(128, ions.getLocationX(unit));
	pEventData->SetAt(129, ions.getLocationY(unit));
	pEventData->SetAt(205, ions.getSumOfTOF(unit, 0, 1));
	pEventData->SetAt(206, ions.getSumOfTOF(unit, 0, 2));
	pEventData->SetAt(207, ions.getSumOfTOF(unit, 0, 3));
	pEventData->SetAt(208, ions.getSumOfTOF(unit, 1, 2));
	pEventData->SetAt(209, ions.getSumOfTOF(unit, 1, 3));
	pEventData->SetAt(210, ions.getSumOfTOF(unit, 2, 3));
	// momentum data 
	pEventData->SetAt(88, ions.getMomentumX(unit));
	pEventData->SetAt(89, ions.getMomentumY(unit));
	pEventData->SetAt(90, ions.getMomentumZ(unit));
	pEventData->SetAt(75, ions.getEnergy(unit));
	pEventData->SetAt(92, ions.getMotionalDirectionXY(unit));
	pEventData->SetAt(93, ions.getMotionalDirectionZY(unit));
	pEventData->SetAt(94, ions.getMotionalDirectionZX(unit));

	// write electron data
	{
		// plot data of a object 
		const int &m = electrons.getNumberOfRealOrDummyObjects();
		for (int i = 0; i < m; i++)
		{
			// basic data
			pEventData->SetAt(31 + i,
							  electrons.getRealOrDummyElectron(i).getLocationX(unit));
			pEventData->SetAt(31 + 4 + i,
							  electrons.getRealOrDummyElectron(i).getLocationY(unit));
			pEventData->SetAt(98 + i,
							  electrons.getRealOrDummyElectron(i).getTOF(unit));
			// momentum data 
			pEventData->SetAt(107 + i,
							  electrons.getRealOrDummyElectron(i).getMomentumX(unit));
			pEventData->SetAt(111 + i,
							  electrons.getRealOrDummyElectron(i).getMomentumY(unit));
			pEventData->SetAt(115 + i,
							  electrons.getRealOrDummyElectron(i).getMomentumZ(unit));
			pEventData->SetAt(103 + i,
							  electrons.getRealOrDummyElectron(i).getEnergy(unit));
		}
	}
	// plot data of objects 
	// basic data 
	pEventData->SetAt(131, electrons.getLocationX(unit));
	pEventData->SetAt(132, electrons.getLocationY(unit));
	pEventData->SetAt(122, electrons.getMomentum(unit));
	pEventData->SetAt(137, electrons.getLocationalDirectionXY(unit));
	pEventData->SetAt(198, electrons.getLocation(unit));
	// momentum data 
	pEventData->SetAt(136, electrons.getMotionalDirectionZ(unit));
	pEventData->SetAt(123, electrons.getMotionalDirectionXY(unit));
	pEventData->SetAt(124, electrons.getMotionalDirectionZY(unit));
	pEventData->SetAt(125, electrons.getMotionalDirectionZX(unit));

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
	//		const int& n = electrons.getNumberOfObjects();
	//		for (int i = 0; i < n; i++)
	//		{
	//			pEventData->SetAt(199 + i,
	//			                  ions.getEnergy(unit)
	//			                  + electrons.getElectron(i).getEnergy(unit));
	//		}
}

///////////////////////
// AnalysisFinalize
///////////////////////
// is called when analysis is stopped (not paused!)
CDAN_API void AnalysisFinalize(CDoubleArray* pEventData, CDoubleArray* pParameters, CDoubleArray* pWeighParameter)
{
	delete pRun;
}
