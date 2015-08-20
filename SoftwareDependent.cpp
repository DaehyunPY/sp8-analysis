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
