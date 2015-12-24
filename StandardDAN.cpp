///////////////////////////////////////////////////////////////////////////
//	Standard User defined analysis part called from cobold main program
//  (c) by Klaus Ullmann-Pfleger & RoentDek Handels GmbH
//
//  V1 - 20.07.2001
//  V2 - 08.03.2002		- is now independent from # of Hits of DAQ
//						- n = number of hit information now calculated
//  V3 - 08.04.2002		- adaption to new layout for DAN (hex anode)
///////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// Parameter description used in this insert dependent part!
// ---------------------------------------------------------
//
// Used from DAQ
// -------------
// Parameter  2 = Save TimeStamp
//                0 = no Timestamp,
//                1 = 32Bit Timestamp	(Low.Low, Low.high)
//                2 = 64Bit Timestamp	(Low.Low, Low.high, High.Low, High.high)
// Parameter  5 = TimeScaling (Internally set, tics per second)
// Parameter  6 = DAQ Version # (Internally set)
// Parameter  7 = Time Reference of DAQ Initialize (Start time of LMF) (Internally set)
// Parameter  8 = DAQ_ID
//					DAQ_ID_RAW			0x000000		// for RAW (no Data)
//					DAQ_ID_HM1			0x000001		// for HM1 (single)
//					DAQ_ID_TDC8			0x000002		// for TDC8/ISA/PCI
//					DAQ_ID_CAMAC		0x000003		// for CAMAC
//					DAQ_ID_2HM1			0x000004		// for 2 HM1
//					DAQ_ID_2TDC8		0x000005		// for 2 TDC8
//
// Parameter 20 = Resolution of TDC in ns (internally set)
// Parameter 21 = TDC data type information (internally set)
//			0 = Not defined
//			1 = Channel information
//			2 = Time information (in ns)
//
// Parameter 32 = number of Channels (reread during offline)
//				1..
// Parameter 33 = number of hits (reread during offline)
//              1..
//
// Parameter 40 = DataFormat (Internally set)
//
// Needed by DAN
// -------------
// Parameter 100 = Conversion Parameter for RAW data
//			0 = Counts (no conversion)
//			1 = Time (ns)
//			2 = Point (mm)
// Parameter 102 = Hex-Anode calculations
//			0 = no Hex-Anode
//			1 = Hex-Anode
// Parameter 103 = PHI conversion
//			0 = RAD [-pi..pi]
//			1 = RAD [0..2pi]
//			2 = DEG [-180..180]
//			3 = DEG [0..360]
// Parameter 104 = DNL correction (GP1/HM1 only)
//			0 = no DNL correction
//			1 = Correction value (typically 0.31)
// Parameter 105 = Start of DAQ Data for DAN (Start Coordinate)
//			0 = automatic
// Parameter 106 = Start of DAN Data (Start Coordinate)
//			0 = automatic
// Parameter 107 = Analyze hit #
//
// Parameter 110 = pTPCalX = Time to Point calibration factor for x (mm/ns)
// Parameter 111 = pTPCalY = Time to Point calibration factor for y (mm/ns)
// Parameter 112 = pTPCalZ = Time to Point calibration factor for z (mm/ns)
//
// Parameter 120 = pCOx = Rotation Offset Center for x
// Parameter 121 = pCOy = Rotation Offset Center for y
// Parameter 122 = pRotA = Rotation Angle mathematical direction
// 		      		value in RAD if Parameter 103 = 0 or 1
//		      		value in DEG if Parameter 103 = 2 or 3
//
// Parameter 135 = pOPx = Offset for x Point
// Parameter 136 = pOPy = Offset for y Point
// Parameter 137 = pOPw = Offset for w Point
// Parameter 138 = pOSum = Offset for Sum/Diff calculations
//
/////////////////////////////////////////////////////////////////////////////
// Raw data
/////////////////////////////////////////////////////////////////////////////
// TimeStamp
// n1,x1 = channel 1 (hit 1..)
// n2,x2 = channel 2 (hit 1..)
// n3,y1 = channel 3 (hit 1..)
// n4,y2 = channel 4 (hit 1..)
// n5,z1 = channel 5 (hit 1..)
// n6,z2 = channel 6 (hit 1..)

/////////////////////////////////////////////////////////////////////////////
// Conversions
/////////////////////////////////////////////////////////////////////////////
// to time
//	time = TDCR / channel					(ns)
//		TDCR = TDCResolution from DAQ Parameter 
//  point = time / pTPCal
//		pTPCal = time (ns)/mm
// ROT = (cos   -sin)
//		 (sin   -cos)

/////////////////////////////////////////////////////////////////////////////
// coordinates calculated by this DAN
// ------------------------------------------------
//
// AbsoluteEventTime
// AbsoluteDeltaEventTime
// EventCounter
// True
// ConsistensIndicator
// PLLStatusLocked
//
// n1,n2,n3,n4,n5,n6							number of hits for channel (i)
// x1,x2										counts, time(ns) or point(mm)
// y1,y2										counts, time(ns) or point(mm)
// z1,z2	(not used here, always 0, hex anode)					counts, time(ns) or point(mm)
//
// x												counts, time(ns) or point(mm)
//		x	  = (x1 - x2)
// y												counts, time(ns) or point(mm)
//		y     = (y1 - y2)
// w(mm)											counts, time(ns) or point(mm)
//		w     = 0									(if hex flag not set)
//		w(mm) = (z1 - z2) + pOPw					(if hex flag set)
//
// sumx		(x1 + x2) + pOSum								counts, time(ns) or point(mm)
// sumy		(y1 + y2) + pOSum								counts, time(ns) or point(mm)
// sumw		(z1 + z2) + pOSum								counts, time(ns) or point(mm)
// sumxyw	(x1 + x2) + (y1 + y2) + (z1 + z2) + pOSum		counts, time(ns) or point(mm)
// diffxy	(sumx - sumy) + pOSum							counts, time(ns) or point(mm)
//
// PosX
//		= x + pOPx									(if hex flag not set)
//		= -x + pOPx									(if hex flag set)
// PosY
//		= y + pOPy									(if hex flag not set)
//		= 1/sqrt(3) * (2y+x) + pOPy					(if hex flag set)
//
// r		sqrt((PosX-pCOx)^2 + (PosY-pCOy)^2)		counts, time(ns) or point(mm)
// phi		acos((PosX-pCOx) / r) / pi * 180. * (PosY-pCOy)/fabs((PosY-pCOy))		RAD or DEG
// xRot		(PosX-pCOx) * ROT + pCOx				counts, time(ns) or point(mm)
// yRot		(PosY-pCOy) * ROT + pCOy				counts, time(ns) or point(mm)
//
// Xuv		-x + pOPx
// Yuv		1/sqrt(3) * (2y+x) + pOPy
// Xuw		Xuv
// Yuw		1/sqrt(3) * (2w-x) + pOPy
// Xvw		1/sqrt(3) * (y-w) + pOPx
// Yvw		1/sqrt(3) * (y+w) + pOPy
// dX		Xuv - Xvw
// dY		Yuv - Yvw
// 
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// variables definitions

LARGE_INTEGER	lipTimeStamp;	// TimeStamp time information
int ipTimeStamp;			// TimeStamp information type
double dpTimeScaling;		// Time Scaling (ticks per second)
CTime ctpLMFStartTime;		// Start Time of LMF

int iDAQ_ID;				// parameter 8

double dpTDCResolution;		// parameter 20
int ipTDCDataType;			// parameter 21

int	ipNumberOfChannels;		// parameter 32
int	ipNumberOfHits;			// parameter 33

int ipConversion;			// parameter 100
bool bpHexAnodeFlag;		// parameter 102
int ipPhiConversion;		// parameter 103
double dpDnlCorrection;		// parameter 104
int ipStartDAQData;			// parameter 105
int ipStartDANData;			// parameter 106
int ipUseHit;				// parameter 107

double dpTPCalX;			// parameter 110
double dpTPCalY;			// parameter 111
double dpTPCalZ;			// parameter 112

double dpCOx;				// parameter 120
double dpCOy;				// parameter 121
double dpRotA;				// parameter 122

double dpOPx;				// parameter 135
double dpOPy;				// parameter 136
double dpOPw;				// parameter 137
double dpOSum;				// parameter 138

double dStartDanCoordinates;
int iDataOffset;
int iIndexDataOffset;
int ipStartDAQTDCData;


double EventCounter;				// EventCounter for the data in LM-file

///////////////////////////////
// AnalysisGetInformationString
///////////////////////////////
// is called during startup procedure of CoboldPC
// 
// return value is a string that will be displayed in 
// the splash box and the about box

CDAN_API LPCTSTR AnalysisGetInformationString()
{
	return LPCTSTR("RoentDek Standard (V3)");
}

/////////////////////
// AnalysisInitialize
/////////////////////
// is called when new command is executed
//
// return value is false -> error during initialize
//                 true  -> initialize ok 

CDAN_API BOOL AnalysisInitialize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
	_dOldEventTime = 0.0;
	_dStartEventTime = 0.0;

	// first check if all parameters are set
	if(pParameters->GetSize() < 138)
	{
		AfxMessageBox("Not all parameters are defined\n for this DAN module!");
		return false;
	}

	// check DAQ-Version
	if((int)(pParameters->GetAt(5)+0.1) != 20020408)
	{
		AfxMessageBox("DAN module works only with list mode data\ntaken with Standard Daq-Modules from\n08.04.2002",MB_ICONSTOP);
		return false;
	}

	// transfer parameters
	//    in the GetAt lines you'll find the following correction values
	//    0.1 for float to int converion
	//    -1 if parameter is for indexing
	ipTimeStamp = (int)(pParameters->GetAt(1)+0.1);										// parameter 2
	dpTimeScaling = pParameters->GetAt(4);												// parameter 5
	ctpLMFStartTime = CTime((time_t)((int)(pParameters->GetAt(6)+0.1)));				// parameter 7
	iDAQ_ID = (int)(pParameters->GetAt(7)+0.1);											// parameter 8

	dpTDCResolution = pParameters->GetAt(19);											// parameter 20
	ipTDCDataType = (int)(pParameters->GetAt(20)+0.1);									// parameter 21

	ipNumberOfChannels = (int)(pParameters->GetAt(31)+0.1);								// parameter 32
	ipNumberOfHits = (int)(pParameters->GetAt(32)+0.1);									// parameter 33

	ipConversion = (int)(pParameters->GetAt(99)+0.1);									// parameter 100
	bpHexAnodeFlag = (bool)((int)(pParameters->GetAt(101)+0.1) ? true : false);			// parameter 102
	ipPhiConversion = (int)(pParameters->GetAt(102)+0.1);								// parameter 103
	dpDnlCorrection = pParameters->GetAt(103);											// parameter 104
	ipStartDAQData = (int)(pParameters->GetAt(104)+0.1) - 1;							// parameter 105
	ipStartDANData = (int)(pParameters->GetAt(105)+0.1) - 1;							// parameter 106
	ipUseHit = (int)(pParameters->GetAt(106)+0.1);										// parameter 106

	dpTPCalX = pParameters->GetAt(109);													// parameter 110
	dpTPCalY = pParameters->GetAt(110);													// parameter 111
	dpTPCalZ = pParameters->GetAt(111);													// parameter 112

	dpCOx = pParameters->GetAt(119);													// parameter 120
	dpCOy = pParameters->GetAt(120);													// parameter 121
	dpRotA = pParameters->GetAt(121);													// parameter 122

	dpOPx = pParameters->GetAt(134);													// parameter 135
	dpOPy = pParameters->GetAt(135);													// parameter 136
	dpOPw = pParameters->GetAt(136);													// parameter 137
	dpOSum = pParameters->GetAt(137);													// parameter 138

	// check parameter 20 for valid data
	if(pParameters->GetAt(19) <= 0.0)
	{
		AfxMessageBox("Parameter 20\nTDC-resolution\ncontains no valid data!\n\nValues should be >= 0.0");
		return false;
	}

	// check parameter 21 for valid data
	if((int)(pParameters->GetAt(20)) < 0 || (int)(pParameters->GetAt(20)) > 2)
	{
		AfxMessageBox("Parameter 21\nTDC-data type\ncontains no valid data!\n\nValues should be 0,1 or 2");
		return false;
	}

	// set EventCounter to zero at start of new command
	EventCounter = 0;

	// get information about time-info 
	iDataOffset = 0;
	iIndexDataOffset = 2;

	if(ipTimeStamp == 0)
	{
		iDataOffset = 0;
		iIndexDataOffset = 4;
	}
	if(ipTimeStamp == 1)
	{
		iDataOffset = 2;
		iIndexDataOffset = 2;
	}
	if(ipTimeStamp == 2)
	{
		iDataOffset = 4;
		iIndexDataOffset = 0;
	}

	// correct data for start of dan coordinates
	if(ipStartDANData == -1)
	{
		ipStartDANData = 4 + (ipNumberOfChannels * (ipNumberOfHits+1));	// timeinfo + tdc data
		pParameters->SetAt(105,double(ipStartDANData));			// write information back to parameter 106
	}
	ipStartDAQTDCData = ipStartDAQData + 4 + 1;

	// initialize the random generator
	srand( (unsigned)time( NULL ) );

	return true;
}

///////////////////////
// AnalysisProcessEvent
///////////////////////
// is called when new command is executed
//CDAQ_API LARGE_INTEGER DAQTimeStamp;
//__declspec(dllimport) int DAQTimeStamp;
__declspec(dllimport) int DAQTimeStamp;

CDAN_API void AnalysisProcessEvent(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
	double AbsoluteEventTime;			// ns since start
	double DeltaEventTime;				// This Time - PreviousTime
	double True = true;					// alsways true
	double ConsistensIndicator;
	double PLLStatusLocked;				// totally locked then true else false

	double n1,n2,n3,n4,n5,n6;
	double x1,x2,y1,y2,z1,z2;
	double x,y,w;
	double sumx,sumy,sumw,sumxyw;
	double diffxy;
	double PosX,PosY;
	double r,phi;
	double xRot,yRot;
	double Xuv,Yuv,Xuw,Yuw,Xvw,Yvw;
	double dX,dY;

	// correct position of Event Data due to time information
	CorrectEventDataPosistion(pEventData,iIndexDataOffset,ipStartDANData);

	// get time information if present
	if(iDataOffset)
	{
		// AbsoluteEventTime
		AbsoluteEventTime = GetEventTime(pEventData,pParameters);
		// AbsoluteDeltaEventTime
		DeltaEventTime = GetDeltaEventTime(pEventData,pParameters);
		// EventCounter
	}

	// EventCounter
	EventCounter += 1;
	// TRUE (set in DAN-Init routine

	// Get Status Information
	if(iDAQ_ID == DAQ_ID_HM1)
	{
		n1 = ((int)(pEventData->GetAt(ipStartDAQTDCData+0*(ipNumberOfHits+1))+0.1) & 0x0007) -1;
		n2 = ((int)(pEventData->GetAt(ipStartDAQTDCData+1*(ipNumberOfHits+1))+0.1) & 0x0007) -1;
		n3 = ((int)(pEventData->GetAt(ipStartDAQTDCData+2*(ipNumberOfHits+1))+0.1) & 0x0007) -1;
		n4 = ((int)(pEventData->GetAt(ipStartDAQTDCData+3*(ipNumberOfHits+1))+0.1) & 0x0007) -1;
		n5 = n6 = 0;
		// correct negative (missed) hit
		n1 = n1 < 0 ? 0 : n1;
		n2 = n2 < 0 ? 0 : n2;
		n3 = n3 < 0 ? 0 : n3;
		n4 = n4 < 0 ? 0 : n4;

		PLLStatusLocked = ((int)(pEventData->GetAt(ipStartDAQTDCData+0*(ipNumberOfHits+1))+0.1) & 0x0080) &
			((int)(pEventData->GetAt(ipStartDAQTDCData+1*(ipNumberOfHits+1))+0.1) & 0x0080) &
			((int)(pEventData->GetAt(ipStartDAQTDCData+2*(ipNumberOfHits+1))+0.1) & 0x0080) &
			((int)(pEventData->GetAt(ipStartDAQTDCData+3*(ipNumberOfHits+1))+0.1) & 0x0080);
		PLLStatusLocked = PLLStatusLocked ? true : false;

	}
	else if (iDAQ_ID == DAQ_ID_TDC8)
	{
		n1 = (int)(pEventData->GetAt(ipStartDAQTDCData+0*(ipNumberOfHits+1))+0.1);
		n2 = (int)(pEventData->GetAt(ipStartDAQTDCData+1*(ipNumberOfHits+1))+0.1);
		n3 = (int)(pEventData->GetAt(ipStartDAQTDCData+2*(ipNumberOfHits+1))+0.1);
		n4 = (int)(pEventData->GetAt(ipStartDAQTDCData+3*(ipNumberOfHits+1))+0.1);
		if(!bpHexAnodeFlag)
			n5 = n6 = 0;
		else
		{
			n5 = (int)(pEventData->GetAt(ipStartDAQTDCData+4*(ipNumberOfHits+1))+0.1);
			n6 = (int)(pEventData->GetAt(ipStartDAQTDCData+5*(ipNumberOfHits+1))+0.1);
		}
		PLLStatusLocked = false;
	}
	else	// for all other... this information is not valid
	{
		PLLStatusLocked = false;
		n1 = n2 = n3 = n4 = n5 = n6 = 0;
	}

	// ConsistensIndicator
	ConsistensIndicator = 
		(n1>=ipUseHit ? 0x0001 : 0x0000) + 
		(n2>=ipUseHit ? 0x0002 : 0x0000) + 
		(n3>=ipUseHit ? 0x0004 : 0x0000) + 
		(n4>=ipUseHit ? 0x0008 : 0x0000) + 
		(n5>=ipUseHit ? 0x0010 : 0x0000) + 
		(n6>=ipUseHit ? 0x0020 : 0x0000);

	// Get Raw-Data
	x1 = pEventData->GetAt(ipStartDAQTDCData+0*(ipNumberOfHits+1)+ipUseHit);
	x2 = pEventData->GetAt(ipStartDAQTDCData+1*(ipNumberOfHits+1)+ipUseHit);
	y1 = pEventData->GetAt(ipStartDAQTDCData+2*(ipNumberOfHits+1)+ipUseHit);
	y2 = pEventData->GetAt(ipStartDAQTDCData+3*(ipNumberOfHits+1)+ipUseHit);
	if(bpHexAnodeFlag)
	{
		z1 = pEventData->GetAt(ipStartDAQTDCData+4*(ipNumberOfHits+1)+ipUseHit);
		z2 = pEventData->GetAt(ipStartDAQTDCData+5*(ipNumberOfHits+1)+ipUseHit);
	}
	else
		z1 = z2 = 0.0;

	// correct DNL if
	if((iDAQ_ID == DAQ_ID_HM1) && dpDnlCorrection)
	{
		x1 = CorrectGP1NDL(x1,dpDnlCorrection);
		x2 = CorrectGP1NDL(x2,dpDnlCorrection);
		y1 = CorrectGP1NDL(y1,dpDnlCorrection);
		y2 = CorrectGP1NDL(y2,dpDnlCorrection);
	}

	// do conversion ? then first to time (ns)
	if(ipConversion)
	{
		x1 *= dpTDCResolution;
		x2 *= dpTDCResolution;
		y1 *= dpTDCResolution;
		y2 *= dpTDCResolution;
		if(bpHexAnodeFlag)
		{
			z1 *= dpTDCResolution;
			z2 *= dpTDCResolution;
		}
	}
	// convert also to position? then to position (mm)
	if(ipConversion == 2)
	{
		x1 *= dpTPCalX;
		x2 *= dpTPCalX;
		y1 *= dpTPCalY;
		y2 *= dpTPCalY;
		if(bpHexAnodeFlag)
		{
			z1 *= dpTPCalZ;
			z2 *= dpTPCalZ;
		}
	}

	// now get the "real" position
	x = (x1 - x2);
	y = (y1 - y2);
	if(bpHexAnodeFlag)
		w = (z1 - z2);
	else
		w = 0;

	// correct scaling due to logic...
	if(ipConversion == 2)
	{
		x /= 2.0;
		y /= 2.0;
		w /= 2.0;
	}
	if(bpHexAnodeFlag)				// after /2 correction add Offset for w
		w += dpOPw;

	// sums and differences
	sumx = (x1 + x2) + dpOSum;
	sumy = (y1 + y2) + dpOSum;
	if(bpHexAnodeFlag)
		sumw = (z1 + z2) + dpOSum;
	else
		sumw = 0;
	if(bpHexAnodeFlag)
		sumxyw = (sumx + sumy + sumw) - (2*dpOSum);		// only one OSum -> -(2*dpOSum)!!!
	else
		sumxyw = (sumx + sumy) - dpOSum;				// only one OSum -> -(dpOSum)!!!
	diffxy = (sumx - sumy) + dpOSum;

	if(bpHexAnodeFlag)
	{
		PosX = -x +dpOPx;
		PosY = 1/sqrt(3) * (2*y+x) + dpOPy;
	}
	else
	{
		PosX = x + dpOPx;
		PosY = y + dpOPy;
	}
	
	// do rotation
	if(dpRotA)
		RotateXY(PosX, PosY, dpRotA, ipPhiConversion, dpCOx, dpCOy, xRot, yRot);
	else
	{
		xRot = PosX;
		yRot = PosY;
	}

	// convert to r-phi
	OrthoToRPhi(PosX, PosY, ipPhiConversion, dpCOx, dpCOy, r, phi);

	if(bpHexAnodeFlag)
	{
		Xuv	=	-x + dpOPx;
		Yuv	=	1/sqrt(3) * (2*y+x) + dpOPy;
		Xuw	=	Xuv;
		Yuw	=	1/sqrt(3) * (2*w-x) + dpOPy;
		Xvw	=	1/sqrt(3) * (y-w) + dpOPx;
		Yvw	=	1/sqrt(3) * (y+w) + dpOPy;
		dX	=	Xuv - Xvw;
		dY	=	Yuv - Yvw;
	}
	else
		Xuv = Yuv = Xuw = Yuw = Xvw = Yvw = dX = dY = 0;		// not hex anode -> set to 0


	/////////////////////////////////////
	// write all data back to coordinates
	/////////////////////////////////////
	pEventData->SetAt(ipStartDANData+0,AbsoluteEventTime);
	pEventData->SetAt(ipStartDANData+1,DeltaEventTime);
	pEventData->SetAt(ipStartDANData+2,EventCounter);
	pEventData->SetAt(ipStartDANData+3,True);
	pEventData->SetAt(ipStartDANData+4,ConsistensIndicator);
	pEventData->SetAt(ipStartDANData+5,PLLStatusLocked);

	pEventData->SetAt(ipStartDANData+6,n1);
	pEventData->SetAt(ipStartDANData+7,n2);
	pEventData->SetAt(ipStartDANData+8,n3);
	pEventData->SetAt(ipStartDANData+9,n4);
	pEventData->SetAt(ipStartDANData+10,n5);
	pEventData->SetAt(ipStartDANData+11,n6);

	pEventData->SetAt(ipStartDANData+12,x1);
	pEventData->SetAt(ipStartDANData+13,x2);
	pEventData->SetAt(ipStartDANData+14,y1);
	pEventData->SetAt(ipStartDANData+15,y2);
	pEventData->SetAt(ipStartDANData+16,z1);
	pEventData->SetAt(ipStartDANData+17,z2);

	pEventData->SetAt(ipStartDANData+18,x);
	pEventData->SetAt(ipStartDANData+19,y);
	pEventData->SetAt(ipStartDANData+20,w);

	pEventData->SetAt(ipStartDANData+21,sumx);
	pEventData->SetAt(ipStartDANData+22,sumy);
	pEventData->SetAt(ipStartDANData+23,sumw);
	pEventData->SetAt(ipStartDANData+24,sumxyw);

	pEventData->SetAt(ipStartDANData+25,diffxy);

	pEventData->SetAt(ipStartDANData+26,PosX);
	pEventData->SetAt(ipStartDANData+27,PosY);

	pEventData->SetAt(ipStartDANData+28,r);
	pEventData->SetAt(ipStartDANData+29,phi);

	pEventData->SetAt(ipStartDANData+30,xRot);
	pEventData->SetAt(ipStartDANData+31,yRot);

	pEventData->SetAt(ipStartDANData+32,Xuv);
	pEventData->SetAt(ipStartDANData+33,Yuv);
	pEventData->SetAt(ipStartDANData+34,Xuw);
	pEventData->SetAt(ipStartDANData+35,Yuw);
	pEventData->SetAt(ipStartDANData+36,Xvw);
	pEventData->SetAt(ipStartDANData+37,Yvw);

	pEventData->SetAt(ipStartDANData+38,dX);
	pEventData->SetAt(ipStartDANData+39,dY);
}


///////////////////////
// AnalysisFinalize
///////////////////////
// is called when analysis is stopped (not paused!)
CDAN_API void AnalysisFinalize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
}
