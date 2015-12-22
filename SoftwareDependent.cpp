// CDan.cpp : User-Stuff routines for the DLL.
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "CDan.h"
#include "DANUtilities.h"

#include "cdaq.h"
#include "LMInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// insert the subprogram part for software insert here
// ------> update dependencies <------

////////////////////////////////////////
//
//  RoentDek Standard DAN
//
////////////////////////////////////////
// #include "EmptyDAN.cpp"
// #include "StandardDAN.cpp"

#define COBOLDPC2002_SOFTWAREDEPENDENT_H
#define FOR_COBOLDPC2002
#include "Analysis/JSONReader.cpp"
#include "Analysis/EventDataReader.cpp"
#include "Analysis/Unit.cpp"
#include "Analysis/Flag.cpp"
#include "Analysis/ObjectFlag.cpp"
#include "Analysis/EventDataFlag.cpp"
#include "Analysis/Object.cpp"
#include "Analysis/Objects.cpp"
#include "Analysis/Ion.cpp"
#include "Analysis/Ions.cpp"
#include "Analysis/Electron.cpp"
#include "Analysis/Electrons.cpp"
#include "Analysis/EquipmentParameters.cpp"
#include "Analysis/ObjectParameters.cpp"
#include "Analysis/IonParameters.cpp"
#include "Analysis/ElectronParameters.cpp"
#include "Analysis/AnalysisTools.cpp"
#include "AnalysisDAN.cpp"
