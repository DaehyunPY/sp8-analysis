// CoboldDll.h : main header file for the COBOLDDLL DLL
//

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCoboldDllApp
// See CoboldDll.cpp for the implementation of this class
//
#ifndef CCOBOLDDLLAPP_DEF
#define CCOBOLDDLLAPP_DEF
class CCoboldDllApp : public CWinApp
{
public:
	CCoboldDllApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCoboldDllApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCoboldDllApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#include "CheckProcess.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef COBOLDDLL_EXPORTS
	#define COBOLDDLL_API __declspec(dllexport)
#else
	#define COBOLDDLL_API __declspec(dllimport)
#endif

/////////////////////////////////////////////////////////////////////////////
// ATARI Time structure
typedef struct {	
	unsigned short 		hsec	: 5;
	unsigned short 		min		: 6;
	unsigned short 		hour	: 5;
		}ATime;

typedef struct {	
	unsigned short 		day		: 5;
	unsigned short 		month	: 4;
	unsigned short 		year	: 7;
		}ADate;

typedef struct {
	ATime 		time;	
	ADate 		date;
		}ADateTime;

// This class is exported from the CoboldSecurity.dll
//template<class double, class double> 
class COBOLDDLL_API CDoubleArray : public CObject
{

public:
	DECLARE_SERIAL(CDoubleArray)

// Construction
	CDoubleArray();
	CDoubleArray(int NumberOfElements);
	CDoubleArray(int NumberOfXElements, int NumberOfYElements);

// Attributes
	inline int GetSize() const {return m_nSize;}
	int GetUpperBound() const;
	void SetSize(int nNewSize, int nGrowBy = -1);
	void SetMatixSize(int nNewXSize, int nNewYSize, int nGrowBy = -1);

// Operations
	// Clean up
	void FreeExtra();
	void RemoveAll();

	// Accessing elements
	inline double GetAt(int nIndex) const 
		{ ASSERT(nIndex >= 0 && nIndex < m_nSize); return m_pData[nIndex];}
	inline void SetAt(int nIndex, double newElement) 
		{ ASSERT(nIndex >= 0 && nIndex < m_nSize); m_pData[nIndex] = newElement;}
	inline double& ElementAt(int nIndex) 
		{ASSERT(nIndex >=0 && nIndex< m_nSize); 
		return m_pData[nIndex];}
	void Empty();

	// Potentially growing the array
	void SetAtGrow(int nIndex, double newElement);
	int Add(double newElement) {int nIndex = m_nSize; SetAtGrow(nIndex,newElement); return nIndex;}

	// overloaded operator helpers
	double operator[](int nIndex) const;
	double& operator[](int nIndex);
	const CDoubleArray& operator = (const CDoubleArray& sData);

	// Operations that move elements around
	void InsertAt(int nIndex, double newElement, int nCount = 1);
	void RemoveAt(int nIndex, int nCount = 1);
	void InsertAt(int nStartIndex, CDoubleArray* pNewArray);

	// Matrix operation
	void SetAtMatrix(int posx, int posy,double Value);
	double GetAtMatrix(int posx, int posy);
	inline int GetSizeX() const {return SizeX;}
	inline int GetSizeY() const {return SizeY;}

	// Expand operation
	bool CDoubleArray::ExpandTo(int xchannels);
	bool CDoubleArray::ExpandTo(int xchannels, int ychannels);


protected:
	void AssignCopy(int nSrcLen, double *Data);

// Implementation
public:
	double* m_pData; // the actual array of data
	bool m_LoadError;
protected:
	int	m_VersionNumber;
	long m_nSize;     // # of elements (upperBound - 1)
	int m_nMaxSize;  // max allocated
	int m_nGrowBy;   // grow amount

	int SizeX;		// Size of Matrix in X
	int SizeY;		// Size of Matrix in Y

public:
	~CDoubleArray();
	virtual void Serialize(CArchive& ar);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif
};

COBOLDDLL_API int CheckDate(CString Module);

COBOLDDLL_API COLORREF GetTNGColor(CDC *pDC, int ColorIndex);
COBOLDDLL_API COLORREF GetTNGScreenColor(int ColorIndex);
COBOLDDLL_API COLORREF GetTNGPrintColor(int ColorIndex);
COBOLDDLL_API void GetTNGDefaultColors(void);
COBOLDDLL_API void SetTNGScreenColor(int ColorIndex, COLORREF Color);
COBOLDDLL_API void SetTNGPrintColor(int ColorIndex, COLORREF Color);
COBOLDDLL_API void ReadColorDefinitions(void);
COBOLDDLL_API void ReadDelimiterDefinitions(void *pvView);
COBOLDDLL_API void WriteColorDefinitions(void);
COBOLDDLL_API void WriteDelimiterDefinitions(void *pvView);

COBOLDDLL_API double GetCStringNumber(CString cAddress);

COBOLDDLL_API void PumpDlgMessages(CWnd *pWnd);
COBOLDDLL_API void PumpMessages();

COBOLDDLL_API void DoubleAtari(double register *var);
COBOLDDLL_API void LongAtari(unsigned long register *var);
COBOLDDLL_API void IntAtari(unsigned short register *var);

COBOLDDLL_API void ToAtariTime(CTime *pCTime, ADateTime *pATime);
COBOLDDLL_API void ToCTime(CTime *pCTime, ADateTime *pATime);

COBOLDDLL_API bool ReadWindowPlacement(LPWINDOWPLACEMENT pwp, UINT *puiFlags, char *pszSection, char *pszWindowFlag, char *pszWindowPos);
COBOLDDLL_API void WriteWindowPlacement(CWnd *pWnd, char *pszSection, char *pszWindowFlag, char *pszWindowPos);
COBOLDDLL_API bool TestFlatBarAvailable(void);

COBOLDDLL_API int GetTaskListNT(PTASK_LIST  pTask, DWORD dwNumTasks);
COBOLDDLL_API BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
COBOLDDLL_API int CountApplications(CString VersionFileDescription);

COBOLDDLL_API int FileNotFoundMessage(CStringArray *pActiveBatchFile, CString BatchName, bool bQuitMessage = false);

COBOLDDLL_API HRESULT GetComCtlVersion(LPDWORD pdwMajor, LPDWORD pdwMinor);

COBOLDDLL_API void DeleteLocalRegistryKey(HKEY myKey, char RegPath[MAX_PATH]);

COBOLDDLL_API void CorrectGP1DifferentialNonlinearity(CDoubleArray *pEventData, CDoubleArray *pParameters, int rawdata, int iRes);

COBOLDDLL_API CString &GetApplicationPath();

COBOLDDLL_API CString &GetSerialNumber();
COBOLDDLL_API bool IsSerialNumberOK();
COBOLDDLL_API bool IsSerialNumberHM1();
COBOLDDLL_API bool IsSerialNumberDemo();
COBOLDDLL_API bool IsDemoExpired();

COBOLDDLL_API void OrthogonalToRPhi(double x,double y,double &r,double &phi);

#ifdef __cplusplus
}
#endif

/////////////////////////////////////////////////////////////////////////////

#endif
