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
// Custom DAN
//
////////////////////////////////////////
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"

#include "Custom/glb.cpp"
#include "Custom/init.cpp"
#include "Custom/proc.cpp"
#include "Custom/fin.cpp"
