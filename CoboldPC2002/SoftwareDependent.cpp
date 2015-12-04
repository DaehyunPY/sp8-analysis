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
#include "StandardDAN.cpp"				// Standard DAN Module

#include "../include/Analysis/JSONReader.cpp"
#include "../include/Analysis/LMFReader.cpp"

#include "../include/Analysis/Unit.cpp"
#include "../include/Analysis/Flag.cpp"
#include "../include/Analysis/ObjectFlag.cpp"
#include "../include/Analysis/EventDataFlag.cpp"

#include "../include/Analysis/Object.cpp"
#include "../include/Analysis/Objects.cpp"
#include "../include/Analysis/Ion.cpp"
#include "../include/Analysis/Ions.cpp"
#include "../include/Analysis/Electron.cpp"
#include "../include/Analysis/Electrons.cpp"

#include "../include/Analysis/EquipmentParameters.cpp"
#include "../include/Analysis/ObjectParameters.cpp"
#include "../include/Analysis/IonParameters.cpp"
#include "../include/Analysis/ElectronParameters.cpp"
#include "../include/Analysis/AnalysisTools.cpp"
