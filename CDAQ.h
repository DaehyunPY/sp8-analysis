// CDAQ.h : main header file for the CDAQ DLL
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
//#include "..\CoboldDll\CoboldDll.h"
//#include "..\HM1\HM1.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef CDAQ_EXPORTS
#define CDAQ_API __declspec(dllexport)
#else
#define CDAQ_API __declspec(dllimport)
#endif

#define INCLUDE_HARDWARE_SUPPORT

// definitions for main program event data format
#define LM_BYTE					1	//  8bit integer
#define LM_SHORT				2	// 16bit integer
#define LM_LONG					3	// 32bit integer
#define	LM_FLOAT				4   // 32bit IEEE float
#define LM_DOUBLE				5	// 64bit IEEE float
#define LM_CAMAC				6	// 24bit integer
#define LM_DOUBLELONG			7	// 64bit integer
#define LM_LASTKNOWNDATAFORMAT	LM_DOUBLELONG
#define LM_USERDEF				-1	// user will handle the reading 
									// of file and filling of EventData Array
									// except reading the main header file

CDAQ_API LPCTSTR DAQGetInformationString();

CDAQ_API int  DAQInitialize(CDoubleArray *pParameter, unsigned int *pDataFormat);
CDAQ_API bool DAQFinalize(CDoubleArray *pParameter);

CDAQ_API bool DAQOnlineInitialize(CArchive *pAr, CDoubleArray *pParameter, unsigned int *pLMUserHeaderLength = NULL);
CDAQ_API bool DAQOfflineInitialize(CArchive *pAr, CDoubleArray *pParameter, unsigned int LMUserHeaderLength);

CDAQ_API bool DAQOnlineFinalize(CArchive *pAr, CDoubleArray *pParameter);
CDAQ_API bool DAQOfflineFinalize(CArchive *pAr, CDoubleArray *pParameter);

CDAQ_API int DAQOnlineReadEvent(CArchive *pAr, CDoubleArray *pEventData, CDoubleArray *pParameter);
CDAQ_API int DAQOfflineReadEvent(CArchive *pAr, CDoubleArray *pEventData, CDoubleArray *pParameter);

/////////////////////////////////////////////////////////////////////////////
// CCDAQApp
// See CDAQ.cpp for the implementation of this class
//

class CCDAQApp : public CWinApp
{
public:
	CCDAQApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDAQApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCDAQApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
}
#endif

