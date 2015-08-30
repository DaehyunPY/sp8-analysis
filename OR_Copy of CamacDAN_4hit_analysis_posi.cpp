// 1 counter is shifted for the 1st hit 2003.6.22

/////////////////////////////////////////////////////////////////////////////
// CamacDAN_match4.cpp
// Auther : M.Machida                                             2003.06.11
// Ident. : Ver.0.0
/////////////////////////////////////////////////////////////////////////////
// Modifications
// Ver.0.1  Modified by M.Oura                                    2003.06.17
// Ver.3.0  Modified by M.Machida                                 2003.06.20
// Ver.4.0  NOW modifying........
/////////////////////////////////////////////////////////////////////////////
//                    ATTENTION!!!!
// We changed ROM of TDC ,so the TAC (min max) value was shifted
//                                                                  2003.06.21
/////////////////////////////////////////////////////////////////////////////
//	User defined analysis part called from cobold main program
/////////////////////////////////////////////////////////////////////////////
// Sample DAN for seven CAMAC Hoshin-TDC with 14 {(6starts&1stop)x2} channels
// each and 6 hits each channels. The Event Array is described below.
// Hexagonal DL-Detector in TDC1-TDC3 Channels 1-2
//             TDC1-ch1   Hex anode X1
//             TDC1-ch2   Hex anode X2
//             TDC2-ch1   Hex anode Y1
//             TDC2-ch2   Hex anode Y2
//             TDC3-ch1   Hex anode Z1
//             TDC3-ch2   Hex anode Z2
//             TDC4-ch1   Hex MCP electron
//             TDC4-ch2   Hex MCP ion
//             TDC5-ch1   Square anode X1
//             TDC5-ch2   Square anode X2
//             TDC6-ch1   Square anode Y1
//             TDC6-ch2   Square anode Y2
//             TDC7-ch1   Bunch signal
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Following is applicable for two-TDC8 coordinate.
// Assuming Control Coordinate in TDC1&2 Channel7&8
// The Control Coordinate is used to synchronise the Date between
// TDC1 and TDC2. Because the this is for both TDCs the same coordinate
// the difference should be a deltafunction close to 0 (depends on local
// delay in each path to the TDCs). So use only data with a condition to 
// this "delta-function". Otherwhise you can't be sure that the data from
// TDC1 and TDC2 are out of the same "trigger".
/////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// Parameter description used in this insert dependent part!
// ---------------------------------------------------------
//
// Hardware
// --------
//
// Software
// --------
/////////////////////////////////////////////////////////////////////////////


#include <stdio.h>
#include <math.h>
FILE *fp1, *fp_cor, *fp_sel;

////////////////  Please   ATTENTION !!!!!!!!   /////////////////////////////
////    This progrm is restricted version for 4hit.  ////////////////////////
////    AnaIon value must be fixed "4" in CCF now.   ////////////////////////
/////////////////////////////////////////////////////////////////////////////
#define	nTDC	7		// Number of TDC used.  Don't forget !! Input real number 
#define	nCH		2		// Number of channels in each TDC.
#define	nHit	4		// This corresponds to number of hit. Check the active CAMAC.CIF

const double pi = 3.141592654;
const double dElectronMass = 9.109534E-31;		// Electron Mass (kg)
const double dElectron = 1.60219E-19;				// Electric charge (Coulomb)
const double dMassUnit = 1.6605656E-27;			// Atomic Mass Unit (kg)
const double dMomentum_au = 1.9921E-24;			// 1 au momentum = 1.9921E-24 (kg*m/s)
const double Nano = 1.0E-9;

// Analysis global variable definitions
int iNumberOfHits;				// # of hits in LMF
int iNumberOfChannels;			// # of coordinates in LMF (always 8)
int iStartTDC1;					// start coordinate for TDC
int iEventCounter;				// coordinate for EventCounter
int iTRUE;						// start coordinate of TRUE (channel 1-8)
int iPos;						// start of position coordinates
int iSum;						// start of sum coordinates
int iSumSq;
int iSqPos;
int iHitPattern;				// start of hit-patter
int iStartTDC;					// start of TDC data
int itIon;						// start of ion TOF
int iIonEnergy;
int iIonP;
int icom;
int itElectron;					// start of electron TOF
int iElectronEnergy;
int iElectronP;
int iAnaIon, iMaster;
int	iCorrelation;
int	iPolarization;

double dDelay;
double dPixelSizeX, dPixelSizeY;
double dCorX, dCorY;

//double dXion, dYion;

double dT0[nHit];						// average TOF 
double dq[nHit];						// charge of ion
double dM[nHit];						// mass of ion (atomic unit)

double dCOM_x0, dCOM_y0, dCOM_z0=0;
double dCOM_x_width, dCOM_y_width, dCOM_z_width;
double dMax_TOF[6], dMin_TOF[6];
double dWidthPx, dWidthPy, dWidthPz;
double dElectric_Field, dElectric_Field2, dElectric_Field3;					// electric field (V/m)
double dMagnetic_Field;					// electric field (G)
double dElectron_T0;
double dBunch;
double dL, dL2, dL3; 								// length of ion acceleration region
double dD1;								// length of electron acceleration region
double dD2;								// length of electron drift tube 
double dMax_e_Energy;					// maximum electron energy expected (eV)
double dDelta_T_width;					// time error for the calculation of electron Pz (ns)
double dMax_eTOF;
double dMin_eTOF;
double dCOM_x_e0, dCOM_y_e0;			// center of mass of electron
double dX_cor, dY_cor;					// position correction for electron detector
double dCOM_x_e_tan;					//tangent of electron COM X  mm/ns 
double dCOM_y_e_tan;					//tangent of electron COM X  mm/ns 
double dAngle_Width;
double dPosi_cor[20];
double dJet;	//jet correction +dJet*tIon(ns)
double dCharge[nHit], dMass[nHit];

double ionEngMin, ionEngMax, elcEngMin, elcEngMax;
unsigned long lEventCounter;	// selected for Lucchesse 


/////////////////////////////////////////////////////////////////////////////
// Analysis information string

// to calculate time of flight
double tof(double p0, double xx, double mass, double charge)
{
	double p1, p2, p3, t1, t2, t3, tt;
	p1=sqrt(p0*p0+2*mass*charge*dElectric_Field*(dL-xx));
	p2=sqrt(p1*p1+2*mass*charge*dElectric_Field2*dL2);
	p3=sqrt(p2*p2+2*mass*charge*dElectric_Field3*dL3);
	t1=(p1-p0)/charge/dElectric_Field;
	t2=(p2-p1)/charge/dElectric_Field2;
	t3=(p3-p2)/charge/dElectric_Field3;
	tt=t1+t2+t3;
	return tt;
}

// to find the pz from zero z coordinate
void find_pz(const double mass, const double charge, const double TOF, double &pz)
{
	double p0, p1, p2, p3, tof0;
	double Jac;   //Jacobian matrix
	int k;
	double delta = 1.0, limit = dMomentum_au*1.0E-2;
	
	//Initial guess of pz and z
	p0 = 0.0;
		
	k = 0;
	while ((fabs(delta) > limit) && (k < 100))
	{
	   // corresponding Jacobian matrix 
      p1 = sqrt(p0*p0 + 2*mass*charge*dElectric_Field*(dL - dCOM_z0));
      p2 = sqrt(p1*p1 + 2*mass*charge*dElectric_Field2*dL2);
      p3 = sqrt(p2*p2 + 2*mass*charge*dElectric_Field3*dL3);
      tof0 = (p1 - p0)/charge/dElectric_Field 
               + (p2 -p1 )/charge/dElectric_Field2 
               + (p3 -p2 )/charge/dElectric_Field3;
      Jac  = - 1/charge/dElectric_Field 
            + p0/p1 * (1/charge/dElectric_Field - 1/charge/dElectric_Field2)
            + p0/p2 * (1/charge/dElectric_Field2 - 1/charge/dElectric_Field3)
            + p0/p3 * 1/charge/dElectric_Field3;
      
      // Calculate the delta and get new pz
      if(Jac==0) return;
      delta = - (tof0 - TOF)/Jac;
      p0 = p0 + delta;
      k = k + 1;
   }
   pz = p0;
   return;
}


// to find the pz and z for pair or triple fill the requirment of momentum conservation
void find_pz_z(const double *mass, const double *charge, const double *TOF, double &z, double *pz, const int NumIons)
{
	double p0[4]={0}, p1[4], p2[4], p3[4], z0 = 0.0, tof0[4];
	double Jac[4][4]={0}, InvJac[4][4]={0}, JacProduct = 0;   //Jacobian matrix
	int i, j, k;
	double delta[4] = {1.0, 1.0, 1.0, 1.0}, sumDeltaM, MomentumLimit = dMomentum_au*1.0E-2, PositionLimit = 1.0E-5; 
	
	k = 0;
	sumDeltaM = 0;
	for(i=0; i<NumIons-1; i++) sumDeltaM = sumDeltaM + fabs(delta[i]);
	while ((sumDeltaM > MomentumLimit) && (fabs(delta[NumIons-1]) > PositionLimit) && (k < 100))
	{
	   // corresponding Jacobian matrix 
	   // Row from 0 to NumIons-2
   	   for (i=0; i<NumIons-1; i++)
   	{
       	p1[i] = sqrt(p0[i]*p0[i] + 2*mass[i]*charge[i]*dElectric_Field*(dL-z0));
         p2[i] = sqrt(p1[i]*p1[i] + 2*mass[i]*charge[i]*dElectric_Field2*dL2);
         p3[i] = sqrt(p2[i]*p2[i] + 2*mass[i]*charge[i]*dElectric_Field3*dL3);
           tof0[i] = (p1[i] - p0[i])/charge[i]/dElectric_Field 
                  + (p2[i]-p1[i])/charge[i]/dElectric_Field2 
                  + (p3[i]-p2[i])/charge[i]/dElectric_Field3;
           Jac[i][i] = - 1/charge[i]/dElectric_Field 
                 + p0[i]/charge[i]/p1[i] * (1/dElectric_Field -  1/dElectric_Field2)
                 + p0[i]/charge[i]/p2[i] * (1/dElectric_Field2 - 1/dElectric_Field3)
                 + p0[i]/charge[i]/p3[i] *  1/dElectric_Field3;
           Jac[i][NumIons-1] = 
	   		  - mass[i]*dElectric_Field/p1[i] * (1/dElectric_Field - 1/dElectric_Field2)
                 - mass[i]*dElectric_Field/p2[i] * (1/dElectric_Field2 - 1/dElectric_Field3)
                 - mass[i]*dElectric_Field/p3[i] * 1/dElectric_Field3;
      }
       //the NumIons'th row
       i = NumIons -1;
       p1[i] = sqrt(p0[i]*p0[i] + 2*mass[i]*charge[i]*dElectric_Field*(dL-z0));
       p2[i] = sqrt(p1[i]*p1[i] + 2*mass[i]*charge[i]*dElectric_Field2*dL2);
       p3[i] = sqrt(p2[i]*p2[i] + 2*mass[i]*charge[i]*dElectric_Field3*dL3);
       tof0[i] = (p1[i] - p0[i])/charge[i]/dElectric_Field 
                + (p2[i] - p1[i])/charge[i]/dElectric_Field2 
                + (p3[i] - p2[i])/charge[i]/dElectric_Field3;
       for (j=0; j<NumIons-1; j++)
   	   {
           Jac[i][j] = 1/charge[i]/dElectric_Field 
                 + p0[j]/charge[i]/p1[i] * (1/dElectric_Field -  1/dElectric_Field2)
                 + p0[j]/charge[i]/p2[i] * (1/dElectric_Field2 - 1/dElectric_Field3)
                 + p0[j]/charge[i]/p3[i] *  1/dElectric_Field3;
       }
       Jac[i][NumIons-1] = 
	        - mass[i]*dElectric_Field/p1[i] * (1/dElectric_Field - 1/dElectric_Field2)
            - mass[i]*dElectric_Field/p2[i] * (1/dElectric_Field2 - 1/dElectric_Field3)
            - mass[i]*dElectric_Field/p3[i] * 1/dElectric_Field3;
       // Get Inversed 2-dimensional matrix directly.
       if(abs(NumIons - 2)<0.2)
       {
           JacProduct = Jac[0][0]*Jac[1][1] - Jac[0][1]*Jac[1][0];
           if(JacProduct==0) return;
           InvJac[0][0] = Jac[1][1]/JacProduct;
           InvJac[0][1] = -Jac[0][1]/JacProduct;
           InvJac[1][0] = -Jac[1][0]/JacProduct;
           InvJac[1][1] = Jac[0][0]/JacProduct;
       }
       if(abs(NumIons - 3)<0.2)
       {
           JacProduct = Jac[0][0]*Jac[1][1]*Jac[2][2] - Jac[0][2]*Jac[1][1]*Jac[2][0] - Jac[0][0]*Jac[1][2]*Jac[2][1];
           if(JacProduct==0) return;
           InvJac[0][0] = (-Jac[1][2]*Jac[2][1]+Jac[1][1]*Jac[2][2])/JacProduct;
           InvJac[0][1] = Jac[0][2]*Jac[2][1]/JacProduct;
           InvJac[0][2] = -Jac[0][2]*Jac[1][1]/JacProduct;
           InvJac[1][0] = Jac[1][2]*Jac[2][0]/JacProduct;
           InvJac[1][1] = (-Jac[0][2]*Jac[2][0]+Jac[0][0]*Jac[2][2])/JacProduct;
           InvJac[1][2] = -Jac[0][0]*Jac[1][2]/JacProduct;
           InvJac[2][0] = -Jac[1][1]*Jac[2][0]/JacProduct;
           InvJac[2][1] = -Jac[0][0]*Jac[2][1]/JacProduct;
           InvJac[2][2] = Jac[0][0]*Jac[1][1]/JacProduct;
       }
       if(abs(NumIons - 4)<0.2)
        {
           JacProduct = Jac[0][0]*Jac[1][1]*Jac[2][2]*Jac[3][3] - Jac[0][3]*Jac[1][1]*Jac[2][2]*Jac[3][0] - Jac[0][0]*Jac[1][3]*Jac[2][2]*Jac[3][1] - Jac[0][0]*Jac[1][1]*Jac[2][3]*Jac[3][2];
           if(JacProduct==0) return;
           InvJac[0][0] = (Jac[1][1]*Jac[2][2]*Jac[3][3]-Jac[1][1]*Jac[2][3]*Jac[3][2]-Jac[1][3]*Jac[2][2]*Jac[3][1])/JacProduct;
           InvJac[0][1] = Jac[0][3]*Jac[2][2]*Jac[3][1]/JacProduct;
           InvJac[0][2] = Jac[0][3]*Jac[1][1]*Jac[3][2]/JacProduct;
           InvJac[0][3] = -Jac[0][3]*Jac[1][1]*Jac[2][2]/JacProduct;
           InvJac[1][0] = Jac[1][3]*Jac[2][2]*Jac[3][0]/JacProduct;
           InvJac[1][1] = (Jac[0][0]*Jac[2][2]*Jac[3][3]-Jac[0][3]*Jac[2][2]*Jac[3][0]-Jac[0][0]*Jac[2][3]*Jac[3][2])/JacProduct;
           InvJac[1][2] = Jac[0][0]*Jac[1][3]*Jac[3][2]/JacProduct;
           InvJac[1][3] = -Jac[0][0]*Jac[1][3]*Jac[2][2]/JacProduct;
           InvJac[2][0] = Jac[1][1]*Jac[2][3]*Jac[3][0]/JacProduct;
           InvJac[2][1] = Jac[0][0]*Jac[2][3]*Jac[3][1]/JacProduct;
           InvJac[2][2] = (Jac[0][0]*Jac[1][1]*Jac[3][3]-Jac[0][3]*Jac[1][1]*Jac[3][0]-Jac[0][0]*Jac[1][3]*Jac[3][1])/JacProduct;
           InvJac[2][3] = -Jac[0][0]*Jac[1][1]*Jac[2][3]/JacProduct;
           InvJac[3][0] = -Jac[1][1]*Jac[2][2]*Jac[3][0]/JacProduct;
           InvJac[3][1] = -Jac[0][0]*Jac[2][2]*Jac[3][1]/JacProduct;
           InvJac[3][2] = -Jac[0][0]*Jac[1][1]*Jac[3][2]/JacProduct;
           InvJac[3][3] = Jac[0][0]*Jac[1][1]*Jac[2][2]/JacProduct;
        }       
       // Calculate the delta and get new pz and z
       for (i=0; i<NumIons; i++)
       {  
		   delta[i] =0; 
          for (j=0; j<NumIons; j++)
          {
             delta[i] = delta[i] - InvJac[i][j] * (tof0[j] - TOF[j]);
          }
       }
       p0[NumIons-1] = 0;
       sumDeltaM = 0;
       for (i=0; i<NumIons-1; i++)
       {  
          p0[i] = p0[i] + delta[i];
          p0[NumIons-1] = p0[NumIons-1] - p0[i];
          sumDeltaM = sumDeltaM + fabs(delta[i]);
       }
       z0 = z0 + delta[NumIons-1];
       k = k + 1;
   }
   for (i=0; i<NumIons; i++)
   {  
      *(pz+i) = p0[i];
   }
   z = z0;
   return;
}

// calculate electron tof
double electof(double p0)
{
	double p1, t1, t2, tt;
	
	p1 = sqrt(p0*p0 + 2*dElectronMass*dElectron*dElectric_Field*dD1);
	t1 = (p1-p0)/dElectron/dElectric_Field;
	t2 = dD2*dElectronMass/p1;
	tt = t1 + t2;
	return tt;
}

// calculate electron pz
void find_elec_pz(const double TOF, const double z0, double &pz)
{
	double p0, p1, tof0;
	double Jac;   //Jacobian 
	int k;
	double delta = 1.0, limit = dMomentum_au*5.0E-5;
	
	//Initial guess of pz and z
	p0 = 0.0;
		
	k = 0;
	while ((fabs(delta) > limit) && (k < 100))
	{
	   // corresponding Jacobian matrix 
      p1 = sqrt(p0*p0+2*dElectronMass*dElectron*dElectric_Field*(dD1-z0));
      tof0 = (p1-p0)/dElectron/dElectric_Field + dD2*dElectronMass/p1;
      Jac  = 1.0/dElectron/dElectric_Field * 
            ( (p0*p0*p0 + dElectronMass*dElectron*dElectric_Field*p0*(2*(dD1-z0)-dD2))/(p1*p1*p1) - 1);
      
      // Calculate the delta and get new pz
      delta = - (tof0 - TOF)/Jac;
      p0 = p0 + delta;
      k = k + 1;
   }
   //throw out the event that electron pass the mesh at ion side
   if( p0 < - sqrt(2*dElectronMass*dElectric_Field*(dL+z0)*dElectron)) p0 = -10000;
   pz = p0;
   return;
}

CDAN_API LPCTSTR AnalysisGetInformationString()
{
	return LPCTSTR("CAMAC for CoboldPC 6.2.0.81");
}

/////////////////////////////////////////////////////////////////////////////
// Analysis functions

CDAN_API BOOL AnalysisInitialize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
	if(pParameters->GetSize()<16)						// if less than 15 parameters then show error
		return false;

	iNumberOfChannels	= (int)(pParameters->GetAt(32-1)+0.1);
	iNumberOfHits		= (int)(pParameters->GetAt(33-1)+0.1);

	// Get Coordinate Parameter

	iStartTDC			= (int)(pParameters->GetAt(110-1)-0.9);
	iEventCounter		= (int)(pParameters->GetAt(112-1)-0.9);     // - 1 because coordinate
	lEventCounter		= 0;									    // reset EventCounter
	iPos				= (int)(pParameters->GetAt(113-1)-0.9);     // - 1 because coordinate
	iHitPattern			= (int)(pParameters->GetAt(117-1)-0.9);     // Match & MO
	
	itIon				= (int)(pParameters->GetAt(120-1)-0.9);
	iIonEnergy			= (int)(pParameters->GetAt(121-1)-0.9);
	iIonP				= (int)(pParameters->GetAt(122-1)-0.9);
	icom				= (int)(pParameters->GetAt(123-1)-0.9);
	itElectron			= (int)(pParameters->GetAt(124-1)-0.9);
	iElectronEnergy		= (int)(pParameters->GetAt(125-1)-0.9);
	iElectronP			= (int)(pParameters->GetAt(126-1)-0.9);
	iMaster				= (int)(pParameters->GetAt(127-1)-0.9);
	iCorrelation		= (int)(pParameters->GetAt(128-1)-0.9);
	iPolarization		= (int)(pParameters->GetAt(137-1)+0.1);
 	dPixelSizeX			= pParameters->GetAt(147-1);
 	dPixelSizeY			= pParameters->GetAt(148-1);
 	dCorX				= pParameters->GetAt(149-1);
 	dCorY				= pParameters->GetAt(150-1);


	dT0[0]				= 1.0E-9 * pParameters->GetAt(180-1);
	dT0[1]				= 1.0E-9 * pParameters->GetAt(181-1);
	dT0[2]				= 1.0E-9 * pParameters->GetAt(182-1);
	dq[0]				= pParameters->GetAt(183-1);
	dq[1]				= pParameters->GetAt(184-1);
	dq[2]				= pParameters->GetAt(185-1);
	dM[0]				= pParameters->GetAt(186-1);
	dM[1]				= pParameters->GetAt(187-1);
	dM[2]				= pParameters->GetAt(188-1);
	iAnaIon				= (int)(pParameters->GetAt(189-1)+0.1);

	dMin_TOF[0]			= 1.0E-9 * pParameters->GetAt(190-1);
 	dMax_TOF[0]			= 1.0E-9 * pParameters->GetAt(191-1);
 	dMin_TOF[1]			= 1.0E-9 * pParameters->GetAt(192-1);
 	dMax_TOF[1]			= 1.0E-9 * pParameters->GetAt(193-1);
 	dMin_TOF[2]			= 1.0E-9 * pParameters->GetAt(194-1);
 	dMax_TOF[2]			= 1.0E-9 * pParameters->GetAt(195-1);

	dCOM_x0				= 0.001 * pParameters->GetAt(196-1);
	dCOM_y0				= 0.001 * pParameters->GetAt(197-1);
	dCOM_x_width		= 0.001 * pParameters->GetAt(198-1);
	dCOM_y_width		= 0.001 * pParameters->GetAt(199-1);
	dCOM_z_width		= 0.001 * pParameters->GetAt(200-1);		// m

	dWidthPx			= pParameters->GetAt(201-1);
	dWidthPy			= pParameters->GetAt(202-1);
	dWidthPz			= pParameters->GetAt(203-1);


	dL2					= 0.001 * pParameters->GetAt(208-1);		// length of ion acceleration region
	dL3					= 0.001 * pParameters->GetAt(209-1);		// length of ion acceleration region

	dL					= 0.001 * pParameters->GetAt(210-1);		// length of ion acceleration region
	dD1					= 0.001 * pParameters->GetAt(211-1);		// m
	dD2					= 0.001 * pParameters->GetAt(212-1);		// m

	dElectric_Field		= pParameters->GetAt(213-1);				//V/m
	dMagnetic_Field		= 1.0E-4 * pParameters->GetAt(214-1);		// Tesra
	dBunch				= 1.0E-9 * pParameters->GetAt(215-1); 				// sec

	dAngle_Width		= pParameters->GetAt(216-1);				// width of plane angle (degree)

	dElectric_Field2		= pParameters->GetAt(217-1);			//V/m
	dElectric_Field3		= pParameters->GetAt(218-1);			//V/m


	dDelay				= 1.0e-9 * pParameters->GetAt(220-1);		// (sec) electron zero point
	dElectron_T0		= 1.0E-9 * pParameters->GetAt(221-1);		// electron TOF with 0 energy
	dMax_e_Energy		= pParameters->GetAt(222-1);
	//dDelta_T_width		= 1.0E-9 * pParameters->GetAt(223-1);		// nsec
	dDelta_T_width		= pi/180.0 * pParameters->GetAt(223-1);		// nsec
	dMin_eTOF			= 1.0E-9 * pParameters->GetAt(224-1);		// nsec
	dMax_eTOF			= 1.0E-9 * pParameters->GetAt(225-1);		// nsec
	dX_cor				= pParameters->GetAt(226-1);				// position correction for electron
	dY_cor				= pParameters->GetAt(227-1);				// position correction for electron
	dCOM_x_e0			= 1.0E-3 * pParameters->GetAt(228-1);			// m
	dCOM_y_e0			= 1.0E-3 * pParameters->GetAt(229-1);			// m
	dCOM_x_e_tan		= 1.0E-3 * 1.0E9 * pParameters->GetAt(230-1);	// m/s
	dCOM_y_e_tan		= 1.0E-3 * 1.0E9 * pParameters->GetAt(231-1);
	dJet				= pParameters->GetAt(240-1);
	
	//for Lucchesse
	ionEngMin           = pParameters->GetAt(243-1)*dElectron;
	ionEngMax           = pParameters->GetAt(244-1)*dElectron;
	elcEngMin           = pParameters->GetAt(245-1)*dElectron;
	elcEngMax           = pParameters->GetAt(246-1)*dElectron;

	for(int iHit=0;iHit<iAnaIon;iHit++){
		dCharge[iHit] = dq[iHit]*dElectron;
		dMass[iHit] = dM[iHit]*dMassUnit;
	}

	if(pEventData->GetSize() < 80)
	return false;  // 16 channels with 1 timesignal and 4 anodesiganl makes 16*5 = 80

	fp1 = fopen("com.dat","wt");
	double tc1= tof(0, 0, dMass[0], dCharge[0]);
	double tc2= tof(0, 0, dMass[1], dCharge[1]);
	fprintf(fp1, "%12.10f  %12.10f", tc1,tc2);
   fclose(fp1);
   
	fp_cor = fopen("angle_cor.dat","rt");
	double ang;
	for(int i=0;i<19;i++)
	{
		dPosi_cor[i] = 1.0;
		fscanf(fp_cor, "%lf %lf", &ang,&dPosi_cor[i]);
	}
	fclose(fp_cor);
//	for(i=0;i<19;i++)
//	{
//		fprintf(fp1, "%lf \n", dPosi_cor[i]);
//	}
    fp_sel = fopen("SelLuc.dat","wb");
    
	return TRUE;
}

CDAN_API void AnalysisProcessEvent(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
	// for Hexagonal anode
	double xhex[nHit], yhex[nHit];
	//double dOriginElectron, 
	double dtElectron[nHit];
	
	// for Square anode
	double dxSq[nHit], dySq[nHit], dxSqN[2][nHit], dySqN[2][nHit];
	double dx0, dy0;
	double dtIon[nHit], dtIonN[2][nHit], dOriginIon;
	
	// for hitpattern
	int nHexX1 = 0, nHexX2 = 0, nHexY1 = 0, nHexY2 = 0, nHexZ1 = 0, nHexZ2 = 0, nMCPele = 0, nMCPion = 0;
	int nSqX1 = 0, nSqX2 = 0, nSqY1 = 0, nSqY2 = 0, nBunch = 0;

	int iHitsTDC1 = iNumberOfHits;			// for electron
	int iHitsTDC2 = iNumberOfHits;			// for ion
	int iTDC, iCH;
	int iHit;
	int e_mcp = 6, i_mcp = 4, jjj, jn;		// NEW
	int flagmatrix = 0;
	int iMaster_Flag = 0;           //set the credibility of the event
	int e_region =0;						// cut out detecter
	int iSTOP = 6;				// address for STOP signal of Hoshin-TDC
	int iCount;
	int iangle_cor;
	int isellect=-1;	//1:first ion is N+, 2: first ion is O+, 
						//3:difficult to determine(N+), 4:difficult to determine(O+)

	double a[8][8];				// for electron
	double b[8][8];				// for ion

	double dSum_Of_mt = 0;
	double dSum_Of_mxt = 0;
	double dSum_Of_myt = 0;
	double dCOM_x[nHit], dCOM_y[nHit], dCOM_z[nHit];
	double square_P = -1000;
	double dIonPx[2][nHit], dIonPy[2][nHit], dIonPz[2][nHit], dIonP[nHit];		// Components of momentum of Ion
	double dIonPx_norm[nHit], dIonPy_norm[nHit], dIonPz_norm[nHit];				// Normalized momentum of Ion
	double dElectronPx[nHit], dElectronPy[nHit], dElectronPz[nHit], dElectronP[nHit]; // Components of momentum of Electron
	double dElectronPx_norm[nHit], dElectronPy_norm[nHit], dElectronPz_norm[nHit];	  // Normalized momenta
	double dIon_Total_Energy = -10000;
	double dIonEnergy[nHit];
	double dElectronEnergy[nHit];
	double dIon_theta = -10000, dIon_psai_xy = -10000, dIon_psai_zy = -10000, dIon_psai_zx = -10000;
	double dSum_of_momenta_X[nHit], dSum_of_momenta_Y[nHit], dSum_of_momenta_Z[nHit], dSum_of_momenta[nHit];
	double dIon_nPx = -10000, dIon_nPy = -10000, dIon_nPz = -10000;
	double dElectron_theta[nHit], dElectron_psai[nHit];
	double dEX = -1000, dEY = -1000, dalpha = -1000, dphai = -1000, dConst = -1000, dTcyc = -1000, dalpha1 = -1000;
	double c0 = -1000, c1 = -1000, an = -1000, sqrtA = -1000, tc0 = -1000, tc1 = -1000;
	double distancexy = -1000, dDelta_T = -1000, dDelta_Tn = -1000, dW1 = -1000, dW2 = -1000, dWn = -1000;
	double sq_2mqe = -1000, sq_dw = -1000, sq_w = -1000, sq_d1 = -1000;
	double dtheta_e_Ion = -1000, dElectron_psai_position[nHit];
	double dCOM_x_e, dCOM_y_e;
	double dIon_Angle[100], dElectron_Angle[100], dIon_Electron_Angle[100];
	double d_I_Angle = -10000;
	double d_e_I_Angle = -10000;
	double dElectron_psai_xy = -1000, dElectron_psai_zy = -1000, dElectron_psai_zx = -1000, dElectron_posi_xy = -1000;
	double dElectron_theta0[nHit], dElectronEnergyXY;
	double dIon_Electron_Angle0=-1000, dIon_Angle0=-1000, dElectron_Angle0=-1000;
	double dtN=-1000,dtO=-1000;

	// for CAMAC Hoshin-TDC
	double TDC[nTDC][nCH][nHit+1];


	// Initialize variables
	for(jjj=0;jjj<100;jjj++){
		dIon_Angle[jjj] = -1000;
		dElectron_Angle[jjj] = -2000;
		dIon_Electron_Angle[jjj] = -10000;
	}
  

	for(iTDC=0;iTDC<nTDC;iTDC++){
		for(iCH=0;iCH<nCH;iCH++){	
			for(iHit=0;iHit<nHit;iHit++){
				TDC[nTDC][nCH][nHit+1] = -10000;
			}
		}
	}


	jn = 0;
	for(iHit=0;iHit<nHit;iHit++){
		xhex[iHit] = -10000;
		yhex[iHit] = -10000;
		dtIon[iHit] = -10000;
		dtElectron[iHit] = -10000;
		dxSq[iHit] = -10000;
		dySq[iHit] = -10000;
		dElectronPx[iHit] = -10000;
		dElectronPy[iHit] = -10000;
		dElectronPz[iHit] = -10000;
		dElectronEnergy[iHit] = -10000;
		dIonPx_norm[iHit] = -10000;
		dIonPy_norm[iHit] = -10000;
		dIonPz_norm[iHit] = -10000;
		dElectronPx_norm[iHit] = -10000;
		dElectronPy_norm[iHit] = -10000;
		dElectronPz_norm[iHit] = -10000;
		dElectron_theta[iHit] = -10000;
		dElectron_theta0[iHit] = -10000;
		dElectron_psai[iHit] = -10000;
		dElectron_psai_position[iHit] = -10000;
		dtElectron[iHit] = -1000;
		for(int i=0; i<7; i++)
		{
			a[i][iHit] = -10000;
		}
	}
	for(jjj=0;jjj<2;jjj++){
		for(iHit=0;iHit<4;iHit++){
			dIonPx[jjj][iHit] = -10000;
			dIonPy[jjj][iHit] = -10000;
			dIonPz[jjj][iHit] = -10000;
			dxSqN[jjj][iHit] = -10000;
			dySqN[jjj][iHit] = -10000;
			dtIonN[jjj][iHit] = -10000;
		}
		dCOM_x[jjj] = -10000;
		dCOM_y[jjj] = -10000;
		dCOM_z[jjj] = -10000;
		dSum_of_momenta_X[jjj] = -10000;
		dSum_of_momenta_Y[jjj] = -10000;
		dSum_of_momenta_Z[jjj] = -10000;
		dSum_of_momenta[jjj] = -10000;
	}

///////////////////////////////////////////////////////////////////////////////////////////

	int ihit;

	for(iTDC=0;iTDC<nTDC;iTDC++)
	{
		for(iCH=0;iCH<nCH;iCH++)
		{	
			for(ihit=0;ihit<nHit+1;ihit++)
			{
				TDC[iTDC][iCH][ihit]  = pEventData->GetAt(ihit+iCH*(nHit+1)+iTDC*(nHit+1)*nCH)/1000.0;
			}
		}
	}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Make sure how many hits come to each channeles of each TDCs (if TAC Rawdata 
// values are not equal to 2048 , it means there is signal)
/////////////////////////////////////////////////////////////////////////////
	for(iHit=0;iHit<nHit;iHit++){
		if(TDC[3][0][iHit]<15000) nMCPele += 1;
		if(TDC[3][1][iHit]<15000) nMCPion += 1;
	}
	iHitsTDC1 = (nMCPele <= iHitsTDC1) ? nMCPele : iHitsTDC1;
	iHitsTDC2 = (nMCPion <= iHitsTDC2) ? nMCPion : iHitsTDC2;


	if((nMCPele==0) || (nMCPion==0))  //If the number of electrons and ions is not within requirement
	{
		iMaster_Flag = -10;
		flagmatrix = 1;
		goto hell_2;
	}




/////////////////////////////////////////////////////////////////
//////////////////////  Analysis  for ION ///////////////////////
/////////////////////////////////////////////////////////////////

	dOriginIon = TDC[6][0][0]+dBunch;		// Interval of bunch -> 57 ns //114 is just only trial

	// Inversion of time direction
	// First ion defines time-axis-origin in time
	// nHit+1->nHit
	for(iHit=0;iHit<nHit;iHit++){
		b[0][iHit] = TDC[4][0][iHit];		// x1 (ns)
		b[1][iHit] = TDC[4][1][iHit];		// x2 (ns)
		b[4][iHit] = TDC[3][1][iHit];		// MCP (ns)
	}  //because the logic of read data is different from data be stored in resort, some strange sentences are used

//ion TOF
	for(iHit=0;iHit<iAnaIon;iHit++) 
		dtIon[iHit] = (b[4][iHit])*(1.0E-9) - dDelay;	// unit sec
	dtN=dtIon[0];
	dtO=dtIon[1];
	
// Now do calculates for the ion's position of all hits
	for(iHit=0;iHit<iAnaIon;iHit++){
		dx0 = b[0][iHit]-100;
		dy0 = b[1][iHit]-100;
		dxSq[iHit] = 1.0E-3*((dx0+dCorX*dy0)*dPixelSizeX);			// Unit Meter
		dySq[iHit] = 1.0E-3*((dy0+dCorY*dx0)*dPixelSizeY + dJet*dtIon[iHit]/1.0E-9);	// Unit Meter
        //dXion = dxSq[0];
        //dYion = dySq[0];
	}


// Calculation of ion time ordered as y
// Require 2 hits for ion
//--------------------------------------------------------------------------
	if(iHitsTDC2<iAnaIon) // make sure the number of ions is larger than required
	{
		iMaster_Flag = -10;
		flagmatrix = 3;
		goto hell; //
	}

	for(iHit=0;iHit<iAnaIon;iHit++)  //
	{
		if(dtIon[iHit]<dMin_TOF[iHit])
		{
			iMaster_Flag = -10;
			flagmatrix = 4;
		}
		if(dtIon[iHit]>dMax_TOF[iHit])
		{
			iMaster_Flag = -10;
			flagmatrix = 5;
		}
	}

	if(iMaster_Flag<0) goto hell; // select the ion from Time of Flight. 

	isellect=0;


	// Calculation of ion momenta 
	// --------------------------------------------------------
//-----------------------------------------------------mistake iHitTDC2 
	for(iHit=0;iHit<iAnaIon;iHit++){
		dtIonN[0][iHit] = dtIon[iHit];
		dxSqN[0][iHit] = dxSq[iHit];
		dySqN[0][iHit] = dySq[iHit];
	}
	dtIonN[1][0] = dtIon[1];
	dxSqN[1][0] = dxSq[1];
	dySqN[1][0] = dySq[1];
	dtIonN[1][1] = dtIon[0];
	dxSqN[1][1] = dxSq[0];
	dySqN[1][1] = dySq[0];

   //double tempsqmd[2];   //get calculated p0z0 tof
   //double tempdtIonN[2];
   //tempsqmd[0] = tof(0, 0, dMass[0], dCharge[0]);
   //tempsqmd[1] = tof(0, 0, dMass[1], dCharge[1]);
	for(jjj=0;jjj<2;jjj++)
	{
      // Calculation of Pz
		dSum_of_momenta_Z[jjj] = 0;
		for(iHit=0;iHit<iAnaIon;iHit++)
		{	
			//double dTT0 = dtIonN[jjj][iHit]-dT0[iHit];
			//double sqmd = sqrt(2.0*dMass[iHit]*dL/dCharge[iHit]/dElectric_Field);
			//tempsqmd[i] = sqmd;
			//dIonPz[jjj][iHit] = -dCharge[iHit]*dElectric_Field*dTT0*(dTT0+2.0*sqmd)/2.0/(dTT0+sqmd);
			find_pz(dMass[iHit], dCharge[iHit], dtIonN[jjj][iHit], dIonPz[jjj][iHit]);
			dSum_of_momenta_Z[jjj] = dSum_of_momenta_Z[jjj]+dIonPz[jjj][iHit];
		}

//		c0 = dT0[0]-sqrt(2.0*dMass[0]*dL/dCharge[0]/dElectric_Field);
//		c1 = dT0[1]-sqrt(2.0*dMass[1]*dL/dCharge[1]/dElectric_Field);
//		tc0 = dtIonN[jjj][0]-c0;
//		tc1 = dtIonN[jjj][1]-c1;
//		sqrtA = fabs(dElectric_Field/sqrt(8.0)*(dCharge[0]*tc0*tc0/dMass[0]-dCharge[1]*tc1*tc1/dMass[1])
//			/(tc0/dMass[0]+tc1/dMass[1]));
//		an = 1.0;
//		if(dtIonN[jjj][0]>dT0[0]) an = -1.0;
//		dIonPz[jjj][0] = an*sqrt(2.0) * sqrtA;
//		dIonPz[jjj][1] = -dIonPz[jjj][0];
//		dCOM_z[jjj] = tc0/2.0/dMass[0]*(dCharge[0]*dElectric_Field*tc0+an*sqrt(8.0)*sqrtA)-dL;

       find_pz_z(dMass, dCharge, dtIonN[jjj], dCOM_z[jjj], dIonPz[jjj], iAnaIon);

		dSum_Of_mt	= 0;
		dSum_Of_mxt = 0;
		dSum_Of_myt = 0;

		for(iHit=0;iHit<iAnaIon;iHit++)
		{
			dSum_Of_mt = dSum_Of_mt+dMass[iHit]/dtIonN[jjj][iHit];                    
      dSum_Of_mxt = dSum_Of_mxt+dxSqN[jjj][iHit]*dMass[iHit]/dtIonN[jjj][iHit]; 
      dSum_Of_myt = dSum_Of_myt+dySqN[jjj][iHit]*dMass[iHit]/dtIonN[jjj][iHit]; 
		}
		
		dCOM_x[jjj] = dSum_Of_mxt/dSum_Of_mt;		// Position of Center_of_Mass of the molecule
		dCOM_y[jjj] = dSum_Of_myt/dSum_Of_mt;		// (or you can call it	<Point of Production>)		

		dSum_of_momenta_X[jjj] = 0;
		dSum_of_momenta_Y[jjj] = 0;
		for(iHit=0;iHit<iAnaIon;iHit++)
		{
			dIonPx[jjj][iHit] = dMass[iHit]/dtIonN[jjj][iHit]*(dxSqN[jjj][iHit]-dCOM_x0);
			dIonPy[jjj][iHit] = dMass[iHit]/dtIonN[jjj][iHit]*(dySqN[jjj][iHit]-dCOM_y0);
			dSum_of_momenta_X[jjj] = dSum_of_momenta_X[jjj]+dIonPx[jjj][iHit];
			dSum_of_momenta_Y[jjj] = dSum_of_momenta_Y[jjj]+dIonPy[jjj][iHit];
			dIonPx[jjj][iHit] = dMass[iHit]/dtIonN[jjj][iHit]*(dxSqN[jjj][iHit]-dCOM_x[jjj]);
			dIonPy[jjj][iHit] = dMass[iHit]/dtIonN[jjj][iHit]*(dySqN[jjj][iHit]-dCOM_y[jjj]);
		}
	}  //end of jjj

	jjj=0;
	// If the position or momentum of ion-pair is out of range
	if(fabs(dCOM_x[jjj]-dCOM_x0)>dCOM_x_width)   
	{
		flagmatrix = 5;
		iMaster_Flag = -5;
		goto ion1;
	}
	if(fabs(dCOM_y[jjj] - dCOM_y0) > dCOM_y_width){
		flagmatrix = 6;
		iMaster_Flag = -5;
		goto ion1;
	}
	if(fabs(dCOM_z[jjj]) > dCOM_z_width){
		flagmatrix = 7;
		iMaster_Flag = -5;
		goto ion1;
	}
	if(fabs(dSum_of_momenta_X[jjj]) / dMomentum_au > dWidthPx){
		flagmatrix = 8;
		iMaster_Flag = -5;
		goto ion1;
	}
	if(fabs(dSum_of_momenta_Y[jjj]) / dMomentum_au > dWidthPy){
		flagmatrix = 9;
		iMaster_Flag = -5;
		goto ion1;
	}
	if(fabs(dSum_of_momenta_Z[jjj]) / dMomentum_au > dWidthPz){
		flagmatrix = 10;
		iMaster_Flag = -5;
		goto ion1;
	}

ion1:
	jjj=1;
	if(fabs(dCOM_x[jjj] - dCOM_x0) > dCOM_x_width){
		flagmatrix = 11;
		iMaster_Flag = iMaster_Flag-10;
		goto ion2;
	}
	if(fabs(dCOM_y[jjj] - dCOM_y0) > dCOM_y_width){
		flagmatrix = 12;
		iMaster_Flag = iMaster_Flag-10;
		goto ion2;
	}
	if(fabs(dCOM_z[jjj]) > dCOM_z_width){
		iMaster_Flag = iMaster_Flag-10;
		flagmatrix = 13;
		goto ion2;
	}
	if(fabs(dSum_of_momenta_X[jjj]) / dMomentum_au > dWidthPx){
		iMaster_Flag = iMaster_Flag-10;
		flagmatrix = 14;
		goto ion2;
	}
	if(fabs(dSum_of_momenta_Y[jjj]) / dMomentum_au > dWidthPy){
		flagmatrix = 15;
		iMaster_Flag = iMaster_Flag-10;
		goto ion2;
	}
	if(fabs(dSum_of_momenta_Z[jjj]) / dMomentum_au > dWidthPz){
		flagmatrix = 16;
		iMaster_Flag = iMaster_Flag-10;
		goto ion2;
	}

ion2:
	if(iMaster_Flag < -12 )
	{
		goto hell;
	}
	if(iMaster_Flag == -10 ){
		flagmatrix = 30;
		jn=0;
		isellect=1;
	}
	if(iMaster_Flag == -5 ){
		flagmatrix = 31;
		jn=1;
		isellect=2;
	}
	if(iMaster_Flag > -2 )
	{
		flagmatrix = 32;
		isellect=3;
		for(jjj=0; jjj<2; jjj++){
 			//dSum_of_momenta[jjj] = dSum_of_momenta_Y[jjj]*dSum_of_momenta_Y[jjj] + dSum_of_momenta_X[jjj]*dSum_of_momenta_X[jjj];
			dSum_of_momenta[jjj] = dSum_of_momenta_Z[jjj]*dSum_of_momenta_Z[jjj];
		}
		jn=0;
		if(dSum_of_momenta[0] > dSum_of_momenta[1]) 
		{
			jn=1;
			isellect=4;
		}
	}
	iMaster_Flag = 0;
    
    //Here we definitely know that CO+ arrive detector later than O+ does
    //jn = 0;
    //isellect = 1;
    
	if(jn==1)
	{
		double dtp=dtN;
		dtN=dtO;
		dtO=dtp;
	}

// end of check ion 
	for(iHit=0; iHit<iAnaIon; iHit++){
		square_P = dIonPx[jn][iHit]*dIonPx[jn][iHit]+dIonPy[jn][iHit]*dIonPy[jn][iHit]+dIonPz[jn][iHit]*dIonPz[jn][iHit];
			dIonP[iHit]= sqrt(square_P);	  // Total Momentum of Ion
			dIonPx_norm[iHit] = dIonPx[jn][iHit]/dIonP[iHit];	// Normalized
			dIonPy_norm[iHit] = dIonPy[jn][iHit]/dIonP[iHit];	// momentum-vectors
			dIonPz_norm[iHit] = dIonPz[jn][iHit]/dIonP[iHit];
	} // END for(iHit=0; iHit < iAnaIon; iHit++)

	if(iMaster_Flag < 0) goto hell;

	// Calculation of ion kinetic energies
	// -------------------------------------------------------------------------------
	dIon_Total_Energy = 0;
	for(iHit=0; iHit<iAnaIon; iHit++){
		dIonEnergy[iHit] = 0.5*dIonP[iHit]*dIonP[iHit]/dMass[iHit];
		dIon_Total_Energy = dIon_Total_Energy+dIonEnergy[iHit];
	}

	// Calculation of molecular direction
	// ------------------------------------
	dIon_nPx = dIonPx[jn][0]-dIonPx[jn][1];	
	dIon_nPy = dIonPy[jn][0]-dIonPy[jn][1];	
	dIon_nPz = dIonPz[jn][0]-dIonPz[jn][1];	
	square_P = dIon_nPx*dIon_nPx+dIon_nPy*dIon_nPy+dIon_nPz*dIon_nPz;
	if(square_P > 0){
		square_P = sqrt(square_P);			// Total Momentum of Ion
		dIon_nPx = dIon_nPx/square_P ;		// Normalized
		dIon_nPy = dIon_nPy/square_P ;		// Normalized
		dIon_nPz = dIon_nPz/square_P ;		// Normalized
	} else {
		flagmatrix = 42;
		iMaster_Flag = -10;	// square_P must not be zero !! kill this event !
	}
		
	if(iMaster_Flag < 0) goto hell;

//	Horizonatal polarization
	if(iPolarization == 0){
		dIon_theta = acos(dIon_nPz);  // Angle between Ion and spectrometer (Z-axis))
	}
//	Vertical polarization
	if(iPolarization == 1){
		dIon_theta = acos(dIon_nPy);  // Angle between Ion and E field (Y), polar angle
	}

	if((dIon_nPy == 0) && (dIon_nPx == 0)){
		dIon_psai_xy=-1000;
	} else {
		dIon_psai_xy= atan2(dIon_nPy ,dIon_nPx);
	}

	if((dIon_nPz == 0) && (dIon_nPx == 0)) {
		dIon_psai_zx=-1000;
	} else {
		dIon_psai_zx= atan2(dIon_nPx ,dIon_nPz);
	}

	if((dIon_nPy == 0) && (dIon_nPz == 0)) {
		dIon_psai_zy=-1000;
	} else {
		dIon_psai_zy= atan2(dIon_nPy ,dIon_nPz);
	}

hell:	//this is just a label
//////////////////////////////////////////////////////////////////////////////////
//////////// Analysis for electron ///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

	// Deduce the origin of ion by checking the bunch marker
	//	dOriginElectron = dOriginIon;

	for(iHit=0;iHit<nHit;iHit++){
		a[0][iHit] = TDC[0][0][iHit];		// x1 (ns)
		a[1][iHit] = TDC[0][1][iHit];		// x2 (ns)
		a[6][iHit] = TDC[3][0][iHit];		// MCP (ns)
	}
	

//electron TOF
	for(iHit=0;iHit<nHit;iHit++) 
	{
		dtElectron[iHit] = (a[6][iHit])*(1.0E-9) ;	// unit sec
        //dtElectron[iHit] = dtElectron[iHit] - dDelay;
	}
// Calculation of POSITIONS in meters via old method (just for checks):
// ------------------------------------------------------------------------
	//for(iHit=0;iHit<nHit;iHit++)
	//{
		
		//HEXAGONAL to SQUARE
		for(iCount=0;iCount<nHit;iCount++)
		{
			xhex[iCount]    = (a[0][iCount]-100) / 1000; //unit meter
			yhex[iCount]    = (a[1][iCount]-100) / 1000;
			
		}
		
	//}
	
	  //rotation due to BIGHEX configuration
      for(iCount=0;iCount<nHit;iCount++)
      {
        double ang_shift;
        double xhextemp, yhextemp;
        ang_shift = 30.0/180.0*3.1415926;
        xhextemp = xhex[iCount];
        yhextemp = yhex[iCount];
        xhex[iCount] =  xhextemp*cos(ang_shift) + yhextemp*sin(ang_shift);
        yhex[iCount] = -xhextemp*sin(ang_shift) + yhextemp*cos(ang_shift);
      }

		for(iCount=0;iCount<nHit;iCount++)
		{
			xhex[iCount]    = xhex[iCount] * dX_cor;
			yhex[iCount]    = yhex[iCount] * dY_cor;
		}
			
	// selectrion of valid delay line pair
    
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//**************************************‚±‚ê‚æ‚èã*********************************************************************
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// selection of electron TOF, the first electron
	for(iHit=0; iHit<1; iHit++)
	{
   	    if((dtElectron[iHit]< dMin_eTOF) || (dtElectron[iHit]> dMax_eTOF))
   	    {
   	    	iMaster_Flag = -20;			
   	    	flagmatrix = 50;
   	    	goto hell_2;
   	    }
	}
////  Calculation of Electron momenta


//	for(iCount=0; iCount<iHitsTDC1; iCount++)
    double dtElectron0;
	dtElectron0 = electof(0);
	for(iCount=0; iCount<1; iCount++)
	{	

//   calculation of Pz
        dDelta_T = dtElectron[iCount] - dElectron_T0 + dtElectron0;
		find_elec_pz(dDelta_T, -dCOM_z[jn], dElectronPz[iCount]);
		if (dElectronPz[iCount] < -9900) 
		{
		  iMaster_Flag = -20;
		  goto hell_2;
		}
// calculation of Px, Py
		dCOM_x_e = dCOM_x_e0 + dCOM_x_e_tan * dDelta_T;
		dCOM_y_e = dCOM_y_e0 + dCOM_y_e_tan * dDelta_T;
		if(iMaster_Flag < 0) 
		{
			dEX=xhex[iCount] - dCOM_x_e;
			dEY=yhex[iCount] - dCOM_y_e;
		}
		else
		{
//			dEX=xhex[iCount] - dCOM_x_e;
//			dEY=yhex[iCount] - dCOM_y_e;
			dEX=xhex[iCount] - dCOM_x_e - (-dCOM_x[jn]+dCOM_x0);
			dEY=yhex[iCount] - dCOM_y_e - (dCOM_y[jn]-dCOM_y0);
		}

		
			if((atan2(dEY,dEX)*180/pi <120) && (atan2(dEY,dEX)*180/pi >60)) ///////////////////////////////now modifying 2003.8.11////////////////////////////////////cut out detector region
			{
				e_region = 1;
			}
		
		//angle correction, and make interpolation		
		dElectron_posi_xy= atan2(dEY,dEX)*180.0/pi;
		if(dElectron_posi_xy < 0) dElectron_posi_xy = dElectron_posi_xy + 360.;
		iangle_cor=(int) (dElectron_posi_xy/20);
		double angle_int_cor = dPosi_cor[iangle_cor] + (dElectron_posi_xy 
		   - (double) iangle_cor * 20.0) / 20.0 * (dPosi_cor[iangle_cor+1] - dPosi_cor[iangle_cor] );
		dEX = dEX*angle_int_cor;
		dEY = dEY*angle_int_cor;


		distancexy = sqrt( dEX*dEX + dEY*dEY );
		//distancexy = 10;

		if(dMagnetic_Field  > 1.0e-8 )
		{
			dTcyc = 2.0 * pi * dElectronMass / dMagnetic_Field / dElectron;
			//double dalpha2 = (dtElectron[iCount] - dDelay)/dTcyc;
			dalpha1 = dDelta_T/dTcyc;
			dalpha = (dalpha1 - int(dalpha1))*2.0*pi;
			dphai = - dalpha * 0.5;
			dConst = fabs(dElectron * dMagnetic_Field / sin(dphai) / 2);
			dElectronPx[iCount] = dConst * (cos(dphai)*dEX + sin(dphai)*dEY);
			dElectronPy[iCount] = dConst * (-sin(dphai)*dEX + cos(dphai)*dEY);
		}
		else
		{
			//dElectronPx[iCount] = dElectronMass * dEX / (dtElectron[iCount] - dDelay);
			//dElectronPy[iCount] = dElectronMass * dEY / (dtElectron[iCount] - dDelay);
			dElectronPx[iCount] = dElectronMass * dEX / dDelta_T;
			dElectronPy[iCount] = dElectronMass * dEY / dDelta_T;
		}

		square_P = dElectronPx[iCount]*dElectronPx[iCount] + dElectronPy[iCount]*dElectronPy[iCount] + dElectronPz[iCount]*dElectronPz[iCount];

		if(square_P > 0)
		{
			dElectronP[iCount]= sqrt(square_P);    // Total Momentum of Electron
			dElectronPx_norm[iCount]	= dElectronPx[iCount] / dElectronP[iCount];	// Normalized
			dElectronPy_norm[iCount]	= dElectronPy[iCount] / dElectronP[iCount];	// momentum-vectors
			dElectronPz_norm[iCount]	= dElectronPz[iCount] / dElectronP[iCount];
		} 
		else	
		{
			flagmatrix = 26;
			iMaster_Flag = -20;	// square_P must not be zero !! kill this event !
			goto hell_2;
		}
		
	} // END for(iCount=0; iCount < iHItsTDC1; iCount++)

    flagmatrix = 65;
// Calculation of electron kinetic energies
//	for(iHit=0; iHit<iHitsTDC1; iHit++){

	for(iHit=0; iHit<1; iHit++)
	{
		dElectronEnergy[iHit] = 0.5*dElectronP[iHit]*dElectronP[iHit]/dElectronMass;
		dElectronEnergyXY = 0.5*(dElectronPx[iHit]*dElectronPx[iHit] + dElectronPy[iHit]*dElectronPy[iHit] )/dElectronMass;
	}
	
	// Calculation of electron angles
//	for(iHit=0; iHit<iHitsTDC1; iHit++){
	for(iHit=0; iHit<1; iHit++)
	{
		dElectron_theta0[iHit] = acos(dElectronPz_norm[iHit]);  
		dElectron_psai[iHit]= atan2(dElectronPy_norm[iHit],dElectronPx_norm[iHit]);
		dElectron_psai_xy= atan2(dElectronPy_norm[iHit],dElectronPx_norm[iHit]);
		//if(iPolarization==1){dElectron_psai_xy= atan2(dElectronPx_norm[iHit],dElectronPy_norm[iHit]);}
		dElectron_psai_zy= atan2(dElectronPy_norm[iHit],dElectronPz_norm[iHit]);                    
		dElectron_psai_zx= atan2(dElectronPx_norm[iHit],dElectronPz_norm[iHit]);
		dElectron_posi_xy= atan2(dEY,dEX);
		if(fabs(yhex[iHit]-dCOM_y_e)>1e-6 || fabs(xhex[iHit]-dCOM_x_e)>1e-6)
			dElectron_psai_position[iHit] = atan2(yhex[iHit]-dCOM_y_e,xhex[iHit]-dCOM_x_e); //not used
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    iHit = 0;
    int inplane, iSign_x, iSign_y, iSign_z;
    double theta, dnx, dny, dnz, dn_ion, dn_electron, dn_length; 
    double dpx, dpy, dpz, dp_ion, dp_electron;
	inplane = 0; iSign_x = -1; iSign_y = +1; iSign_z = -1;
	if(iPolarization==0)
	{			// horizontal polarization
		dElectron_theta[iHit] = acos(dElectronPz_norm[iHit]);  
		dn_length = sin(dAngle_Width/2./180*pi);
        //	for(int it=0; it<int(180*2/dAngle_Width+0.1); it++)
		for(int it=0; it<int(180/dAngle_Width+0.1); it++)
		{
			theta = double(it)*dAngle_Width;
            //	theta = double(it)*dAngle_Width/2;
			theta = theta*pi/180;
			dnx = sin(theta);		//versor orthogonal to the plane that contains the polarization 
			dny = cos(theta);		//versor orthogonal to the plane
			dnz = 0.0;				//versor orthogonal to the plane
			dn_ion = dnx*dIon_nPx+dny*dIon_nPy+dnz*dIon_nPz;
			dn_electron = iSign_x*dnx*dElectronPx_norm[0]+iSign_y*dny*dElectronPy_norm[0]+iSign_z*dnz*dElectronPz_norm[0];
            //now checks that both ion and electron are in this plane
			if((fabs(dn_ion)<dn_length) && (fabs(dn_electron)<dn_length))
			{
                //if so, projects both ion and electron in such plane
                inplane = 1;
				dpx = sin(theta+pi/2);		//this vector is in the plane
				dpy = cos(theta+pi/2);
				dpz = 0.0;
				dp_ion = dpx*dIon_nPx+dpy*dIon_nPy+dpz*dIon_nPz;
				dp_electron = iSign_x*dpx*dElectronPx_norm[0]+iSign_y*dpy*dElectronPy_norm[0]+iSign_z*dpz*dElectronPz_norm[0];
				dIon_Angle[it] = atan2(dp_ion,dIon_nPz);
				dElectron_Angle[it] = atan2(dp_electron,iSign_z*dElectronPz_norm[0]);
				dIon_Electron_Angle[it] = dElectron_Angle[it]-dIon_Angle[it];
				if(dIon_Electron_Angle[it]<-pi) dIon_Electron_Angle[it] = dIon_Electron_Angle[it]+2.0*pi; 
				if(dIon_Electron_Angle[it]>pi) dIon_Electron_Angle[it] = dIon_Electron_Angle[it]-2.0*pi; 
				d_e_I_Angle = dIon_Electron_Angle[it];
			}
		}
	} 
	else if(iPolarization==1)
	{	// Vertical polarization
		dElectron_theta[iHit] = acos(dElectronPy_norm[iHit]);  
		dn_length = sin(dAngle_Width/2./180*pi);
		for(int it=0; it<int(180/dAngle_Width+0.1); it++)
		{
//		for(int it=0; it<int(180*2/dAngle_Width+0.1); it++)
			theta = double(it)*dAngle_Width;
//			theta = double(it)*dAngle_Width/2;
			theta = theta/180*pi;
			dnx = sin(theta);
			dny = 0.0;
			dnz = cos(theta);
			dn_ion = dIon_nPx*dnx+dIon_nPy*dny+dIon_nPz*dnz;
			dn_electron = iSign_x*dElectronPx_norm[0]*dnx+iSign_y*dElectronPy_norm[0]*dny+iSign_z*dElectronPz_norm[0]*dnz;
			//now checks that both ion and electron are in this plane
			if((fabs(dn_ion)<dn_length) && (fabs(dn_electron)<dn_length)){
			    inplane = 1;
				dpx = sin(theta+pi/2);
				dpy = 0.0;
				dpz = cos(theta+pi/2);
				dp_ion = +dIon_nPx*dpx+dIon_nPy*dpy+dIon_nPz*dpz;
				dp_electron = iSign_x*dElectronPx_norm[0]*dpx+iSign_y*dElectronPy_norm[0]*dpy+iSign_z*dElectronPz_norm[0]*dpz;
				dIon_Angle[it] = atan2(dp_ion,dIon_nPy);
				dElectron_Angle[it] = atan2(dp_electron,iSign_y*dElectronPy_norm[0]);
				dIon_Electron_Angle[it] = dElectron_Angle[it]-dIon_Angle[it];
				if(dIon_Electron_Angle[it]<-pi) dIon_Electron_Angle[it] = dIon_Electron_Angle[it]+2.0*pi; 
				if(dIon_Electron_Angle[it]>pi) dIon_Electron_Angle[it] = dIon_Electron_Angle[it]-2.0*pi; 

				//////////////////////// angle correction
				//if(dIon_Electron_Angle[it]<0) dIon_Angle[it] = dIon_Angle[it] + 7.5/180*pi; 
				//if(dIon_Electron_Angle[it]>0) dIon_Angle[it] = dIon_Angle[it] - 22.5/180*pi; 
				//if(dIon_Electron_Angle[it]<-pi) dIon_Electron_Angle[it] = dIon_Electron_Angle[it] + 2.0*pi;  
				//if(dIon_Electron_Angle[it]>pi) dIon_Electron_Angle[it] = dIon_Electron_Angle[it] - 2.0*pi; 
				if(dIon_Angle[it]<-pi) dIon_Angle[it] = dIon_Angle[it] + 2.0*pi; 
				if(dIon_Angle[it]>pi) dIon_Angle[it] = dIon_Angle[it] - 2.0*pi; 
				////////////////////////
				d_e_I_Angle = dIon_Electron_Angle[it];
				d_I_Angle = dIon_Angle[it];
			}
		}
	}
	{ 
		theta = 0.0;
		dnx = sin(theta);
		dny = 0.0;
		dnz = cos(theta);
		dn_ion = -dnx*dIon_nPx+dny*dIon_nPy+dnz*dIon_nPz;
		dn_electron = dnx*dElectronPx_norm[0]+dny*dElectronPy_norm[0]+dnz*dElectronPz_norm[0];
		dpx = sin(theta+pi/2);
		dpy = 0.0;
		dpz = cos(theta+pi/2);
		dp_ion = -dpx*dIon_nPx+dpy*dIon_nPy+dpz*dIon_nPz;
		dp_electron = dpx*dElectronPx_norm[0]+dpy*dElectronPy_norm[0]+dpz*dElectronPz_norm[0];
		dIon_Angle0 = -atan2(dp_ion,dIon_nPy);
		dElectron_Angle0 = atan2(dp_electron,dElectronPy_norm[0]);
		dIon_Electron_Angle0 = dElectron_Angle0-dIon_Angle0;
		if(dIon_Electron_Angle0<-pi) dIon_Electron_Angle0 = dIon_Electron_Angle0+2.0*pi; 
		if(dIon_Electron_Angle0>pi) dIon_Electron_Angle0 = dIon_Electron_Angle0-2.0*pi; 
		if(dIon_Angle0<-pi) dIon_Angle0 = dIon_Angle0+2.0*pi; 
		if(dIon_Angle0>pi) dIon_Angle0 = dIon_Angle0-2.0*pi; 
		if(dElectron_Angle0<-pi) dElectron_Angle0 = dElectron_Angle0+2.0*pi; 
		if(dElectron_Angle0>pi) dElectron_Angle0 = dElectron_Angle0-2.0*pi; 
	}
	//Sellect event for Lucchesse'a analysis, and convert the angle into th lab frame
	double dIon_theta_lab, dIon_psai_lab;
	double dElectron_theta_lab, dElectron_psai_lab, dElectronEnergytemp;
	if (iPolarization==0)
  {
    dIon_theta_lab = dIon_theta;
    dIon_psai_lab = pi + dIon_psai_xy;
    dElectron_theta_lab = acos(iSign_z*dElectronPz_norm[0]);
    dElectron_psai_lab = pi + atan2(iSign_y*dElectronPy_norm[0],iSign_x*dElectronPx_norm[0]);
  }
  else if(iPolarization==1)
  {
    dIon_theta_lab = dIon_theta;
    dIon_psai_lab = pi/2 + dIon_psai_zx;
    dElectron_theta_lab = acos(iSign_y*dElectronPy_norm[0]);
    dElectron_psai_lab = pi/2 + atan2(iSign_x*dElectronPx_norm[0],iSign_z*dElectronPz_norm[0]);
  }
	dElectronEnergytemp = dElectronEnergy[0];
	if((dIon_Total_Energy > ionEngMin) && (dIon_Total_Energy < ionEngMax) && 
	    (dElectronEnergytemp > elcEngMin) && (dElectronEnergytemp < elcEngMax) 
	    //&& (dElectron_psai_zx > -elcPsiLim) && (dElectron_psai_zx < elcPsiLim)
	    )
	{   
	    fwrite(&dIon_Total_Energy,sizeof(double),1,fp_sel);
	    fwrite(&dIon_theta_lab,sizeof(double),1,fp_sel);
	    fwrite(&dIon_psai_lab,sizeof(double),1,fp_sel);
	    fwrite(&dElectronEnergytemp,sizeof(double),1,fp_sel);
      fwrite(&dElectron_theta_lab,sizeof(double),1,fp_sel);
      fwrite(&(dElectron_psai_lab),sizeof(double),1,fp_sel);
      lEventCounter++ ;
	}


				////////////////////////

//Export valid data


////////// select sigma or pi symmetry  ////////modified by macchi    2003.10.2
///////////////////////////////////////////////////////////////////////////////
////////// using these caluculated momentum ///////////////////////////////////
////		dIonPx_norm[iHit] = dIonPx[jn][iHit]/dIonP[iHit];  ////////////////	
////		dIonPy_norm[iHit] = dIonPy[jn][iHit]/dIonP[iHit];  ////////////////	
////		dIonPz_norm[iHit] = dIonPz[jn][iHit]/dIonP[iHit];  ////////////////
///////////////////////////////////////////////////////////////////////////////	


 	for(iHit=0; iHit<1; iHit++){
//for sigma transition
		if ((dIonPz_norm[iHit]>0.94) || (dIonPz_norm[iHit]<-0.94))
		flagmatrix = 150;
// for pi transition
		if ((dIonPz_norm[iHit]<0.34) && (dIonPz_norm[iHit]>-0.34))	
		flagmatrix = 160;
	}

hell_2:	//this is just a label



/////////////////////////////////////////////////////////////////////////////
///                       calculated data output
/////////////////////////////////////////////////////////////////////////////
	pEventData->SetAt(iHitPattern+0,nHexX1);						// iHitPattern = 18
	pEventData->SetAt(iHitPattern+1,nHexX2);
	pEventData->SetAt(iHitPattern+2,nHexY1);
	pEventData->SetAt(iHitPattern+3,nHexY2);
	pEventData->SetAt(iHitPattern+4,nHexZ1);
	pEventData->SetAt(iHitPattern+5,nHexZ2);
	pEventData->SetAt(iHitPattern+6,nMCPele);
	pEventData->SetAt(iHitPattern+7,nMCPion);
	pEventData->SetAt(iHitPattern+8,nSqX1);
	pEventData->SetAt(iHitPattern+9,nSqX2);
	pEventData->SetAt(iHitPattern+10,nSqY1);
	pEventData->SetAt(iHitPattern+11,nSqY2);
	pEventData->SetAt(iHitPattern+12,nBunch);

	for(iHit=0; iHit<4; iHit++){
		pEventData->SetAt(31+iHit,xhex[iHit]*1000);					// Hex_X
		pEventData->SetAt(31+4+iHit,yhex[iHit]*1000);				// Hex_Y
		//pEventData->SetAt(31+28+iHit,dxSq[iHit]*1000);				// SQ_x (mm)
		//pEventData->SetAt(31+32+iHit,dySq[iHit]*1000);				// SQ_y (mm)		
		pEventData->SetAt(31+28+iHit,dxSqN[jn][iHit]*1000);				// SQ_x (mm)
		pEventData->SetAt(31+32+iHit,dySqN[jn][iHit]*1000);				// SQ_y (mm)
		pEventData->SetAt(67+iHit,dtIon[iHit]*1.0E9);				// Ion TOF (ns)
		pEventData->SetAt(67+4+iHit,dIonEnergy[iHit]/dElectron);	// Ion Kinetic Energy
	}

	pEventData->SetAt(75,dIon_Total_Energy/dElectron);				// Ion Total Kinetic Energy	
//	pEventData->SetAt(75,(dIonEnergy[0]*14/16)/dElectron);				// Ion Total Kinetic Energy	
	
	for(iHit=0; iHit<4; iHit++){
	pEventData->SetAt(76+iHit,dIonPx[jn][iHit]/dMomentum_au);		// Ion Px
	pEventData->SetAt(80+iHit,dIonPy[jn][iHit]/dMomentum_au);		// Ion Py
	pEventData->SetAt(84+iHit,dIonPz[jn][iHit]/dMomentum_au);		// Ion Pz
	}

	pEventData->SetAt(88,dSum_of_momenta_X[jn]/dMomentum_au);		// Sum_px
	pEventData->SetAt(89,dSum_of_momenta_Y[jn]/dMomentum_au);		// Sum_py
	pEventData->SetAt(90,dSum_of_momenta_Z[jn]/dMomentum_au);		// Sum_pz

	
	pEventData->SetAt(91,dIon_theta*180.0/pi); 						// 				

	pEventData->SetAt(92,dIon_psai_xy*180.0/pi);					//Ion_psai_xy
	pEventData->SetAt(93,dIon_psai_zy*180.0/pi);					//Ion_psai_yz
	pEventData->SetAt(94,dIon_psai_zx*180.0/pi);					//Ion_psai_zx

	pEventData->SetAt(95,dIon_nPx);					// Ion_Pnx
	pEventData->SetAt(96,dIon_nPy);					// Ion_Pny
	pEventData->SetAt(97,dIon_nPz);					// Ion_Pnz

//chek OK
   	pEventData->SetAt(102,dElectronEnergyXY/dElectron);			// Electron Kinetic Energy
	for(iHit=0; iHit<4; iHit++){
		pEventData->SetAt(98+iHit,dtElectron[iHit]*1.0E9);				// Electron TOF
		pEventData->SetAt(103+iHit,dElectronEnergy[iHit]/dElectron);	// Electron Kinetic Energy
		pEventData->SetAt(107+iHit,dElectronPx[iHit]/dMomentum_au);		// Electron Px
		pEventData->SetAt(111+iHit,dElectronPy[iHit]/dMomentum_au);		// Electron Py
		pEventData->SetAt(115+iHit,dElectronPz[iHit]/dMomentum_au);		// Electron Pz
	}
	pEventData->SetAt(119,dElectronPx_norm[0]);						// Normalized electron Px
	pEventData->SetAt(120,dElectronPy_norm[0]);						// Normalized electron Py
	pEventData->SetAt(121,dElectronPz_norm[0]);						// Normalized electron Pz
	pEventData->SetAt(122,dElectronP[0]/dMomentum_au);				// Sum_P_Electron

//	pEventData->SetAt(123,dElectron_theta[0]*180.0/pi);
//	pEventData->SetAt(124,dElectron_psai[0]*180.0/pi);
//	pEventData->SetAt(125,dElectron_psai_position[0]*180.0/pi);
	pEventData->SetAt(123,dElectron_psai_xy*180.0/pi);
	pEventData->SetAt(124,dElectron_psai_zy*180.0/pi);
	pEventData->SetAt(125,dElectron_psai_zx*180.0/pi);

	pEventData->SetAt(126,iMaster_Flag);
	pEventData->SetAt(127,flagmatrix);

	pEventData->SetAt(128,dCOM_x[jn]*1000);								// COM_x  mm
	pEventData->SetAt(129,dCOM_y[jn]*1000);								// COM_y  mm 
	pEventData->SetAt(130,dCOM_z[jn]*1000);								// COM_z  mm


	

	pEventData->SetAt(131,dCOM_x_e);								// COM_e_x
	pEventData->SetAt(132,dCOM_y_e);								// COM_e_y



	pEventData->SetAt(133,dphai/pi*180);
	pEventData->SetAt(134,dEX*1000);
	pEventData->SetAt(135,dEY*1000);
	
	pEventData->SetAt(136,dElectron_theta0[0]*180.0/pi);

	pEventData->SetAt(137,dElectron_posi_xy*180.0/pi);

	pEventData->SetAt(138,dIon_Electron_Angle0*180.0/pi);
	pEventData->SetAt(139,isellect);								// COM_e_y
	pEventData->SetAt(140,dtN*1.0E9);								// COM_e_y
	pEventData->SetAt(141,dtO*1.0E9);								// COM_e_y

	for(int ii=0; ii<int(180/dAngle_Width+0.1); ii++){
		pEventData->SetAt(142+ii*3,dIon_Angle[ii]/pi*180);
		pEventData->SetAt(142+ii*3+1,dElectron_Angle[ii]/pi*180);
		pEventData->SetAt(142+ii*3+2,dIon_Electron_Angle[ii]/pi*180);
	}
	pEventData->SetAt(196,d_I_Angle*180.0/pi);
	pEventData->SetAt(197,d_e_I_Angle*180.0/pi);
	pEventData->SetAt(198,distancexy*1000);	
	return;
}

CDAN_API void AnalysisFinalize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter)
{
//	fclose(fp);
//	char buffer[20];
//	itoa( TotalCount, buffer, 10 );
//	MessageBoxA(0,buffer,"TotalCount",MB_OK);
//	itoa( TrueCount, buffer, 10 );
//	MessageBoxA(0,buffer,"TrueCount",MB_OK);
    double tctemp[3];
    double dTcyc;
    fclose(fp_sel);
    tctemp[0] = tof(0, 0, dMass[0], dCharge[0]); 
    tctemp[1] = tof(0, 0, dMass[1], dCharge[1]); 
    tctemp[2] = electof(0);
    dTcyc = 2.0 * pi * dElectronMass / dMagnetic_Field / dElectron;
    fp1 = fopen("tc.dat","wt");
	fprintf(fp1, "Tion1 = %12.10f ns\n", tctemp[0]*1.0E9);
	fprintf(fp1, "Tion2 = %12.10f ns\n", tctemp[1]*1.0E9);
	fprintf(fp1, "Tion1(z0) = %12.10f ns\n", tof(0, 0.0000354, dMass[0], dCharge[0])*1.0E9);
	fprintf(fp1, "Tion2(z0) = %12.10f ns\n", tof(0, 0.0000354, dMass[1], dCharge[1])*1.0E9);
	fprintf(fp1, "Telec = %12.10f ns\n", tctemp[2]*1.0E9);
	fprintf(fp1, "dTcyc = %12.10f ns \n", dTcyc*1.0E9);
	fprintf(fp1, "atan2(1,0) = %f\n", atan2(1,0));
	fprintf(fp1, "atan2(0,1) = %f\n", atan2(0,1));
	fprintf(fp1, "lEventCounter = %u\n", lEventCounter);
   fclose(fp1);
}
