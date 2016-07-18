// CDan.cpp : User-Stuff routines for the DLL.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "stdafx.h"
#include "CDan.h"
#include "DANUtilities.h"
#include "CDaq.h"
#include "LMInfo.h"
#include "UserSP.h"
#include "resort64.Import.h"

FILE *fp, *fp1;
//FILE *fpout;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// insert the subprogram part for software insert here
// ------> update dependencies <------

#define DAQ_VERSION7    20080507        // 07.05.2008
#define DAQ_VERSION6    20080901
#define DAQ_VERSION5    20020408
#define DAQ_VERSION4    20020408
#define LMF_VERSION5    5
#define LMF_VERSION6    6
#define LMF_VERSION7    7

#define DAN_VERSION6    20070901
#define DAN_VERSION5    20020513
#define DAN_SUBVERSION    0x0003        // increase with 1 (start with 0 at V6)


/////////////////////////////////////////////////////////////////////////////
// variables definitions

LARGE_INTEGER lipTimeStamp;    // TimeStamp time information
CTime ctpLMFStartTime;        // Start Time of LMF

int ipTimeStamp;            // parameter 2
double dpTimeScaling;        // parameter 5 - Time Scaling (ticks per second)

int iDAQ_ID;                // parameter 8

double dpTDCResolution;        // parameter 20
int ipTDCDataType;            // parameter 21

int ipNumberOfChannels;        // parameter 32
int ipNumberOfHits;            // parameter 33
int iDataFormat;            // parameter 40


int ipStartDAQData;            // parameter 105
int ipStartDANData;            // parameter 106

//errno_t error;

int nTDC;                            // number of TDC (TDC module*2)
const int nDetector = 2;
const int nhit = 4;
const int nTDCmax = 16, nTDChalf = 8;
const int iFlagCondition = 14, eFlagCondition = 14;

double offset[2];    // ION image offset
double f1[2], f2[2], f3[2];

int TotalCount, TrueCount;
int iTDC, ihit, i, iOutpara;


double dtime_sumx[2], dtime_sumy[2], dtime_sumz[2];
double dsumx_width[2], dsumy_width[2], dsumz_width[2];
double dInterval, dx0, dy0;

double dStartDanCoordinates;
int ipStartDAQTDCData;

double danoderuntime[2];
int iDo_Reconstructions;            // some flags
int iresort;
double distortx_ele[51][4], distorty_ele[51][4];
double distortx_ion[51][4], distorty_ion[51][4];

double eFalseStart1, eFalseStop1, eFalseStart2, eFalseStop2;

double EventCounter;                // EventCounter for the data in LM-file
inline int signofd(double a) { return (a == 0.0) ? 0 : (a < 0.0 ? -1 : 1); }
extern CUserSpectrum *pUserSP;





// *** Export ROOT File: Declare ***
#include <fstream>
#include <string>
#include <TFile.h>
#include <TTree.h>
bool isFileExist(const char *fileName) {
  std::ifstream file(fileName);
  return file.good();
}
std::string rootFilename = "";
TFile *pRootFile;
//TTree pRootTree("resortedData", "Resorted Data");
TTree *pRootTree;
struct DataSet {
  double x, y, t;
  int flag;
} elecDataSet[nhit], ionDataSet[nhit];





///////////////////////////////
// AnalysisGetInformationString
///////////////////////////////
// is called during startup procedure of CoboldPC
//
// return value is a string that will be displayed in
// the splash box and the about box

CString myInfoString;
CDAN_API LPCTSTR AnalysisGetInformationString() {
  CString DAQDebugInfo;
#ifdef _DEBUG
  DAQDebugInfo = "Debug ";
#endif
  myInfoString.Format("RoentDek Standard (%sVersion %08d - %04d - %04d)",
                      DAQDebugInfo,
                      DAN_VERSION6,
                      DAN_SUBVERSION,
                      LMF_VERSION7);

  return LPCTSTR(myInfoString);
}

/////////////////////
// AnalysisInitialize
/////////////////////
// is called when new command is executed
//
// return value is false -> error during initialize
//                 true  -> initialize ok

CDAN_API BOOL AnalysisInitialize(CDoubleArray &EventData,
                                 CDoubleArray &Parameters,
                                 CDoubleArray &WeighParameter) {





  // *** Export ROOT File: Initialize ***
  pRootTree = new TTree("resortedData", "Resorted Data");
  for (int i = 0; i < 10000; i++) {
    char ch[5];
    sprintf(ch, "%04d", i);
    rootFilename = "resortLess";
    rootFilename += ch;
    rootFilename += ".root";
    if (isFileExist(rootFilename.c_str())) {
      continue;
    }
    break;
  }
  pRootFile = new TFile(rootFilename.c_str(), "new");
//	pRootFile.Open("resortLess.root", "new");
  for (int i = 0; i < nhit; i++) {
    char ch[2];
    sprintf(ch, "%01d", i);
    std::string str;
    str = "Elec";
    pRootTree->Branch((str + "X" + ch).c_str(),
                      &elecDataSet[i].x,
                      (str + "X" + ch + "/D").c_str());
    pRootTree->Branch((str + "Y" + ch).c_str(),
                      &elecDataSet[i].y,
                      (str + "Y" + ch + "/D").c_str());
    pRootTree->Branch((str + "T" + ch).c_str(),
                      &elecDataSet[i].t,
                      (str + "T" + ch + "/D").c_str());
    pRootTree->Branch((str + "Flag" + ch).c_str(),
                      &elecDataSet[i].flag,
                      (str + "Flag" + ch + "/I").c_str());
    str = "Ion";
    pRootTree->Branch((str + "X" + ch).c_str(),
                      &ionDataSet[i].x,
                      (str + "X" + ch + "/D").c_str());
    pRootTree->Branch((str + "Y" + ch).c_str(),
                      &ionDataSet[i].y,
                      (str + "Y" + ch + "/D").c_str());
    pRootTree->Branch((str + "T" + ch).c_str(),
                      &ionDataSet[i].t,
                      (str + "T" + ch + "/D").c_str());
    pRootTree->Branch((str + "Flag" + ch).c_str(),
                      &ionDataSet[i].flag,
                      (str + "Flag" + ch + "/I").c_str());
  }


  if (!pUserSP) {
    pUserSP = (CUserSpectrum *) new CUserSpectrum();
  }

  _dOldEventTime = 0.0;
  _dStartEventTime = 0.0;

  // first check if all parameters are set
  if (Parameters.GetSize() < 140) {
    // indicate parameter number error
    int iAnswer = AfxMessageBox(
        "a Not all parameters are defined\n for this DAN module!\n\nYou may ignore this on your own risk!\n\nTo Ignore press YES",
        MB_ICONQUESTION | MB_YESNO);
    if (iAnswer == IDNO)
      return false;
  }

  // check DAQ-Version
  unsigned int _iDAQVersion = (unsigned int) (Parameters[5] + 0.1);
  if ((_iDAQVersion != DAQ_VERSION5) && (_iDAQVersion != DAQ_VERSION6)
      && (_iDAQVersion != DAQ_VERSION7)) {
    // indicate DAQ version error
    CString MsgText;
    MsgText.Format(
        "DAN module works only with list mode data\ntaken with Standard Daq-Modules version\n(%d and %d and %d)!",
        DAQ_VERSION5,
        DAQ_VERSION6,
        DAQ_VERSION7);
    MsgText +=
        "\n\nYou may ignore this on your own risk!\n\nTo Ignore press YES";
    int iAnswer = AfxMessageBox(MsgText, MB_ICONQUESTION | MB_YESNO);
    if (iAnswer == IDNO)
      return false;
  }

  // transfer parameters
  //    in the next lines you'll find the following correction values
  //    0.1 for float to int converion
  //    -1 if parameter is for indexing
  ipTimeStamp = (int) (Parameters[1]
      + 0.1);                                        // parameter 2
  dpTimeScaling =
      Parameters[4];                                                // parameter 5
  ctpLMFStartTime = CTime((time_t)((int) (Parameters[6]
      + 0.1)));                // parameter 7
  iDAQ_ID = (int) (Parameters[7]
      + 0.1);                                            // parameter 8

  dpTDCResolution =
      Parameters[19];                                            // parameter 20

  ipTDCDataType = (int) (Parameters[20]
      + 0.1);                                    // parameter 21

  ipNumberOfChannels = (int) (Parameters[31]
      + 0.1);                                // parameter 32
  ipNumberOfHits = (int) (Parameters[32]
      + 0.1);                                    // parameter 33
  iDataFormat =
      (int) (signofd(Parameters[39] + 0.1) < 0 ? Parameters[39] - 0.1 :
             Parameters[39] + 0.1);    // parameter 40

  ipStartDAQData = (int) (Parameters[104] + 0.1)
      - 1;                            // parameter 105
  ipStartDANData = (int) (Parameters[105] + 0.1)
      - 1;                            // parameter 106

  bool bEventCounterReset =
      (bool) ((int) (Parameters[139] + 0.1) ? true : false);// parameter 140


  nTDC = (int) (Parameters[150 - 1] + 0.1);// parameter 150

  offset[0] = Parameters[160 - 1];
  f1[0] = Parameters[161 - 1];
  f2[0] = Parameters[162 - 1];
  f3[0] = Parameters[163 - 1];
  offset[1] = Parameters[164 - 1];
  f1[1] = Parameters[165 - 1];
  f2[1] = Parameters[166 - 1];
  f3[1] = Parameters[167 - 1];

  dtime_sumx[0] = Parameters[170 - 1];
  dtime_sumy[0] = Parameters[171 - 1];
  dtime_sumz[0] = Parameters[172 - 1];
  dsumx_width[0] = Parameters[173 - 1];
  dsumy_width[0] = Parameters[174 - 1];
  dsumz_width[0] = Parameters[175 - 1];
  dtime_sumx[1] = Parameters[176 - 1];
  dtime_sumy[1] = Parameters[177 - 1];
  dtime_sumz[1] = Parameters[178 - 1];
  dsumx_width[1] = Parameters[179 - 1];
  dsumy_width[1] = Parameters[180 - 1];
  dsumz_width[1] = Parameters[181 - 1];
  dx0 = Parameters[182 - 1];
  dy0 = Parameters[183 - 1];

  dInterval = Parameters[190 - 1]; //bunch interval

  iresort = (int) (Parameters[191 - 1] + 0.1);
  danoderuntime[0] = Parameters[192 - 1];
  danoderuntime[1] = Parameters[193 - 1];
  eFalseStart1 = Parameters[200 - 1];
  eFalseStop1 = Parameters[201 - 1];
  eFalseStart2 = Parameters[202 - 1];
  eFalseStop2 = Parameters[203 - 1];

  //if( (error = fopen_s(&fp, "hi.dat", "wt")) != 0){
  //	AfxMessageBox("Error: 'hi.dat' could not be opened.", MB_ICONSTOP);
  //	return false;
  //}
//
//  fp = fopen("hi.dat", "wt");
//  fprintf(fp, "This is the directory");
//  fclose(fp);

  double dis = 0;
  char c;
//	if( (error = fopen_s(&fp, "time_sum.dat", "rt")) != 0){
//		AfxMessageBox("Error: 'time_sum.dat' could not be opened.", MB_ICONSTOP);
//		return false;
//	}
//	if( (error = fopen_s(&fp1, "time_sum1.dat", "wt")) != 0){
//		AfxMessageBox("Error: 'time_sum1.dat' could not be opened.", MB_ICONSTOP);
//		return false;
//	}

  fp = fopen("time_sum.dat", "rt");
  fp1 = fopen("time_sum.log", "wt");
  for (i = 1; i < 4; i++) {
    fscanf(fp, "%s\n", &c);
    fprintf(fp1, "%s\n", &c);
    for (iTDC = 1; dis < 210; iTDC++) {
      fscanf(fp, "%lf, %lf\n", &distortx_ele[iTDC][i], &distorty_ele[iTDC][i]);
      fprintf(fp1,
              "%d, %lf, %lf\n",
              iTDC,
              distortx_ele[iTDC][i],
              distorty_ele[iTDC][i]);
      dis = distortx_ele[iTDC][i];
    }
    fprintf(fp1, "\n");
    dis = 0;
  }
  for (i = 1; i < 4; i++) {
    for (iTDC = 1; iTDC < 51; iTDC++) {
      distorty_ele[iTDC][i] = -distorty_ele[iTDC][i];
    }
  }
  for (i = 1; i < 4; i++) {
    fscanf(fp, "%s\n", &c);
    fprintf(fp1, "%s\n", &c);
    for (iTDC = 1; dis < 200; iTDC++) {
      fscanf(fp, "%lf, %lf\n", &distortx_ion[iTDC][i], &distorty_ion[iTDC][i]);
      fprintf(fp1,
              "%d, %lf, %lf\n",
              iTDC,
              distortx_ion[iTDC][i],
              distorty_ion[iTDC][i]);
      dis = distortx_ion[iTDC][i];
    }
    fprintf(fp1, "\n");
    dis = 0;
  }
  for (i = 1; i < 4; i++) {
    for (iTDC = 1; iTDC < 51; iTDC++) {
      distorty_ion[iTDC][i] = -distorty_ion[iTDC][i];
    }
  }
  fclose(fp);
  fclose(fp1);

  //	fpout = fopen("resort.lmf","ab");
//	if( (error = fopen_s(&fpout, "resortLess.lmf", "ab+")) != 0){
//		AfxMessageBox("Error: 'resortLess.lmf' could not be opened.", MB_ICONSTOP);
//		return false;
//	}
//  fpout = fopen("resortLess.lmf", "ab");
  // check parameter 20 for valid data
  if (Parameters[19] <= 0.0) {
    // indicate TDC resolution error
    int iAnswer = AfxMessageBox(
        "Parameter 20\nTDC-resolution\ncontains no valid data!\n\nValues should be >= 0.0\n\nYou may ignore this on your own risk!\n\nTo Ignore press YES",
        MB_ICONQUESTION | MB_YESNO);
    if (iAnswer == IDNO)
      return false;
  }

  // check parameter 21 for valid data
  if ((int) (Parameters[20]) < 0 || (int) (Parameters[20]) > 2) {
    // indicate TDC-data type error
    int iAnswer = AfxMessageBox(
        "Parameter 21\nTDC-data type\ncontains no valid data!\n\nValues should be 0,1 or 2\n\nYou may ignore this on your own risk!\n\nTo Ignore press YES",
        MB_ICONQUESTION | MB_YESNO);
    if (iAnswer == IDNO)
      return false;
  }

  // set EventCounter to zero at start of new command
  if (bEventCounterReset) {
    EventCounter = 0;
    Parameters[139] = false;
  }

  // correct data for start of dan coordinates
  // first the start of DAQData
  ipStartDAQData = ipStartDAQTDCData = 0;
  Parameters[104] =
      ipStartDAQTDCData + 1;    // write information back to parameter 105

  // second the start of DANData
  if (ipStartDANData < 0) {
    if (iDAQ_ID
        == DAQ_ID_HM1_ABM) // correct ABM start DAN data for standard DAN
      ipStartDANData =
          ipStartDAQTDCData + ((ipNumberOfChannels + 1) * (ipNumberOfHits + 1));
    else if (iDAQ_ID == DAQ_ID_2HM1) {
      int ip2NumberOfChannels = (int) (Parameters[81]
          + 0.1);                                // parameter 82
      int ip2NumberOfHits = (int) (Parameters[82]
          + 0.1);                                    // parameter 83

      ipStartDANData =
          ipStartDAQTDCData + (ipNumberOfChannels * (ipNumberOfHits + 1));
      ipStartDANData += (ip2NumberOfChannels * (ip2NumberOfHits + 1));
    }
    else if (iDAQ_ID == DAQ_ID_2TDC8) {
      int ip2NumberOfChannels = (int) (Parameters[33]
          + 0.1);                                // parameter 34
      int ip2NumberOfHits = (int) (Parameters[34]
          + 0.1);                                    // parameter 35

      ipStartDANData =
          ipStartDAQTDCData + (ipNumberOfChannels * (ipNumberOfHits + 1));
      ipStartDANData += (ip2NumberOfChannels * (ip2NumberOfHits + 1));
    }
    else {
      if (iDAQ_ID == DAQ_ID_CAMAC)
        ipStartDANData =
            ipStartDAQTDCData + (ipNumberOfChannels * (ipNumberOfHits));
      else
        ipStartDANData =
            ipStartDAQTDCData + (ipNumberOfChannels * (ipNumberOfHits + 1));
    }

    Parameters[105] = double(ipStartDANData)
        + 1;            // write information back to parameter 106
  }

  // initialize the random generator
  srand((unsigned) time(NULL));

  // test for sufficient # of coordinates defined
  //	if((EventData.GetSize() <= ipStartDANData+48))
  if ((EventData.GetSize() <= ipStartDANData)) {
    // indicate TDC-data type error
    int iAnswer = AfxMessageBox(
        "aa Not all coordinates are defined\nfor this DAN module!\n\n\nYou may ignore this on your own risk!\n\nTo Ignore press YES",
        MB_ICONQUESTION | MB_YESNO);
    if (iAnswer == IDNO)
      return false;
  }

  return true;
}

///////////////////////
// AnalysisProcessEvent
///////////////////////
// is called when new command is executed
//CDAQ_API LARGE_INTEGER DAQTimeStamp;
//__declspec(dllimport) int DAQTimeStamp;
__declspec(dllimport) int DAQTimeStamp;

CDAN_API int AnalysisProcessEvent(CDoubleArray &EventData,
                                  CDoubleArray &Parameters,
                                  CDoubleArray &WeighParameter,
                                  LMFPreEventData &preEventData) {
  double AbsoluteEventTime;            // ns since start
  double DeltaEventTime;                // This Time - PreviousTime
  double True = true;                    // always true
  double False = false;                // always false
  int ConsistencyIndicator = -1000;
  int PLLStatusLocked;                // totally locked then true else false

  int n11, n12, n13, n14, n15, n16, n17, n18;
  int CLK1;
  int n21, n22, n23, n24, n25, n26, n27, n28;
  int CLK2;

  int TDCData[16][7];

  double Time_ns[16][7];
  double dTx[2][6], dTy[2][6], dTz[2][6];
  double SumTx[2][6], SumTy[2][6], SumTz[2][6];
  double X[2][6], Y[2][6];
  int nCount[16], iHitsTDC[2], bitpattern[2][4], j;
  double dtParticle[2][6], dtParticle2[2][6], dOrigin[2], dOrigin2[2],
      dDistance;
  double Xuv[2][6], Yuv[2][6];

  bool ionsAreInCondition = true;
  bool electronsAreInCondition = true;
  TotalCount++;


  // Sample of how to use spectrum commands
  //	pUserSP->AddOneAt(2,20);			// add one in spectrum 3 at channel 20
  //	pUserSP->AddValueAt(3,20,0.5);		// add 0.5 in spectrum 4 at channel 20

  // get time information if present
  if (ipTimeStamp) {
    // AbsoluteEventTime
    AbsoluteEventTime = GetEventTime(preEventData, Parameters);
    // AbsoluteDeltaEventTime
    DeltaEventTime = GetDeltaEventTime(preEventData, Parameters);
    // EventCounter
  }

  // EventCounter
  bool bEventCounterReset = (bool) ((int) (Parameters[139] + 0.1) ? true
                                                                  : false);            // parameter 140
  if (bEventCounterReset) {
    EventCounter = 0;
    Parameters[139] = false;
  }
  EventCounter += 1;

  // force 4hits
  for (size_t i = 0; i < 18; i++) {
    if (EventData[(ipStartDAQTDCData + i * (ipNumberOfHits + 1))] > 4)
      EventData[(ipStartDAQTDCData + i * (ipNumberOfHits + 1))] = 4;
  }

  // Get Status Information
  {

    n11 =
        (int) (EventData[(ipStartDAQTDCData + 0 * (ipNumberOfHits + 1))] + 0.1);
    n12 =
        (int) (EventData[(ipStartDAQTDCData + 1 * (ipNumberOfHits + 1))] + 0.1);
    n13 =
        (int) (EventData[(ipStartDAQTDCData + 2 * (ipNumberOfHits + 1))] + 0.1);
    n14 =
        (int) (EventData[(ipStartDAQTDCData + 3 * (ipNumberOfHits + 1))] + 0.1);
    n15 =
        (int) (EventData[(ipStartDAQTDCData + 4 * (ipNumberOfHits + 1))] + 0.1);
    n16 =
        (int) (EventData[(ipStartDAQTDCData + 5 * (ipNumberOfHits + 1))] + 0.1);
    n17 =
        (int) (EventData[(ipStartDAQTDCData + 6 * (ipNumberOfHits + 1))] + 0.1);
    n18 =
        (int) (EventData[(ipStartDAQTDCData + 7 * (ipNumberOfHits + 1))] + 0.1);
    CLK1 =
        (int) (EventData[(ipStartDAQTDCData + 8 * (ipNumberOfHits + 1))] + 0.1);

    n21 =
        (int) (EventData[(ipStartDAQTDCData + 9 * (ipNumberOfHits + 1))] + 0.1);
    n22 = (int) (EventData[(ipStartDAQTDCData + 10 * (ipNumberOfHits + 1))]
        + 0.1);
    n23 = (int) (EventData[(ipStartDAQTDCData + 11 * (ipNumberOfHits + 1))]
        + 0.1);
    n24 = (int) (EventData[(ipStartDAQTDCData + 12 * (ipNumberOfHits + 1))]
        + 0.1);
    n25 = (int) (EventData[(ipStartDAQTDCData + 13 * (ipNumberOfHits + 1))]
        + 0.1);
    n26 = (int) (EventData[(ipStartDAQTDCData + 14 * (ipNumberOfHits + 1))]
        + 0.1);
    n27 = (int) (EventData[(ipStartDAQTDCData + 15 * (ipNumberOfHits + 1))]
        + 0.1);
    n28 = (int) (EventData[(ipStartDAQTDCData + 16 * (ipNumberOfHits + 1))]
        + 0.1);
    CLK2 = (int) (EventData[(ipStartDAQTDCData + 17 * (ipNumberOfHits + 1))]
        + 0.1);


    PLLStatusLocked = false;
  }

  int i_check;
  i_check = 0;
  i_check = n11 + n12 + n13 + n14 + n15 + n16 + n17;
  if (i_check < 1) goto hell_2;

  i_check = 0;
  i_check = n21 + n22 + n23 + n24 + n25 + n26 + n27;
  if (i_check < 1) goto hell_2;

  // Get Raw-Data



  /////////////////////////////////////
  // write all data back to coordinates
  /////////////////////////////////////
  EventData[ipStartDANData + 0] = AbsoluteEventTime;
  EventData[ipStartDANData + 1] = DeltaEventTime;
  EventData[ipStartDANData + 2] = EventCounter;
  EventData[ipStartDANData + 3] = True;
  EventData[ipStartDANData + 4] = ConsistencyIndicator;
  EventData[ipStartDANData + 5] = PLLStatusLocked;

  EventData[ipStartDANData + 6] = n11;
  EventData[ipStartDANData + 7] = n12;
  EventData[ipStartDANData + 8] = n13;
  EventData[ipStartDANData + 9] = n14;
  EventData[ipStartDANData + 10] = n15;
  EventData[ipStartDANData + 11] = n16;
  EventData[ipStartDANData + 12] = n17;
  EventData[ipStartDANData + 13] = n18;
  EventData[ipStartDANData + 14] = CLK1;

  EventData[ipStartDANData + 15] = n21;
  EventData[ipStartDANData + 16] = n22;
  EventData[ipStartDANData + 17] = n23;
  EventData[ipStartDANData + 18] = n24;
  EventData[ipStartDANData + 19] = n25;
  EventData[ipStartDANData + 20] = n26;
  EventData[ipStartDANData + 21] = n27;
  EventData[ipStartDANData + 22] = n28;
  EventData[ipStartDANData + 23] = CLK2;







  ////////// start calc ///////////

  // get the data to a local array
  // devide 24bit data into upper 12bit and lower 12bit part

  for (ihit = 0; ihit < nhit; ihit++) {

    TDCData[0][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 0 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[1][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 1 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[2][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 2 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[3][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 3 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[4][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 4 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[5][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 5 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[6][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 6 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[7][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 7 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);

    TDCData[8][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 9 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[9][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 10 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[10][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 11 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[11][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 12 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[12][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 13 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[13][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 14 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[14][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 15 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);
    TDCData[15][ihit] = (int) (
        EventData[(ipStartDAQTDCData + 16 * (ipNumberOfHits + 1) + ihit + 1)]
            + 0.1);

  }
  //	}


  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


  for (iTDC = 0; iTDC < nTDCmax; iTDC++) {
    nCount[iTDC] = 0;
    for (ihit = 0; ihit < nhit; ihit++) {
      Time_ns[iTDC][ihit] = -100000;
    }
  }

  int tmp_nhit[16];
  tmp_nhit[0] = n11;
  tmp_nhit[1] = n12;
  tmp_nhit[2] = n13;
  tmp_nhit[3] = n14;
  tmp_nhit[4] = n15;
  tmp_nhit[5] = n16;
  tmp_nhit[6] = n17;
  tmp_nhit[7] = n18;
  tmp_nhit[8] = n21;
  tmp_nhit[9] = n22;
  tmp_nhit[10] = n23;
  tmp_nhit[11] = n24;
  tmp_nhit[12] = n25;
  tmp_nhit[13] = n26;
  tmp_nhit[14] = n27;
  tmp_nhit[15] = n28;


  for (iTDC = 0; iTDC < nTDC; iTDC++) {
    for (ihit = 0; ihit < tmp_nhit[iTDC]; ihit++) {
      Time_ns[iTDC][ihit] = (double) ((double) (TDCData[iTDC][ihit]) * 0.025);
      if (Time_ns[iTDC][ihit] > -10000)
        nCount[iTDC] += 1;
    }
  }


  /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  for (i = 0; i < nDetector; i++) {
    iHitsTDC[i] = nhit;
    for (iTDC = 0; iTDC < nTDChalf - 1; iTDC++) {
      if (iHitsTDC[i] > nCount[iTDC + i * nTDChalf])
        iHitsTDC[i] = nCount[iTDC + i * nTDChalf];
    }
    for (ihit = 0; ihit < nhit; ihit++) {
      bitpattern[i][ihit] = 0;
      int jn = 1;
      for (j = 0; j < 6; j++) {
        if (nCount[j + i * nTDChalf] > ihit)
          bitpattern[i][ihit] = bitpattern[i][ihit] + jn;
        jn = jn * 2;
      }
      dTx[i][ihit] = -10000000;
      dTy[i][ihit] = -10000000;
      dTz[i][ihit] = -10000000;
      SumTx[i][ihit] = -10000000;
      SumTy[i][ihit] = -10000000;
      SumTz[i][ihit] = -10000000;
      X[i][ihit] = -10000000;
      Y[i][ihit] = -10000000;
      dtParticle[i][ihit] = -10000000;
      dtParticle2[i][ihit] = -10000000;
    }
  }

  TrueCount++;


  for (i = 0; i < nDetector; i++) {
    dOrigin[i] = Time_ns[7][0] + dInterval;
    //  TOF of particle
    for (ihit = 0; ihit < nhit; ihit++) {
      dtParticle[i][ihit] = (Time_ns[6 + i * nTDChalf][ihit] - dOrigin[i]);
    }
  }

  double electronMarker;
  electronMarker = Time_ns[6][0] - Time_ns[15][0];
  /////////////////////////////////// RESORT ////////////////////////////////////////
  double a[8][8];                // for electron
  double b[8][8];                // for ion
  int n_ele[8], n_ion[8], n;

  for (n = 0; n < 8; n++) {
    for (int m = 0; m < 8; m++) {
      a[n][m] = -1000.0;
      b[n][m] = -2000.0;
    }
    n_ele[n] = nCount[n];
    n_ion[n] = nCount[n + nTDChalf];
  }
  for (iTDC = 0; iTDC < nTDChalf; iTDC++) {
    for (ihit = 0; ihit < nhit; ihit++) {
      a[iTDC][ihit] = Time_ns[iTDC][ihit];        // x1 (ns)
      b[iTDC][ihit] = Time_ns[iTDC + nTDChalf][ihit];        // x1 (ns)
    }
  }

  double tdc[NUMTDC + 1][NUMION + 1];
  double diameter, f[4];   //   f[4]  not  f[6]
  int ions = 0, ionh = 0, eleh = 0, iMaxHits = 4;
  int flags[NUMION + 1], flagsi[NUMION + 1], cnt[NUMTDC + 1];
  double runtime, offset0;
  double xr[NUMION + 1], yr[NUMION + 1], sigts[4],
      tsum[4];    // sigts[4]   not  sigts[6]
  double xri[NUMION + 1], yri[NUMION + 1];    // sigts[4]   not  sigts[6]
  int iIon;
  double deadmcp, deadano;
  int i;
  double Xx = -1000, Yx = -1000, Xy = -1000, Yy = -1000, Xz = -1000, Yz = -1000,
      Xxs = -1000, Yxs = -1000, Xys = -1000, Yys = -1000;
  double Xx1 = -1000, Yx1 = -1000, Xy1 = -1000, Yy1 = -1000, Xz1 = -1000,
      Yz1 = -1000;
  double Xx2 = -1000, Yx2 = -1000, Xy2 = -1000, Yy2 = -1000, Xz2 = -1000,
      Yz2 = -1000;
  double Xxi = -1000, Yxi = -1000, Xyi = -1000, Yyi = -1000, Xzi = -1000,
      Yzi = -1000, Xxsi = -1000, Yxsi = -1000, Xysi = -1000, Yysi = -1000;
  double Xxi1 = -1000, Yxi1 = -1000, Xyi1 = -1000, Yyi1 = -1000, Xzi1 = -1000,
      Yzi1 = -1000;
  double Xxi2 = -1000, Yxi2 = -1000, Xyi2 = -1000, Yyi2 = -1000, Xzi2 = -1000,
      Yzi2 = -1000;
  double corr_sum_ele[4], corr_sum_ion[4];

  ///////////////// Electron ////////////////////////
  for (n = 0; n < NUMION + 1; n++) {
    for (int m = 0; m < NUMTDC + 1; m++) {
      tdc[m][n] = -m * 1000 - n * 10000;
    }
    cnt[n] = 0;
    xr[n] = -10000;
    yr[n] = -10000;
    flags[n] = -1;
  }

  deadmcp = 15.;
  deadano = 15.;

  // Hexanode: if you dont want to use position-dependent correction of the sums:
  //	for (i=1;i<=3;++i) {
  //		for (k=1;k<=4;++k) {
  //			distortx_ele[k][i]=0.;
  //			distorty_ele[k][i]=999999.;
  //		}
  //	}

  tsum[1] = dtime_sumx[0];
  tsum[2] = dtime_sumy[0];
  tsum[3] = dtime_sumz[0];
  sigts[1] = dsumx_width[0];
  sigts[2] = dsumy_width[0];
  sigts[3] = dsumz_width[0];
  f[1] = f1[0];
  f[2] = f2[0];
  f[3] = f3[0];
  offset0 = offset[0];
  runtime = danoderuntime[0];

  if (bitpattern[0][0] > 62) {
    Yx = a[0][0] + a[1][0] - 2 * a[6][0];
    Xx = a[1][0] - a[0][0];
    Yy = a[2][0] + a[3][0] - 2 * a[6][0];
    Xy = a[3][0] - a[2][0];
    Yz = a[4][0] + a[5][0] - 2 * a[6][0];
    Xz = a[5][0] - a[4][0];
  }

  if (iresort == 1) {

    for (iIon = 0; iIon < n_ele[0]; iIon++)
      tdc[1][iIon + 1] = a[0][iIon] - tsum[1] / 2.;
    for (iIon = 0; iIon < n_ele[1]; iIon++)
      tdc[2][iIon + 1] = a[1][iIon] - tsum[1] / 2.;
    for (iIon = 0; iIon < n_ele[2]; iIon++)
      tdc[3][iIon + 1] = a[2][iIon] - tsum[2] / 2.;
    for (iIon = 0; iIon < n_ele[3]; iIon++)
      tdc[4][iIon + 1] = a[3][iIon] - tsum[2] / 2.;
    for (iIon = 0; iIon < n_ele[4]; iIon++)
      tdc[5][iIon + 1] = a[4][iIon] - tsum[3] / 2. - offset0 / 2;
    for (iIon = 0; iIon < n_ele[5]; iIon++)
      tdc[6][iIon + 1] = a[5][iIon] - tsum[3] / 2. + offset0 / 2;
    for (iIon = 0; iIon < n_ele[6]; iIon++) tdc[7][iIon + 1] = a[6][iIon];

    if (bitpattern[0][0] > 62) {
      Yx1 = tdc[1][1] + tdc[2][1] - 2 * tdc[7][1];
      Xx1 = tdc[2][1] - tdc[1][1];
      Yy1 = tdc[3][1] + tdc[4][1] - 2 * tdc[7][1];
      Xy1 = tdc[4][1] - tdc[3][1];
      Yz1 = tdc[5][1] + tdc[6][1] - 2 * tdc[7][1];
      Xz1 = tdc[6][1] - tdc[5][1];
    }


    for (iTDC = 0; iTDC < nTDChalf - 1;
         iTDC++)    // copy array with number of hits per channel to array for resort
    {
      cnt[iTDC + 1] = n_ele[iTDC];
    }

    diameter =
        130.;    // diameter of mcp in mm       better choose 90 rather than 80



    // NEW ROUTINE:
    if ((electronMarker < eFalseStart1)
        || ((electronMarker > eFalseStop1) && (electronMarker < eFalseStart2))
        || (electronMarker > eFalseStop2))
      eleh = resort6(tdc,
                     cnt,
                     sigts,
                     f,
                     xr,
                     yr,
                     flags,
                     (diameter / 2.),
                     runtime,
                     deadmcp,
                     deadano,
                     1,
                     2,
                     3,
                     4,
                     5,
                     6,
                     7,
                     distortx_ele,
                     distorty_ele,
                     corr_sum_ele);

    //reverse xr yr
    if (eleh > 1) {
      double tempposition;
      for (iIon = 0; iIon < eleh / 2; iIon++) {
        tempposition = xr[iIon + 1];
        xr[iIon + 1] = xr[eleh - iIon];
        xr[eleh - iIon] = tempposition;
        tempposition = yr[iIon + 1];
        yr[iIon + 1] = yr[eleh - iIon];
        yr[eleh - iIon] = tempposition;
      }
    }


    if (eleh > 4) eleh = 4;
    if (eleh < 0) {
      //			printf("Error in hex-routine (electron)\n");
      goto hell_2;
    }
    if (eleh > 0) {
      Yx2 = tdc[1][1] + tdc[2][1] - 2 * tdc[7][1];
      Xx2 = tdc[2][1] - tdc[1][1];
      Yy2 = tdc[3][1] + tdc[4][1] - 2 * tdc[7][1];
      Xy2 = tdc[4][1] - tdc[3][1];
      Yz2 = tdc[5][1] + tdc[6][1] - 2 * tdc[7][1];
      Xz2 = tdc[6][1] - tdc[5][1];
    }



    ////		fprintf(fp,"%d, %d, %d, %d \n", ions, cnt[1], cnt[2], cnt[3], cnt[4]);
    //// loop counter has been changed from ions to (ions+1)   020708
    for (iIon = eleh + 1; iIon < nhit; iIon++) {
      tdc[1][iIon] = -10000. - 2000. * iIon;
      tdc[2][iIon] = -15000. - 2000. * iIon;
      tdc[3][iIon] = -20000. - 2000. * iIon;
      tdc[4][iIon] = -25000. - 2000. * iIon;
      tdc[5][iIon] = -30000. - 2000. * iIon;
      tdc[6][iIon] = -35000. - 2000. * iIon;
      tdc[7][iIon] = -40000. - 2000. * iIon;
    }
/*
		for(iIon=0;iIon<nhit; iIon++)
		{
			a[0][iIon] = (tdc[1][iIon+1]-tsum[1]/2.);
			a[1][iIon] = (tdc[2][iIon+1]-tsum[1]/2.);
			a[2][iIon] = (tdc[3][iIon+1]-tsum[2]/2.);
			a[3][iIon] = (tdc[4][iIon+1]-tsum[2]/2.);
			a[4][iIon] = (tdc[5][iIon+1]-tsum[3]/2.);
			a[5][iIon] = (tdc[6][iIon+1]-tsum[3]/2.);
			//			a[4][iIon] = (tdc[5][iIon+1]-tsum[3]/2. + offset0);
			//			a[5][iIon] = (tdc[6][iIon+1]-tsum[3]/2. - offset0);
			a[6][iIon] =  tdc[7][iIon+1];
		}
*/
    for (iIon = 0; iIon < eleh; iIon++) {
      a[0][iIon] = (tdc[1][eleh - iIon] - tsum[1] / 2.);
      a[1][iIon] = (tdc[2][eleh - iIon] - tsum[1] / 2.);
      a[2][iIon] = (tdc[3][eleh - iIon] - tsum[2] / 2.);
      a[3][iIon] = (tdc[4][eleh - iIon] - tsum[2] / 2.);
      a[4][iIon] = (tdc[5][eleh - iIon] - tsum[3] / 2.);
      a[5][iIon] = (tdc[6][eleh - iIon] - tsum[3] / 2.);
      a[6][iIon] = tdc[7][eleh - iIon];
    }
    for (iIon = eleh; iIon < nhit; iIon++) {
      a[0][iIon] = (tdc[1][iIon + 1] - tsum[1] / 2.);
      a[1][iIon] = (tdc[2][iIon + 1] - tsum[1] / 2.);
      a[2][iIon] = (tdc[3][iIon + 1] - tsum[2] / 2.);
      a[3][iIon] = (tdc[4][iIon + 1] - tsum[2] / 2.);
      a[4][iIon] = (tdc[5][iIon + 1] - tsum[3] / 2.);
      a[5][iIon] = (tdc[6][iIon + 1] - tsum[3] / 2.);
      a[6][iIon] = tdc[7][iIon + 1];
    }

    for (iTDC = 0; iTDC < nTDChalf - 1; iTDC++) n_ele[iTDC] = cnt[iTDC + 1];
  }  // if (iresort == 1)


  // flags correction 2015/02/10
  for (n = 0; n < nhit; n++) {
    if (flags[n + 1] > iFlagCondition) {
      a[6][n] = 16000;///2015/10/03
      ionsAreInCondition = false;
      ///goto hell_2;
    }
  }


  // End of Electron-part  (hexagonal Detector)


  ///////////////// Ion ////////////////////////

  for (n = 0; n < NUMION + 1; n++) {
    for (int m = 0; m < NUMTDC + 1; m++) {
      tdc[m][n] = -1000;
    }
    cnt[n] = 0;
    xri[n] = -10000;
    yri[n] = -10000;
    flagsi[n] = -1;
  }

  deadmcp = 15.;
  deadano = 15.;
  /*
	// Hexanode: if you dont want to use position-dependent correction of the sums:
	for (i=1;i<=3;++i) {
	for (int k=1;k<=4;++k) {
	distortx_ion[k][i]=0.;
	distorty_ion[k][i]=999999.;
	}
	}
	*/
  tsum[1] = dtime_sumx[1];
  tsum[2] = dtime_sumy[1];
  tsum[3] = dtime_sumz[1];
  sigts[1] = dsumx_width[1];
  sigts[2] = dsumy_width[1];
  sigts[3] = dsumz_width[1];
  f[1] = f1[1];
  f[2] = f2[1];
  f[3] = f3[1];
  offset0 = offset[1];
  runtime = danoderuntime[1];

  if (bitpattern[1][0] > 62) {
    Yxi = b[0][0] + b[1][0] - 2 * b[6][0];
    Xxi = b[1][0] - b[0][0];
    Yyi = b[2][0] + b[3][0] - 2 * b[6][0];
    Xyi = b[3][0] - b[2][0];
    Yzi = b[4][0] + b[5][0] - 2 * b[6][0];
    Xzi = b[5][0] - b[4][0];
  }


  if (iresort == 1) {
    for (iIon = 0; iIon < n_ion[0]; iIon++)
      tdc[1][iIon + 1] = b[0][iIon] - tsum[1] / 2.;
    for (iIon = 0; iIon < n_ion[1]; iIon++)
      tdc[2][iIon + 1] = b[1][iIon] - tsum[1] / 2.;
    for (iIon = 0; iIon < n_ion[2]; iIon++)
      tdc[3][iIon + 1] = b[2][iIon] - tsum[2] / 2.;
    for (iIon = 0; iIon < n_ion[3]; iIon++)
      tdc[4][iIon + 1] = b[3][iIon] - tsum[2] / 2.;
    for (iIon = 0; iIon < n_ion[4]; iIon++)
      tdc[5][iIon + 1] = b[4][iIon] - tsum[3] / 2. - offset0 / 2;
    for (iIon = 0; iIon < n_ion[5]; iIon++)
      tdc[6][iIon + 1] = b[5][iIon] - tsum[3] / 2. + offset0 / 2;
    for (iIon = 0; iIon < n_ion[6]; iIon++) tdc[7][iIon + 1] = b[6][iIon];

    if (bitpattern[1][0] > 62) {
      Yxi1 = tdc[1][1] + tdc[2][1] - 2 * tdc[7][1];
      Xxi1 = tdc[2][1] - tdc[1][1];
      Yyi1 = tdc[3][1] + tdc[4][1] - 2 * tdc[7][1];
      Xyi1 = tdc[4][1] - tdc[3][1];
      Yzi1 = tdc[5][1] + tdc[6][1] - 2 * tdc[7][1];
      Xzi1 = tdc[6][1] - tdc[5][1];
    }


    for (iTDC = 0; iTDC < nTDChalf - 1;
         iTDC++)    // copy array with number of hits per channel to array for resort
    {
      cnt[iTDC + 1] = n_ion[iTDC];
    }

    diameter =
        90.;    // diameter of mcp in mm       better choose 90 rather than 80




    // NEW ROUTINE:
    if ((electronMarker < eFalseStart1)
        || ((electronMarker > eFalseStop1) && (electronMarker < eFalseStart2))
        || (electronMarker > eFalseStop2))
      ionh = resort6(tdc,
                     cnt,
                     sigts,
                     f,
                     xri,
                     yri,
                     flagsi,
                     (diameter / 2.),
                     runtime,
                     deadmcp,
                     deadano,
                     1,
                     2,
                     3,
                     4,
                     5,
                     6,
                     7,
                     distortx_ion,
                     distorty_ion,
                     corr_sum_ion);

    //reverse xri yri
    if (ionh > 1) {
      double tempposition;
      for (iIon = 0; iIon < ionh / 2; iIon++) {
        tempposition = xri[iIon + 1];
        xri[iIon + 1] = xri[ionh - iIon];
        xri[ionh - iIon] = tempposition;
        tempposition = yri[iIon + 1];
        yri[iIon + 1] = yri[ionh - iIon];
        yri[ionh - iIon] = tempposition;
      }
    }

    if (ionh > 4) ionh = 4;
    if (ionh < 0) {
      //			printf("Error in hex-routine (electron)\n");
      goto hell_2;
    }
    if (ionh > 0) {
      Yxi2 = tdc[1][1] + tdc[2][1] - 2 * tdc[7][1];
      Xxi2 = tdc[2][1] - tdc[1][1];
      Yyi2 = tdc[3][1] + tdc[4][1] - 2 * tdc[7][1];
      Xyi2 = tdc[4][1] - tdc[3][1];
      Yzi2 = tdc[5][1] + tdc[6][1] - 2 * tdc[7][1];
      Xzi2 = tdc[6][1] - tdc[5][1];
    }


    ////		fprintf(fp,"%d, %d, %d, %d \n", ions, cnt[1], cnt[2], cnt[3], cnt[4]);
    //// loop counter has been changed from ions to (ions+1)   020708
    for (iIon = ionh + 1; iIon < nhit; iIon++) {
      tdc[1][iIon] = -10000. - 2000. * iIon;
      tdc[2][iIon] = -15000. - 2000. * iIon;
      tdc[3][iIon] = -20000. - 2000. * iIon;
      tdc[4][iIon] = -25000. - 2000. * iIon;
      tdc[5][iIon] = -30000. - 2000. * iIon;
      tdc[6][iIon] = -35000. - 2000. * iIon;
      tdc[7][iIon] = -40000. - 2000. * iIon;
    }
/*
		for(iIon=0;iIon<nhit; iIon++)
		{
			b[0][iIon] = (tdc[1][iIon+1]-tsum[1]/2.);
			b[1][iIon] = (tdc[2][iIon+1]-tsum[1]/2.);
			b[2][iIon] = (tdc[3][iIon+1]-tsum[2]/2.);
			b[3][iIon] = (tdc[4][iIon+1]-tsum[2]/2.);
			b[4][iIon] = (tdc[5][iIon+1]-tsum[3]/2.);
			b[5][iIon] = (tdc[6][iIon+1]-tsum[3]/2.);
			//			b[4][iIon] = (tdc[5][iIon+1]-tsum[3]/2. + offset0);
			//			b[5][iIon] = (tdc[6][iIon+1]-tsum[3]/2. - offset0);
			b[6][iIon] =  tdc[7][iIon+1];
		}
*/
    for (iIon = 0; iIon < ionh; iIon++) {
      b[0][iIon] = (tdc[1][ionh - iIon] - tsum[1] / 2.);
      b[1][iIon] = (tdc[2][ionh - iIon] - tsum[1] / 2.);
      b[2][iIon] = (tdc[3][ionh - iIon] - tsum[2] / 2.);
      b[3][iIon] = (tdc[4][ionh - iIon] - tsum[2] / 2.);
      b[4][iIon] = (tdc[5][ionh - iIon] - tsum[3] / 2.);
      b[5][iIon] = (tdc[6][ionh - iIon] - tsum[3] / 2.);
      b[6][iIon] = tdc[7][ionh - iIon];
    }
    for (iIon = ionh; iIon < nhit; iIon++) {
      b[0][iIon] = (tdc[1][iIon + 1] - tsum[1] / 2.);
      b[1][iIon] = (tdc[2][iIon + 1] - tsum[1] / 2.);
      b[2][iIon] = (tdc[3][iIon + 1] - tsum[2] / 2.);
      b[3][iIon] = (tdc[4][iIon + 1] - tsum[2] / 2.);
      b[4][iIon] = (tdc[5][iIon + 1] - tsum[3] / 2.);
      b[5][iIon] = (tdc[6][iIon + 1] - tsum[3] / 2.);
      b[6][iIon] = tdc[7][iIon + 1];
    }


    for (iTDC = 0; iTDC < nTDChalf - 1; iTDC++) n_ion[iTDC] = cnt[iTDC + 1];
  }  // if (iresort == 1)

  // flagsi correction 2015/02/09
  for (n = 0; n < nhit; n++) {
    if (flagsi[n + 1] > eFlagCondition) {
      b[6][n] = 16000;///2015/10/03
      electronsAreInCondition = false;
      //goto hell_2;
    }
  }

  // End of ion-part  (hexagonal Detector)

  //////////////////////////////////////////////////////////////////////

  unsigned int MyData[100];
  dOrigin[0] = Time_ns[7][0];

  if ((iresort == 1) && (ionh > 0)) {
    for (ihit = 0; ihit < nhit; ihit++) {
      Time_ns[0][ihit] = xr[ihit + 1] + 100;        // x1 (mm)
      Time_ns[1][ihit] = yr[ihit + 1] + 100;        // x2 (mm)
      Time_ns[2][ihit] = a[6][ihit] - dOrigin[0] + 2000;        // MCP (ns)
      Time_ns[3][ihit] = xri[ihit + 1] + 100;        // x1 (mm)
      Time_ns[4][ihit] = yri[ihit + 1] + 100;        // x2 (mm)
      Time_ns[5][ihit] = b[6][ihit] - dOrigin[0] + 2000;        // MCP (ns)
      //Time_ns[6][ihit] = a[6][ihit] - dOrigin[0] +2000;		// MCP (ns)





      // *** Export ROOT File: Fill ***
      elecDataSet[ihit].x = Time_ns[0][ihit];
      elecDataSet[ihit].y = Time_ns[1][ihit];
      elecDataSet[ihit].t = Time_ns[2][ihit];
      elecDataSet[ihit].flag = flags[ihit + 1];
      ionDataSet[ihit].x = Time_ns[3][ihit];
      ionDataSet[ihit].y = Time_ns[4][ihit];
      ionDataSet[ihit].t = Time_ns[5][ihit];
      ionDataSet[ihit].flag = flagsi[ihit + 1];
    }
    pRootTree->Fill();





    //for(ihit=0;ihit<nhit;ihit++){
    //	Time_ns[8][ihit] = xri[ihit+1]+100;		// x1 (mm)
    //	Time_ns[9][ihit] = yri[ihit+1]+100;		// x2 (mm)
    //	Time_ns[10][ihit] = b[2][ihit]+1000;		// y1 (ns)
    //	Time_ns[11][ihit] = b[3][ihit]+1000;		// y2 (ns)
    //	Time_ns[7][ihit] = b[6][ihit]- dOrigin[0] + 2000;		// MCP (ns)
    //	Time_ns[12][ihit] = Time_ns[15][0] + 5000;		// MCP (ns)
    //}
    double t1000 = 0;
    for (iTDC = 0; iTDC < 6; iTDC++) {
      for (ihit = 0; ihit < nhit + 1; ihit++) {
        t1000 = 1000 * Time_ns[iTDC][ihit];
        if (t1000 < 0) t1000 = 16 * 16 * 16 * 16 * 16 * 16 - 1;
        if (t1000 < 16 * 16 * 16 * 16 * 16 * 16 - 1) {
          MyData[0] = (unsigned int) (t1000);
          if (MyData[0] < 0) MyData[0] = 16 * 16 * 16 * 16 * 16 * 16 - 1;
        }
        else {
          MyData[0] = 16 * 16 * 16 * 16 * 16 * 16 - 1;
        }
        if (ionsAreInCondition || electronsAreInCondition) {
//          fwrite(&MyData, 3, 1, fpout);
        }
      }
    }
  }

  for (ihit = 0; ihit < nhit; ihit++) {
    Time_ns[0][ihit] = a[0][ihit];        // x1 (ns)
    Time_ns[1][ihit] = a[1][ihit];        // x2 (ns)
    Time_ns[2][ihit] = a[2][ihit];        // y1 (ns)
    Time_ns[3][ihit] = a[3][ihit];        // y2 (ns)
    Time_ns[4][ihit] = a[4][ihit];        // z1 (ns)
    Time_ns[5][ihit] = a[5][ihit];        // z2 (ns)
    Time_ns[6][ihit] = a[6][ihit];        // MCP (ns)
    Time_ns[7][ihit] = dOrigin[0];        // MCP (ns)
    Time_ns[8][ihit] = b[0][ihit];        // x1 (mm)
    Time_ns[9][ihit] = b[1][ihit];        // x2 (mm)
    Time_ns[10][ihit] = b[2][ihit];        // y1 (ns)
    Time_ns[11][ihit] = b[3][ihit];        // y2 (ns)
    Time_ns[12][ihit] = b[4][ihit];        // MCP (ns)
    Time_ns[13][ihit] = b[5][ihit];        // MCP (ns)
    Time_ns[14][ihit] = b[6][ihit];        // x1 (mm)
  }


  for (i = 0; i < nDetector; i++) {
    for (ihit = 0; ihit < nhit; ihit++) {
      dTx[i][ihit] = -10000000;
      dTy[i][ihit] = -10000000;
      dTz[i][ihit] = -10000000;
      SumTx[i][ihit] = -10000000;
      SumTy[i][ihit] = -10000000;
      SumTz[i][ihit] = -10000000;
      Xuv[i][ihit] = -10000000;
      Xuv[i][ihit] = -10000000;
    }

    //  TOF of particle
    for (ihit = 0; ihit < nhit; ihit++)
      dtParticle2[i][ihit] = (Time_ns[6 + i * nTDChalf][ihit] - dOrigin[0]);

    for (ihit = 0; ihit < nhit; ihit++) {
      SumTx[i][ihit] = 2 * Time_ns[6 + i * 8][ihit]
          - (Time_ns[1 + i * 8][ihit] + Time_ns[0 + i * 8][ihit]);
      SumTy[i][ihit] = 2 * Time_ns[6 + i * 8][ihit]
          - (Time_ns[3 + i * 8][ihit] + Time_ns[2 + i * 8][ihit]);
      SumTz[i][ihit] = 2 * Time_ns[6 + i * 8][ihit]
          - (Time_ns[5 + i * 8][ihit] + Time_ns[4 + i * 8][ihit]);

      dTx[i][ihit] = (Time_ns[1 + i * 8][ihit] - Time_ns[0 + i * 8][ihit]) / 2
          / f1[i];//for electron
      dTy[i][ihit] = (Time_ns[3 + i * 8][ihit] - Time_ns[2 + i * 8][ihit]) / 2
          / f2[i];//for electron
      dTz[i][ihit] = (Time_ns[5 + i * 8][ihit] - Time_ns[4 + i * 8][ihit]) / 2
          / f3[i];//for electron

      Xuv[i][ihit] =
          dTx[i][ihit];                            //xa,ya���X,Y���v�Z
      Yuv[i][ihit] = (1 / sqrt(3.) * (dTy[i][ihit] + dTz[i][ihit]));
    }

  }


  ////////////////////////////////////////////////////////////////////////////////

/*
	double DifTx[2][6], DifTy[2][6], DifTz[2][6];
	double Xuv[2][6], Yuv[2][6];
	double dx12[2][6], dx13[2][6], dx23[2][6], dy12[2][6], dy13[2][6], dy23[2][6];
	double dxeich[2][6], dy1eich[2][6], dy2eich[2][6], dy3eich[2][6];


	for(i=0;i<nDetector;i++)
	{
	for(ihit=0;ihit<nhit;ihit++)
	{
	dTx[i][ihit] = -10000000;
	dTy[i][ihit] = -10000000;
	dTz[i][ihit] = -10000000;
	DifTx[i][ihit] = -10000000;
	DifTy[i][ihit] = -10000000;
	DifTz[i][ihit] = -10000000;
	SumTx[i][ihit] =-10000000;
	SumTy[i][ihit] = -10000000;
	SumTz[i][ihit] = -10000000;
	Xuv[i][ihit] = -10000000;
	Xuv[i][ihit] = -10000000;
	dy1eich[i][ihit] = -10000000;
	dy2eich[i][ihit] = -10000000;
	dy3eich[i][ihit] = -10000000;
	dxeich[i][ihit] = -10000000;
	}

	dOrigin[i] =  Time_ns[7][0] + dInterval;
	dOrigin2[i] =  Time_ns[15][0] + dInterval;
	//  TOF of particle
	for(ihit=0;ihit<nCount[6+i*nTDChalf];ihit++) dtParticle[i][ihit] = (Time_ns[6+i*nTDChalf][ihit] - dOrigin[i]);
	for(ihit=0;ihit<nCount[6+i*nTDChalf];ihit++) dtParticle2[i][ihit] = (Time_ns[6+i*nTDChalf][ihit] - dOrigin2[i]);



	for(ihit=0;ihit<iHitsTDC[i];ihit++)
	{
	SumTx[i][ihit] = 2 * Time_ns[6+i*8][ihit] - (Time_ns[1+i*8][ihit] + Time_ns[0+i*8][ihit]) ;
	SumTy[i][ihit] = 2 * Time_ns[6+i*8][ihit] - (Time_ns[3+i*8][ihit] + Time_ns[2+i*8][ihit]) ;
	SumTz[i][ihit] = 2 * Time_ns[6+i*8][ihit] - (Time_ns[5+i*8][ihit] + Time_ns[4+i*8][ihit]) ;

	dTx[i][ihit] = (Time_ns[1+i*8][ihit] - Time_ns[0+i*8][ihit])/2/f1[i]	;//for electron
	dTy[i][ihit] = (Time_ns[3+i*8][ihit] - Time_ns[2+i*8][ihit])/2/f2[i]	;//for electron
	dTz[i][ihit] = (Time_ns[5+i*8][ihit] - Time_ns[4+i*8][ihit])/2/f3[i]	;//for electron
	dx12[i][ihit] = dTx[i][ihit];
	dx13[i][ihit] = dTx[i][ihit];
	dx23[i][ihit] = dTy[i][ihit]-dTz[i][ihit];
	dy12[i][ihit] = (1./sqrt(3.)) * (2. * dTy[i][ihit] - dTx[i][ihit]);
	dy13[i][ihit] = (1./sqrt(3.)) * (2. * dTz[i][ihit] + dTx[i][ihit]);
	dy23[i][ihit] = (1./sqrt(3.)) * (dTy[i][ihit]+dTz[i][ihit]);

	dxeich[i][ihit] = dx12[i][ihit] - dx23[i][ihit];
	dy1eich[i][ihit] = dy13[i][ihit] - dy12[i][ihit];
	dy2eich[i][ihit] = dy23[i][ihit] - dy12[i][ihit];
	dy3eich[i][ihit] = dy23[i][ihit] - dy13[i][ihit];

	Xuv[i][ihit]	=	dTx[i][ihit];							//xa,ya���X,Y���v�Z
	Yuv[i][ihit]	=	(1/sqrt(3.) * (dTy[i][ihit]+dTz[i][ihit]));
	DifTx[i][ihit] = (Time_ns[1+i*8][ihit] - Time_ns[0+i*8][ihit])	;//for electron
	DifTy[i][ihit] = (Time_ns[3+i*8][ihit] - Time_ns[2+i*8][ihit])	;
	DifTz[i][ihit] = (Time_ns[5+i*8][ihit] - Time_ns[4+i*8][ihit])	;
	}

	}

*/


  TrueCount++;


  hell_2:

  ///////////////////////////////////// Detector test ////////////////////////
  double dx[2], dy[2], dz[2], f2best[2], f3best[2];

  ///////////////////////////////////////OUT put ////////////////////////////////////////////////


  for (iTDC = 0; iTDC < nTDC; iTDC++) {

    for (ihit = 0; ihit < nhit; ihit++) {
      EventData[ipStartDANData + 24 + ihit + iTDC * 4] = Time_ns[iTDC][ihit];
      EventData[ipStartDANData + 199 + ihit + iTDC * 4] = TDCData[iTDC][ihit];

    }
  }

  for (iTDC = 0; iTDC < nTDC; iTDC++) {

    EventData[ipStartDANData + 88 + iTDC] = nCount[iTDC];
  }

  for (i = 0; i < nDetector; i++) {
    for (ihit = 0; ihit < nhit; ihit++) {

      EventData[ipStartDANData + 104 + 32 * i + ihit] = dTx[i][ihit];
      EventData[ipStartDANData + 104 + 32 * i + 4 + ihit] = dTy[i][ihit];
      EventData[ipStartDANData + 104 + 32 * i + 8 + ihit] = dTz[i][ihit];
      EventData[ipStartDANData + 104 + 32 * i + 12 + ihit] = SumTx[i][ihit];
      EventData[ipStartDANData + 104 + 32 * i + 16 + ihit] = SumTy[i][ihit];
      EventData[ipStartDANData + 104 + 32 * i + 20 + ihit] = SumTz[i][ihit];
      EventData[ipStartDANData + 104 + 32 * i + 24 + ihit] = Xuv[i][ihit];
      EventData[ipStartDANData + 104 + 32 * i + 28 + ihit] = Yuv[i][ihit];
    }
  }


  for (i = 0; i < nDetector; i++) {
    EventData[ipStartDANData + 168 + i] = dx[i];
    EventData[ipStartDANData + 168 + 2 + i] = dy[i];
    EventData[ipStartDANData + 168 + 4 + i] = dz[i];
    EventData[ipStartDANData + 168 + 6 + i] = f2best[i];
    EventData[ipStartDANData + 168 + 8 + i] = f3best[i];
    EventData[ipStartDANData + 168 + 10 + i] = iHitsTDC[i];

    for (ihit = 0; ihit < nhit; ihit++) {
      EventData[ipStartDANData + 180 + i * 4 + ihit] = bitpattern[i][ihit];
    }
  }
  int iANAstart = 188;
  for (i = 0; i < nDetector; i++) {
    for (ihit = 0; ihit < nhit; ihit++) {
      EventData[ipStartDANData + iANAstart + ihit + i * 4] =
          dtParticle2[i][ihit];
    }
  }

  EventData[ipStartDANData + iANAstart + 8] = Xx;
  EventData[ipStartDANData + iANAstart + 9] = Yx;
  EventData[ipStartDANData + iANAstart + 10] = Xy;
  EventData[ipStartDANData + iANAstart + 11] = Yy;
  EventData[ipStartDANData + iANAstart + 12] = Xz;
  EventData[ipStartDANData + iANAstart + 13] = Yz;
  EventData[ipStartDANData + iANAstart + 14] = Xxi;
  EventData[ipStartDANData + iANAstart + 15] = Yxi;
  EventData[ipStartDANData + iANAstart + 16] = Xyi;
  EventData[ipStartDANData + iANAstart + 17] = Yyi;
  EventData[ipStartDANData + iANAstart + 18] = Xzi;
  EventData[ipStartDANData + iANAstart + 19] = Yzi;

  for (ihit = 0; ihit < nhit; ihit++) {
    EventData[ipStartDANData + iANAstart + 20 + ihit] = -xri[ihit + 1];
    EventData[ipStartDANData + iANAstart + 20 + 4 + ihit] = -yri[ihit + 1];
    EventData[ipStartDANData + iANAstart + 20 + 8 + ihit] = -xr[ihit + 1];
    EventData[ipStartDANData + iANAstart + 20 + 12 + ihit] = -yr[ihit + 1];
  }
  EventData[ipStartDANData + iANAstart + 36] = Xx1;
  EventData[ipStartDANData + iANAstart + 37] = Yx1;
  EventData[ipStartDANData + iANAstart + 38] = Xy1;
  EventData[ipStartDANData + iANAstart + 39] = Yy1;
  EventData[ipStartDANData + iANAstart + 40] = Xz1;
  EventData[ipStartDANData + iANAstart + 41] = Yz1;
  EventData[ipStartDANData + iANAstart + 42] = Xx2;
  EventData[ipStartDANData + iANAstart + 43] = Yx2;
  EventData[ipStartDANData + iANAstart + 44] = Xy2;
  EventData[ipStartDANData + iANAstart + 45] = Yy2;
  EventData[ipStartDANData + iANAstart + 46] = Xz2;
  EventData[ipStartDANData + iANAstart + 47] = Yz2;

  EventData[ipStartDANData + iANAstart + 48] = corr_sum_ele[1];
  EventData[ipStartDANData + iANAstart + 49] = corr_sum_ele[2];
  EventData[ipStartDANData + iANAstart + 50] = corr_sum_ele[3];

  EventData[ipStartDANData + iANAstart + 51] = Xxi1;
  EventData[ipStartDANData + iANAstart + 52] = Yxi1;
  EventData[ipStartDANData + iANAstart + 53] = Xyi1;
  EventData[ipStartDANData + iANAstart + 54] = Yyi1;
  EventData[ipStartDANData + iANAstart + 55] = Xzi1;
  EventData[ipStartDANData + iANAstart + 56] = Yzi1;
  EventData[ipStartDANData + iANAstart + 57] = Xxi2;
  EventData[ipStartDANData + iANAstart + 58] = Yxi2;
  EventData[ipStartDANData + iANAstart + 59] = Xyi2;
  EventData[ipStartDANData + iANAstart + 60] = Yyi2;
  EventData[ipStartDANData + iANAstart + 61] = Xzi2;
  EventData[ipStartDANData + iANAstart + 62] = Yzi2;

  EventData[ipStartDANData + iANAstart + 63] = corr_sum_ion[1];
  EventData[ipStartDANData + iANAstart + 64] = corr_sum_ion[2];
  EventData[ipStartDANData + iANAstart + 65] = corr_sum_ion[3];

  EventData[ipStartDANData + iANAstart + 66] = Xuv[0][0] + xr[1];
  EventData[ipStartDANData + iANAstart + 67] = Yuv[0][0] + yr[1];
  EventData[ipStartDANData + iANAstart + 68] = Xuv[1][0] + xri[1];
  EventData[ipStartDANData + iANAstart + 69] = Yuv[1][0] + yri[1];

  EventData[ipStartDANData + iANAstart + 70] = flags[1];
  EventData[ipStartDANData + iANAstart + 71] = flagsi[1];
  EventData[ipStartDANData + iANAstart + 72] = flags[2];
  EventData[ipStartDANData + iANAstart + 73] = flagsi[2];

  EventData[ipStartDANData + iANAstart + 74] = eleh;
  EventData[ipStartDANData + iANAstart + 75] = ionh;
  EventData[ipStartDANData + iANAstart + 76] = Xx / 2 / f1[0];
  EventData[ipStartDANData + iANAstart + 77] =
      (Xy / 2 / f2[0] + (Xz + offset[0]) / 2 / f3[0]) / sqrt(3.);
  EventData[ipStartDANData + iANAstart + 78] = Xxi / 2 / f1[1];
  EventData[ipStartDANData + iANAstart + 79] =
      (Xyi / 2 / f2[1] + (Xzi + offset[1]) / 2 / f3[1]) / sqrt(3.);
  EventData[ipStartDANData + iANAstart + 80] = electronMarker;

  /////////////////////// end calc /////////////////////



  return true;
}


///////////////////////
// AnalysisFinalize
///////////////////////
// is called when analysis is stopped (not paused!)
CDAN_API int AnalysisFinalize(CDoubleArray &EventData,
                              CDoubleArray &Parameters,
                              CDoubleArray &WeighParameter) {





  // *** Export ROOT File: Finalize ***
  pRootTree->Write();
  pRootFile->Close();
  delete pRootTree;
  delete pRootFile;
  rootFilename = "";


//  fclose(fpout);
  if (pUserSP) {
    delete pUserSP;
    pUserSP = NULL;
  }
  return true;
}
