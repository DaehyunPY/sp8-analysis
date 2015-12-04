///////////////////////////////////////////////////////////////////////////
//	DAN utilities called from CoboldPC DAN
//  (c) by Klaus Ullmann-Pfleger & RoentDek Handels GmbH
//
//  V1.0 - 31.01.2002
///////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DanUtilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>

#include "CDan.h"
#include "cdaq.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////
// RotateXY function
// -----------------
// rotate an point arout a given centerpoint
//
// input
// -----
// dX, dY   = Point to rotate
// dAngle   = Rotation angle (mathematical) in deg or radian
// iAFormat = Angle format (deg or radian, see AF_* in definition file)
// dCX, dCY = Center of rotation
//
// output
// ------
// dRX, dRY = Rotated dX, dY
//
void RotateXY(double dX, double dY, double dAngle, int iAFormat, double dCX, double dCY, double &dRX, double &dRY)
{
	dAngle *= -1;
	if(dAngle != 0.)
	{
		// if angle is in deg then convert it to radian
		if(iAFormat == AF_DEG || iAFormat == AF_DEG0)
			dAngle = (dAngle / 180.0) * PI;

		// move x,y to center of rotation point
		double x = dX - dCX;
		double y = dY - dCY;
		
		// rotation values (rotation matrix coefficient and centerpoint/offset)
		double dSin = sin(dAngle) ;
		double dCos = cos(dAngle) ;
	
		// rotate and decenter
		dRX = x*dCos + y*dSin + dCX; 
		dRY = -x*dSin + y*dCos + dCY; 
	}
	else
	{
		dRX = dX;
		dRY = dY;
	}
}

//////////////
// OrthoToRPhi
// -----------
// Convert orthogonal coordinates to r-phi coordinates
// if dR < 1E-15 then set phi angle to 0
// if abs(y) < 1E-15 then the quadrant can not be corrected!!!
//
// input
// -----
// dX, dY   = Point to rotate
// iAFormat = Angle format (deg or radian, see AF_* in definition file)
// dCX, dCY = Center of rotation
//
// output
// ------
// dR, dPhi = r-phi coordinates
//
//
void OrthoToRPhi(double dX, double dY, int iAFormat, double dCX, double dCY, double &dR, double &dPhi)
{
	// move x,y to center of rotation point
	double x = dX - dCX;
	double y = dY - dCY;

	// calculate radius
	dR = sqrt(x*x + y*y);

	// if radius is to small then set phi to 0
	if(dR > 1e-15)
	{
		// if y is to small then quadrant correction is not possible
		if(fabs(y) > 1e-15)
			dPhi = acos(x / (dR)) * y/fabs(y);
		else
			dPhi = acos(x / (dR));
	}
	else
		dPhi = 0.;

	// convert angle to specified dataformat
	if(iAFormat == AF_RAD)
	{
		if(dPhi > PI)
			dPhi -= (2.0*PI);
	}
	if(iAFormat == AF_DEG || iAFormat == AF_DEG0)
	{
		dPhi /= (PI * 180.);
		if(iAFormat == AF_DEG)
		{
			if(dPhi > 180.0)
				dPhi -= 360.0;
		}
	}
}
