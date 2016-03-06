///////////////////////////////////////////////////////////////////////////
//  User defined analysis part called from cobold main program
///////////////////////////////////////////////////////////////////////////

#include "Windows4Root.h"
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
	int ionFlag;
	int electronFlag;

	pRun->ProcessEvent(reader, ionFlag, electronFlag);
	const auto &unit = pRun->getUnit();
	const auto &ions = pRun->getIons();
	const auto &electrons = pRun->getElectrons();

	// write flag data
	pEventData->SetAt(126, ionFlag);
	pEventData->SetAt(127, electronFlag);

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
	pEventData->SetAt(211, ions.getDiffOfTOF(unit, 0, 1));
	pEventData->SetAt(212, ions.getDiffOfTOF(unit, 0, 2));
	pEventData->SetAt(213, ions.getDiffOfTOF(unit, 0, 3));
	pEventData->SetAt(214, ions.getDiffOfTOF(unit, 1, 2));
	pEventData->SetAt(215, ions.getDiffOfTOF(unit, 1, 3));
	pEventData->SetAt(216, ions.getDiffOfTOF(unit, 2, 3));
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
}

///////////////////////
// AnalysisFinalize
///////////////////////
// is called when analysis is stopped (not paused!)
CDAN_API void AnalysisFinalize(CDoubleArray* pEventData, CDoubleArray* pParameters, CDoubleArray* pWeighParameter)
{
	delete pRun;
}
