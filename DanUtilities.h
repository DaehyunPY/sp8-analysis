///////////////////////////////////////////////////////////////////////////
//	DAN utilities called from CoboldPC DAN
//  (c) by Klaus Ullmann-Pfleger & RoentDek Handels GmbH
//
//  V1.0 - 31.01.2002
///////////////////////////////////////////////////////////////////////////


// Parameter 100 = Conversion Parameter for RAW data
#define TC_COUNTS	0	// Counts (no conversion)
#define TC_TIME		1	// Time (ns)
#define TC_POSITION	2	// Point (mm)

// Parameter 101 = Autonoise (for floating point coordinates)
#define AN_OFF		0	// Autonoise off
#define	AN_ON		1	// Autonoise on

// Parameter 102 = Hex-Anode calculations
#define	HA_OFF		0	// no Hex-Anode
#define	HA_ON		1	// Hex-Anode

// Parameter 103 = PHI conversion
#define AF_RAD		0	// RAD [-pi..pi]
#define AF_RAD0		1	// RAD [0..2pi]
#define AF_DEG		2	// DEG [-180..180]
#define AF_DEG0		3	// DEG [0..360]

// Parameter 104 = DNL correction (GP1/HM1 only)
#define	DNL_OFF		0	// no DNL correction
#define	DNL_ON		1	// Correction value (typically 0.31)

////////////////////////////////////////////////////////////
// 
// usefull constants
//

#define PI		3.1415926535897932384626433832795			// PI
#define PID2	(PI/2.)							// half PI
#define I_PI	(1/PI)										// one over PI

////////////////////////////////////////////////////////////
// 
// usefull functions
//
void RotateXY(double dX, double dY, double dAngle, int iAFormat, double dCX, double dCY, double &dRX, double &dRY);
void OrthoToRPhi(double dX, double dY, int iAFormat, double dCX, double dCY, double &dR, double &dPhi);