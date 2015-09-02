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
//  User defined analysis part called from cobold main program
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

////////////////  Please   ATTENTION !!!!!!!!   /////////////////////////////
////    This progrm is restricted version for 4hit.  ////////////////////////
////    AnaIon value must be fixed "4" in CCF now.   ////////////////////////
/////////////////////////////////////////////////////////////////////////////
const int nTDC=3; // Number of TDC used.  Don't forget !! Input real number
const int nCH=2;  // Number of channels in each TDC.
const int nHit=4; // This corresponds to number of hit. Check the active CAMAC.CIF

const double pi = 3.141592654;
const double dElectronMass = 9.109534e-31; // Electron Mass (kg)
const double dElectron = 1.60219e-19;      // Electric charge (Coulomb)
const double dMassUnit = 1.6605656e-27;    // Atomic Mass Unit (kg)
const double dMomentum_au = 1.9921e-24;    // 1 au momentum = 1.9921E-24 (kg*m/s)
const double unit_nano = 1.e-9;
const double unit_milli = 1.e-3;
const double unit_gauss = 1.e-4;

std::fstream file_electron, file_log;
std::string specified_file;

// Analysis global variable definitions
int counter = 0;
bool polarization_is_horizontal;
int nIonHit, nElectronHit;
double dDelay;
double dPixelSizeX, dPixelSizeY;
double dCorX, dCorY;
double dCOM_x0, dCOM_y0, dCOM_z0;
double dCOM_x_width, dCOM_y_width, dCOM_z_width;
double dMax_TOF[nHit], dMin_TOF[nHit];
double dElectric_Field1, dElectric_Field2, dElectric_Field3;                 // electric field (V/m)
double dMagnetic_Field;                 // electric field (G)
double dElectron_T0;
double dL1, dL2, dL3;                                // length of ion acceleration region
double dD1;                             // length of electron acceleration region
double dD2;                             // length of electron drift tube
double dMax_eTOF;
double dMin_eTOF;
double dCOM_x_e0, dCOM_y_e0;            // center of mass of electron
double dX_cor, dY_cor;                  // position correction for electron detector
double dCOM_x_e_tan;                    //tangent of electron COM X  mm/ns
double dCOM_y_e_tan;                    //tangent of electron COM X  mm/ns
double dAngle_Width;
double dPosi_cor[20];
double dJet;    //jet correction +dJet*tIon(ns)
double dCharge[nHit], dMass[nHit];

double AtomTOFWidth, AtomXwidth, AtomYwidth; //To remove atomic ion line
double xSq0_HotspotMin, xSq0_HotspotMax, ySq0_HotspotMin, ySq0_HotspotMax;
double xSq1_HotspotMin, xSq1_HotspotMax, ySq1_HotspotMin, ySq1_HotspotMax;
double tIon0_RandCoinMin, tIon0_RandCoinMax, tIon1_RandCoinMin, tIon1_RandCoinMax;

// to calculate time of flight
double tof(const double p0, const double xx, const double mass, const double charge){
    const double p1=sqrt(pow(p0,2.0)+2.0*mass*charge*dElectric_Field1*(dL1-xx));
    const double p2=sqrt(pow(p1,2.0)+2.0*mass*charge*dElectric_Field2*dL2);
    const double p3=sqrt(pow(p2,2.0)+2.0*mass*charge*dElectric_Field3*dL3);
    const double t1=(p1-p0)/charge/dElectric_Field1;
    const double t2=(p2-p1)/charge/dElectric_Field2;
    const double t3=(p3-p2)/charge/dElectric_Field3;
    return t1+t2+t3;
} // end tof

// to find the pz from zero z coordinate
void find_pz(const double mass, const double charge, const double TOF, double &pz){
    double p0, p1, p2, p3, tof0;
    double Jac;   //Jacobian matrix
    int k;
    double delta = 1.e0, limit = dMomentum_au*1.0e-2;

    //Initial guess of pz and z
    p0 = 0.e0;
    k = 0;

    while ((fabs(delta) > limit) && (k < 100)){
        // corresponding Jacobian matrix
        p1 = sqrt(p0*p0 +2.e0*mass*charge*dElectric_Field1*(dL1 -dCOM_z0));
        p2 = sqrt(p1*p1 +2.e0*mass*charge*dElectric_Field2*dL2);
        p3 = sqrt(p2*p2 +2.e0*mass*charge*dElectric_Field3*dL3);
        tof0 = (p1 -p0)/charge/dElectric_Field1
            +(p2 -p1)/charge/dElectric_Field2
            +(p3 -p2)/charge/dElectric_Field3;
        Jac = -1.e0/charge/dElectric_Field1
            +p0/p1*(1.e0/charge/dElectric_Field1 -1.e0/charge/dElectric_Field2)
            +p0/p2*(1.e0/charge/dElectric_Field2 -1.e0/charge/dElectric_Field3)
            +p0/p3*1.e0/charge/dElectric_Field3;

        // Calculate the delta and get new pz
        if(Jac==0.e0){
            return;
        } else{
            delta = -(tof0-TOF)/Jac;
            p0 = p0 +delta;
            k = k +1;
        }
    } // end while
    pz = p0;
    return;
} // end find_pz


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
    while ((sumDeltaM > MomentumLimit) && (fabs(delta[NumIons-1]) > PositionLimit) && (k < 100)){
        // corresponding Jacobian matrix
        // Row from 0 to NumIons-2
        for (i=0; i<NumIons-1; i++){
            p1[i] = sqrt(p0[i]*p0[i] + 2*mass[i]*charge[i]*dElectric_Field1*(dL1-z0));
            p2[i] = sqrt(p1[i]*p1[i] + 2*mass[i]*charge[i]*dElectric_Field2*dL2);
            p3[i] = sqrt(p2[i]*p2[i] + 2*mass[i]*charge[i]*dElectric_Field3*dL3);
            tof0[i] = (p1[i] - p0[i])/charge[i]/dElectric_Field1
            + (p2[i]-p1[i])/charge[i]/dElectric_Field2
            + (p3[i]-p2[i])/charge[i]/dElectric_Field3;
            Jac[i][i] = - 1/charge[i]/dElectric_Field1
            + p0[i]/charge[i]/p1[i] * (1/dElectric_Field1 -  1/dElectric_Field2)
            + p0[i]/charge[i]/p2[i] * (1/dElectric_Field2 - 1/dElectric_Field3)
            + p0[i]/charge[i]/p3[i] *  1/dElectric_Field3;
            Jac[i][NumIons-1] =
            - mass[i]*dElectric_Field1/p1[i] * (1/dElectric_Field1 - 1/dElectric_Field2)
            - mass[i]*dElectric_Field1/p2[i] * (1/dElectric_Field2 - 1/dElectric_Field3)
            - mass[i]*dElectric_Field1/p3[i] * 1/dElectric_Field3;
        } // end for
        //the NumIons'th row
        i = NumIons -1;
        p1[i] = sqrt(p0[i]*p0[i] + 2*mass[i]*charge[i]*dElectric_Field1*(dL1-z0));
        p2[i] = sqrt(p1[i]*p1[i] + 2*mass[i]*charge[i]*dElectric_Field2*dL2);
        p3[i] = sqrt(p2[i]*p2[i] + 2*mass[i]*charge[i]*dElectric_Field3*dL3);
        tof0[i] = (p1[i] - p0[i])/charge[i]/dElectric_Field1
            + (p2[i] - p1[i])/charge[i]/dElectric_Field2
            + (p3[i] - p2[i])/charge[i]/dElectric_Field3;
        for (j=0; j<NumIons-1; j++){
            Jac[i][j] = 1/charge[i]/dElectric_Field1
            + p0[j]/charge[i]/p1[i] * (1/dElectric_Field1 -  1/dElectric_Field2)
            + p0[j]/charge[i]/p2[i] * (1/dElectric_Field2 - 1/dElectric_Field3)
            + p0[j]/charge[i]/p3[i] *  1/dElectric_Field3;
        } // end for
        Jac[i][NumIons-1] =
            - mass[i]*dElectric_Field1/p1[i] * (1/dElectric_Field1 - 1/dElectric_Field2)
            - mass[i]*dElectric_Field1/p2[i] * (1/dElectric_Field2 - 1/dElectric_Field3)
            - mass[i]*dElectric_Field1/p3[i] * 1/dElectric_Field3;
        // Get Inversed 2-dimensional matrix directly.
        if(abs(NumIons - 2)<0.2){
            JacProduct = Jac[0][0]*Jac[1][1] - Jac[0][1]*Jac[1][0];
            if(JacProduct==0) return;
            InvJac[0][0] = Jac[1][1]/JacProduct;
            InvJac[0][1] = -Jac[0][1]/JacProduct;
            InvJac[1][0] = -Jac[1][0]/JacProduct;
            InvJac[1][1] = Jac[0][0]/JacProduct;
        } // end if
        if(abs(NumIons - 3)<0.2){
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
        } // end if
        if(abs(NumIons - 4)<0.2){
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
        } // end if
        // Calculate the delta and get new pz and z
        for (i=0; i<NumIons; i++){
            delta[i] =0;
            for (j=0; j<NumIons; j++){
                delta[i] = delta[i] - InvJac[i][j] * (tof0[j] - TOF[j]);
            } // end for
        } // end for
        p0[NumIons-1] = 0;
        sumDeltaM = 0;
        for (i=0; i<NumIons-1; i++){
            p0[i] = p0[i] + delta[i];
            p0[NumIons-1] = p0[NumIons-1] - p0[i];
            sumDeltaM = sumDeltaM + fabs(delta[i]);
        } // end for
        z0 = z0 + delta[NumIons-1];
        k = k + 1;
    } // end while
    for (i=0; i<NumIons; i++){
        *(pz+i) = p0[i];
    } // end for
    z = z0;
    return;
} // end function(find_pz_z)

// calculate electron tof
double electof(double p0)
{
    double p1, t1, t2, tt;

    p1 = sqrt(p0*p0 + 2*dElectronMass*dElectron*dElectric_Field1*dD1);
    t1 = (p1-p0)/dElectron/dElectric_Field1;
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
        p1 = sqrt(p0*p0+2*dElectronMass*dElectron*dElectric_Field1*(dD1-z0));
        tof0 = (p1-p0)/dElectron/dElectric_Field1 + dD2*dElectronMass/p1;
        Jac  = 1.0/dElectron/dElectric_Field1 *
        ( (p0*p0*p0 + dElectronMass*dElectron*dElectric_Field1*p0*(2*(dD1-z0)-dD2))/(p1*p1*p1) - 1);

        // Calculate the delta and get new pz
        delta = - (tof0 - TOF)/Jac;
        p0 = p0 + delta;
        k = k + 1;
    }
    //throw out the event that electron pass the mesh at ion side
    if( p0 < - sqrt(2*dElectronMass*dElectric_Field1*(dL1+z0)*dElectron)) p0 = -10000;
    pz = p0;
    return;
}

CDAN_API LPCTSTR AnalysisGetInformationString(){
    return LPCTSTR("CAMAC for CoboldPC 6.2.0.81");
} // end LPCTSRT
