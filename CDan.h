// CDan.h : main header file for the CDan DLL
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "CoboldDll.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef CDAN_EXPORTS
#define CDAN_API __declspec(dllexport)
#else
#define CDAN_API __declspec(dllimport)
#endif

// no file support during AnalysisInitialize
// no file support during AnalysisGetInformationString

// 
// I M P O R T A N T
//
// If you use CArchive or something equivalent 
// then flush first any data of the file before 
// you call any function other than reading from the
// file or flushing the data otherwhise you risk a
// misaligned file!

CDAN_API LPCTSTR AnalysisGetInformationString();
CDAN_API int AnalysisInitialize(CDoubleArray *pEventData, CDoubleArray *pParameter, CDoubleArray *pWeighParameter);
CDAN_API void AnalysisProcessEvent(CDoubleArray *pEventData, CDoubleArray *pParameter, CDoubleArray *pWeighParameter);
CDAN_API void AnalysisFinalize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter);

/////////////////////////////////////////////////////////////////////////////
// CCDanApp
// See CDan.cpp for the implementation of this class
//

class CCDanApp : public CWinApp
{
public:
	CCDanApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDanApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCDanApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern double _dOldEventTime;
extern double _dStartEventTime;

double GetDeltaEventTime(CDoubleArray *pEventData,CDoubleArray *pParameters);
double GetEventTime(CDoubleArray *pEventData,CDoubleArray *pParameters);
LARGE_INTEGER GetTimeInformation(CDoubleArray *pEventData,CDoubleArray *pParameters);
void CorrectEventDataPosistion(CDoubleArray *pEventData,int iIndexOffset, int iEndCounter);
double CorrectGP1NDL(double dChannel,double dCorrectionValue);


/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

