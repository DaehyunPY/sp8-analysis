// CDan.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#include "CDan.h"
#include "math.h"
#include "CDaq.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct TACinfo {
	int min;
	int max;
};

TACinfo	TACinfoGate(int,int,int);

/////////////////////////////////////////////////////////////////////////////
// CCDanApp

BEGIN_MESSAGE_MAP(CCDanApp, CWinApp)
	//{{AFX_MSG_MAP(CCDanApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCDanApp construction

CCDanApp::CCDanApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCDanApp object

CCDanApp theApp;

LARGE_INTEGER GetTimeInformation(CDoubleArray *pEventData,CDoubleArray *pParameters)
{
	LARGE_INTEGER liTime;

	int iTimeFormat = (int)(pParameters->GetAt(1)+0.1) ? (int)(pParameters->GetAt(39)+0.1) : false;

	unsigned short *psData = (unsigned short *)&liTime;
	unsigned long *plData = (unsigned long *)&liTime;

	switch(iTimeFormat)
	{
		case LM_BYTE:	//  8bit integer
			break;
		case LM_SHORT:	// 16bit integer
			psData[0] = (int)(pEventData->GetAt(0)+0.1);
			psData[1] = (int)(pEventData->GetAt(1)+0.1);
			psData[2] = (int)(pEventData->GetAt(2)+0.1);
			psData[3] = (int)(pEventData->GetAt(3)+0.1);
			break;
		case LM_LONG:	// 32bit integer
			psData[0] = (int)(pEventData->GetAt(0)+0.1);
			psData[0] = (int)(pEventData->GetAt(0)+0.1);
			break;
		case LM_FLOAT:  // 32bit IEEE float
			break;
		case LM_DOUBLE:	// 64bit IEEE float
			break;
		case LM_DOUBLELONG:	// 24bit integer
			break;
		default:
			liTime.HighPart = 0;
			liTime.LowPart = 0;
	}
	
	return liTime;
}


/////////////////////////////////////////////////////////////////////////////
// Local variables used for Event Time functions
double _dOldEventTime;
double _dStartEventTime;

/////////////////////////////////////////////////////////////////////////////
// GetDeltaEventTime
// -----------------
// Calculate the time between two events
// returns time in seconds (always 0 for the first event!)
double GetDeltaEventTime(CDoubleArray *pEventData,CDoubleArray *pParameters)
{
	LARGE_INTEGER liTime = GetTimeInformation(pEventData,pParameters);

	double dTimeScaling = pParameters->GetAt(4);	// frequency, in counts per second, of the high-resolution performance counter
	double dEventTime = (double)liTime.QuadPart;
	dEventTime /= dTimeScaling;
	double dDeltaEventTime = _dOldEventTime != 0.0 ? dEventTime - _dOldEventTime : 0.0;
	_dOldEventTime = dEventTime;

	return dDeltaEventTime;
}

/////////////////////////////////////////////////////////////////////////////
// GetEventTime
// ------------
// Calculate the time between the aktual event and the first event
// returns time in seconds
double GetEventTime(CDoubleArray *pEventData,CDoubleArray *pParameters)
{
	LARGE_INTEGER liTime = GetTimeInformation(pEventData,pParameters);

	double dTimeScaling = pParameters->GetAt(4);	// frequency, in counts per second, of the high-resolution performance counter
	double dEventTime = (double)liTime.QuadPart;
	dEventTime /= dTimeScaling;
	if(_dStartEventTime == 0)
		_dStartEventTime = dEventTime;
	return (dEventTime - _dStartEventTime);
}

/////////////////////////////////////////////////////////////////////////////
// CorrectEventDataPosistion
// -------------------------
// Moves raw data to the correct position due to time information for the event
// so that the ADC/TDC raw data always starts with the same coordinate
void CorrectEventDataPosistion(CDoubleArray *pEventData,int iIndexOffset, int iEndCounter)
{
	// move TDC data to correct position for DAN
	if(iIndexOffset)
		for(int iCount=iEndCounter;iCount>3;iCount--)
			pEventData->SetAt(iCount,pEventData->GetAt(iCount-iIndexOffset));

	// no Time-Information so set coordinate 1..4 to 0
	if(iIndexOffset == 4)
	{
		pEventData->SetAt(0,0.0);
		pEventData->SetAt(1,0.0);
		pEventData->SetAt(2,0.0);
		pEventData->SetAt(3,0.0);
	}

	// 32 Bit Time-Information so set coordinate 2..4 to 0
	if(iIndexOffset == 2)
	{
		pEventData->SetAt(2,0.0);
		pEventData->SetAt(3,0.0);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CorrectGP1NDL
// -------------
// corrects the differential nonlinearity of the GP1/HM1 module
// returns the corrected
double CorrectGP1NDL(double dChannel, double dCorrectionValue)
{
	extern double dpDnlCorrection;
	double dChStart = dChannel;
	double dOldChannel = dChannel;
	int iRand;
	double dRand;
	bool bMoved = false;
	if(fmod(dChannel,2.) == 0)
	{
		iRand = rand();
		dRand = (double)iRand / (double)(RAND_MAX);
		if(dRand <= dCorrectionValue)
		{
			dChannel += 1.;
			bMoved = true;
		}
	}
	if(bMoved)
		return dChannel;
	else
		return dOldChannel;
}
