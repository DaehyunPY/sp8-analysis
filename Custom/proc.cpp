CDAN_API void AnalysisProcessEvent(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter){
    using namespace std;

    // delate in the future
    int iHit, jjj, jn;
    int iHitsTDC1 = nHit;          // for electron
    int iHitsTDC2 = nHit;          // for ion

    // for Hexagonal anode
    double xhex[nHit], yhex[nHit];
    //double dOriginElectron;
    double dtElectron[nHit];

    // for Square anode
    double dxSq[nHit], dySq[nHit], dxSqN[2][nHit], dySqN[2][nHit];
    double dx0, dy0;
    double dtIon[nHit], dtIonN[2][nHit];//dOriginIon

    // for hitpattern
    int nHexX1, nHexX2, nHexY1, nHexY2, nHexZ1, nHexZ2, nMCPele, nMCPion;
    int nSqX1, nSqX2, nSqY1, nSqY2, nBunch;

    int e_mcp = 6, i_mcp = 4;      // NEW
    int flagmatrix;
    int iMaster_Flag;           //set the credibility of the event
    int e_region =0;                        // cut out detecter
    int iSTOP = 6;              // address for STOP signal of Hoshin-TDC
    int iCount;
    int iangle_cor;
    int isellect=-1;    //1:first ion is N+, 2: first ion is O+,
    //3:difficult to determine(N+), 4:difficult to determine(O+)

    double a[8][8];             // for electron
    double b[8][8];             // for ion

    double dSum_Of_mt;
    double dSum_Of_mxt;
    double dSum_Of_myt;
    double dCOM_x[nHit], dCOM_y[nHit], dCOM_z[nHit];
    double square_P = -1000;
    double dIonPx[2][nHit], dIonPy[2][nHit], dIonPz[2][nHit], dIonP[nHit];      // Components of momentum of Ion
    double dIonPx_norm[nHit], dIonPy_norm[nHit], dIonPz_norm[nHit];             // Normalized momentum of Ion
    double dElectronPx[nHit], dElectronPy[nHit], dElectronPz[nHit], dElectronP[nHit]; // Components of momentum of Electron
    double dElectronPx_norm[nHit], dElectronPy_norm[nHit], dElectronPz_norm[nHit];    // Normalized momenta
    double dIon_Total_Energy = -10000;
    double dIonEnergy[nHit];
    double dElectronEnergy[nHit];
    double dElectronEnergyHigher = -10000, dElectronEnergyLower = -10000;
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

    double random_number;

    // Initialize variables
    for(jjj=0;jjj<100;jjj++){
        dIon_Angle[jjj] = -1000;
        dElectron_Angle[jjj] = -2000;
        dIon_Electron_Angle[jjj] = -10000;
    }


    for(int iTDC=0; iTDC<nTDC; iTDC++){
        for(int iCH=0; iCH<nCH; iCH++){
            for(int iHit=0; iHit<nHit+1; iHit++){
                TDC[iTDC][iCH][iHit] = -10000;
            }
        }
    }


    jn = 0;
    for(int iHit=0;iHit<nHit;iHit++){
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
        for(int i=0; i<7; i++){
            a[i][iHit] = -10000;
        }
    }
    for(jjj=0;jjj<3;jjj++){
        for(int iHit=0;iHit<4;iHit++){
            dIonPx[jjj][iHit] = -10000;
            dIonPy[jjj][iHit] = -10000;
            dIonPz[jjj][iHit] = -10000;
            dxSqN[jjj][iHit] = -10000;
            dySqN[jjj][iHit] = -10000;
            dtIonN[jjj][iHit] = -10000;
            }
        dCOM_x[jjj] = -10000;
        dCOM_y[jjj] = -10000;
        //dCOM_z[jjj] = -10000;
        dSum_of_momenta_X[jjj] = -10000;
        dSum_of_momenta_Y[jjj] = -10000;
        dSum_of_momenta_Z[jjj] = -10000;
        dSum_of_momenta[jjj] = -10000;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////

    for(int iTDC=0; iTDC<nTDC; iTDC++){
        for(int iCH=0; iCH<nCH; iCH++){
            for(int iHit=0; iHit<nHit+1; iHit++){
                TDC[iTDC][iCH][iHit]  = pEventData->GetAt(iHit+iCH*(nHit+1)+iTDC*(nHit+1)*nCH)/1000.0;
            }
        }
    }



    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Make sure how many hits come to each channeles of each TDCs (if TAC Rawdata
    // values are not equal to 2048 , it means there is signal)
    /////////////////////////////////////////////////////////////////////////////
    for(int iHit=0; iHit<nHit; iHit++){
        if(TDC[1][0][iHit]<15000) nMCPele += 1;
        if(TDC[2][1][iHit]<15000) nMCPion += 1;
    }
    iHitsTDC1 = (nMCPele <= iHitsTDC1) ? nMCPele : iHitsTDC1;
    iHitsTDC2 = (nMCPion <= iHitsTDC2) ? nMCPion : iHitsTDC2;


    if((nMCPele==0) || (nMCPion==0))  //If the number of electrons and ions is not within requirement
    {
        iMaster_Flag = -16;
        flagmatrix = 16;
        goto hell_2;
    }




    /////////////////////////////////////////////////////////////////
    //////////////////////  Analysis  for ION ///////////////////////
    /////////////////////////////////////////////////////////////////

    //dOriginIon = TDC[6][0][0]+dBunch;     // Interval of bunch -> 57 ns //114 is just only trial

    // Inversion of time direction
    // First ion defines time-axis-origin in time
    // nHit+1->nHit
    for(int iHit=0;iHit<nHit;iHit++){
        b[0][iHit] = TDC[1][1][iHit];       // x mm
        b[1][iHit] = TDC[2][0][iHit];       // y mm
        b[4][iHit] = TDC[2][1][iHit];       // MCP (ns)
    }  //because the logic of read data is different from data be stored in resort, some strange sentences are used

    //ion TOF
    for(int iHit=0;iHit<nIonHit;iHit++){
        dtIon[iHit] = (b[4][iHit]-2000)*(1.0E-9) - dDelay;  // unit sec
    }
    dtN=dtIon[0];
    dtO=dtIon[1];

    // Now do calculates for the ion's position of all hits
    for(int iHit=0;iHit<nIonHit;iHit++){
        dx0 = b[0][iHit]-100;
        dy0 = b[1][iHit]-100;
        dxSq[iHit] = 1.0E-3*((dx0+dCorX*dy0)*dPixelSizeX);          // Unit Meter
        dySq[iHit] = 1.0E-3*((dy0+dCorY*dx0)*dPixelSizeY + dJet*dtIon[iHit]/1.0E-9);    // Unit Meter
        //dXion = dxSq[0];
        //dYion = dySq[0];
    }


    // Calculation of ion time ordered as y
    // Require 2 hits for ion
    //--------------------------------------------------------------------------
    if(iHitsTDC2<nIonHit){ // make sure the number of ions is larger than required
        iMaster_Flag = -20;
        flagmatrix = 20;
        goto hell; //
    }

    for(int iHit=0;iHit<nIonHit;iHit++){
        if(dtIon[iHit]<dMin_TOF[iHit]){
            iMaster_Flag = -21;
            flagmatrix = 21;
        }
        if(dtIon[iHit]>dMax_TOF[iHit]){
            iMaster_Flag = -22;
            flagmatrix = 22;
        }
    }
    //////////11/20 榊原がPIPICOのおかしな部分を消去するために追加


    // if((dtIon[0]+Del_PIPICOline_posi12+Del_PIPICOline_width12>=dtIon[1])&(dtIon[1]>=dtIon[0]+Del_PIPICOline_posi12)){
    //     iMaster_Flag = -23;
    //     flagmatrix = 23;
    // }
    // if((dtIon[1]+Del_PIPICOline_posi23+Del_PIPICOline_width23>=dtIon[2])&(dtIon[2]>=dtIon[1]+Del_PIPICOline_posi23)){
    //     iMaster_Flag = -24;
    //     flagmatrix = 24;
    // }
    // if((dtIon[2]+Del_PIPICOline_posi34+Del_PIPICOline_width34>=dtIon[3])&(dtIon[3]>=dtIon[2]+Del_PIPICOline_posi34)){
    //     iMaster_Flag = -25;
    //     flagmatrix = 25;
    // }
    if(iMaster_Flag<0) goto hell; // select the ion from Time of Flight.

    isellect=0;


    // Calculation of ion momenta
    // --------------------------------------------------------
    //-----------------------------------------------------mistake iHitTDC2
    for(int iHit=0;iHit<nIonHit;iHit++){
        dtIonN[0][iHit] = dtIon[iHit];
        dxSqN[0][iHit] = dxSq[iHit];
        dySqN[0][iHit] = dySq[iHit];
    }
    //dtIonN[1][0] = dtIon[1];
    //dxSqN[1][0] = dxSq[1];
    //dySqN[1][0] = dySq[1];
    //dtIonN[1][1] = dtIon[0];
    //dxSqN[1][1] = dxSq[0];
    //dySqN[1][1] = dySq[0];

    double TOF0[2];   //get calculated p0z0 tof
    TOF0[0] = tof(0, 0, dMass[0], dCharge[0]);
    TOF0[1] = tof(0, 0, dMass[1], dCharge[1]);
    if ( (fabs(dtIon[0] - TOF0[0]) < AtomTOFWidth/2
    && fabs(dxSq[0] - dCOM_x0) < AtomXwidth/2
    && fabs(dySq[0] - dCOM_y0) < AtomYwidth/2)
    || (fabs(dtIon[1] - TOF0[1]) < AtomTOFWidth/2
    && fabs(dxSq[1] - dCOM_x0) < AtomXwidth/2
    && fabs(dySq[1] - dCOM_y0) < AtomYwidth/2 )) {
        iMaster_Flag = -26;
        goto hell;
    }
    // if 1or2hit ion is within Hotspot of the detector & 1or2hit ionTOF is within RandomCoincidenceLine
    if (
    ( (dxSqN[jn][0]*1000 > xSq0_HotspotMin && dxSqN[jn][0]*1000 < xSq0_HotspotMax && dySqN[jn][0]*1000 > ySq0_HotspotMin && dySqN[jn][0]*1000 < ySq0_HotspotMax) || (dxSqN[jn][1]*1000 > xSq1_HotspotMin && dxSqN[jn][1]*1000 < xSq1_HotspotMax && dySqN[jn][1]*1000 > ySq1_HotspotMin && dySqN[jn][1]*1000 < ySq1_HotspotMax) )
    && ( (dtIon[0]*1.0E9 > tIon0_RandCoinMin && dtIon[0]*1.0E9 < tIon0_RandCoinMax) || (dtIon[1]*1.0E9 > tIon1_RandCoinMin && dtIon[1]*1.0E9 < tIon1_RandCoinMax) )
    ) {
    iMaster_Flag = -27;
    goto hell;
    }

    //for(jjj=0;jjj<2;jjj++)
    for(jjj=0;jjj<1;jjj++){
        // Calculation of Pz
        dSum_of_momenta_Z[jjj] = 0;
        for(int iHit=0;iHit<nIonHit;iHit++){
            //double dTT0 = dtIonN[jjj][iHit]-dT0[iHit];
            //double sqmd = sqrt(2.0*dMass[iHit]*dL1/dCharge[iHit]/dElectric_Field1);
            //tempsqmd[i] = sqmd;
            //dIonPz[jjj][iHit] = -dCharge[iHit]*dElectric_Field1*dTT0*(dTT0+2.0*sqmd)/2.0/(dTT0+sqmd);
            find_pz(dMass[iHit], dCharge[iHit], dtIonN[jjj][iHit], dIonPz[jjj][iHit]);
            dSum_of_momenta_Z[jjj] = dSum_of_momenta_Z[jjj]+dIonPz[jjj][iHit];
        } // end for(int iHit)

        //      c0 = dT0[0]-sqrt(2.0*dMass[0]*dL1/dCharge[0]/dElectric_Field1);
        //      c1 = dT0[1]-sqrt(2.0*dMass[1]*dL1/dCharge[1]/dElectric_Field1);
        //      tc0 = dtIonN[jjj][0]-c0;
        //      tc1 = dtIonN[jjj][1]-c1;
        //      sqrtA = fabs(dElectric_Field1/sqrt(8.0)*(dCharge[0]*tc0*tc0/dMass[0]-dCharge[1]*tc1*tc1/dMass[1])
        //          /(tc0/dMass[0]+tc1/dMass[1]));
        //      an = 1.0;
        //      if(dtIonN[jjj][0]>dT0[0]) an = -1.0;
        //      dIonPz[jjj][0] = an*sqrt(2.0) * sqrtA;
        //      dIonPz[jjj][1] = -dIonPz[jjj][0];
        //      dCOM_z[jjj] = tc0/2.0/dMass[0]*(dCharge[0]*dElectric_Field1*tc0+an*sqrt(8.0)*sqrtA)-dL1;

        //find_pz_z(dMass, dCharge, dtIonN[jjj], dCOM_z[jjj], dIonPz[jjj], nIonHit);

        dSum_Of_mt  = 0;
        dSum_Of_mxt = 0;
        dSum_Of_myt = 0;

        for(int iHit=0; iHit<nIonHit; iHit++){
            dSum_Of_mt = dSum_Of_mt+dMass[iHit]/dtIonN[jjj][iHit];
            dSum_Of_mxt = dSum_Of_mxt+dxSqN[jjj][iHit]*dMass[iHit]/dtIonN[jjj][iHit];
            dSum_Of_myt = dSum_Of_myt+dySqN[jjj][iHit]*dMass[iHit]/dtIonN[jjj][iHit];
        } // end for(int iHit)

        dCOM_x[jjj] = dSum_Of_mxt/dSum_Of_mt;       // Position of Center_of_Mass of the molecule
        dCOM_y[jjj] = dSum_Of_myt/dSum_Of_mt;       // (or you can call it  <Point of Production>)

        dSum_of_momenta_X[jjj] = 0;
        dSum_of_momenta_Y[jjj] = 0;
        for(int iHit=0; iHit<nIonHit; iHit++){
            dIonPx[jjj][iHit] = dMass[iHit]/dtIonN[jjj][iHit]*(dxSqN[jjj][iHit]-dCOM_x0);
            dIonPy[jjj][iHit] = dMass[iHit]/dtIonN[jjj][iHit]*(dySqN[jjj][iHit]-dCOM_y0);
            dSum_of_momenta_X[jjj] = dSum_of_momenta_X[jjj]+dIonPx[jjj][iHit];
            dSum_of_momenta_Y[jjj] = dSum_of_momenta_Y[jjj]+dIonPy[jjj][iHit];
            //dIonPx[jjj][iHit] = dMass[iHit]/dtIonN[jjj][iHit]*(dxSqN[jjj][iHit]-dCOM_x[jjj]);
            //dIonPy[jjj][iHit] = dMass[iHit]/dtIonN[jjj][iHit]*(dySqN[jjj][iHit]-dCOM_y[jjj]);
        } // end for(int iHit)
    }  // end for(jjj)


    jn = 0;
    // end of check ion
    for(int iHit=0; iHit<nIonHit; iHit++){
        square_P = dIonPx[jn][iHit]*dIonPx[jn][iHit]+dIonPy[jn][iHit]*dIonPy[jn][iHit]+dIonPz[jn][iHit]*dIonPz[jn][iHit];
        dIonP[iHit]= sqrt(square_P);      // Total Momentum of Ion
        dIonPx_norm[iHit] = dIonPx[jn][iHit]/dIonP[iHit];   // Normalized
        dIonPy_norm[iHit] = dIonPy[jn][iHit]/dIonP[iHit];   // momentum-vectors
        dIonPz_norm[iHit] = dIonPz[jn][iHit]/dIonP[iHit];
    } // END for(int iHit=0; iHit < nIonHit; iHit++)

    if(iMaster_Flag < 0) goto hell;

    // Calculation of ion kinetic energies
    // -------------------------------------------------------------------------------
    dIon_Total_Energy = 0;
    for(int iHit=0; iHit<nIonHit; iHit++){
    dIonEnergy[iHit] = 0.5*dIonP[iHit]*dIonP[iHit]/dMass[iHit];
    dIon_Total_Energy = dIon_Total_Energy+dIonEnergy[iHit];
    }

    // Calculation of molecular direction
    // ------------------------------------
    random_number = (double)(rand())/RAND_MAX;
    if (random_number < 0.5) {
    dIon_nPx = dIonPx[jn][0];
    dIon_nPy = dIonPy[jn][0];
    dIon_nPz = dIonPz[jn][0];
    }
    else {
    dIon_nPx = dIonPx[jn][1];
    dIon_nPy = dIonPy[jn][1];
    dIon_nPz = dIonPz[jn][1];
    }
    square_P = dIon_nPx*dIon_nPx+dIon_nPy*dIon_nPy+dIon_nPz*dIon_nPz;
    if(square_P > 0){
    square_P = sqrt(square_P);          // Total Momentum of Ion
    dIon_nPx = dIon_nPx/square_P ;      // Normalized
    dIon_nPy = dIon_nPy/square_P ;      // Normalized
    dIon_nPz = dIon_nPz/square_P ;      // Normalized
    } else {
        iMaster_Flag = -36; // square_P must not be zero !! kill this event !
        flagmatrix = 36;
    }

    if(iMaster_Flag < 0) goto hell;

    //  Horizonatal polarization
    if(polarization_is_horizontal){
    dIon_theta = acos(dIon_nPz);  // Angle between Ion and spectrometer (Z-axis))
    }
    //  Vertical polarization
    if(!polarization_is_horizontal){
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

    hell:   //this is just a label
    //////////////////////////////////////////////////////////////////////////////////
    //////////// Analysis for electron ///////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////

    // Deduce the origin of ion by checking the bunch marker
    //  dOriginElectron = dOriginIon;

    for(int iHit=0;iHit<nHit;iHit++){
        a[0][iHit] = TDC[0][0][iHit];       // x1 (ns)
        a[1][iHit] = TDC[0][1][iHit];       // x2 (ns)
        a[6][iHit] = TDC[1][0][iHit];       // MCP (ns)
    }


    //electron TOF
    for(int iHit=0;iHit<nHit;iHit++){
        dtElectron[iHit] = (a[6][iHit]-1980)*(1.0E-9) ; // unit sec
        //dtElectron[iHit] = dtElectron[iHit] - dDelay;
    }
    // Calculation of POSITIONS in meters via old method (just for checks):
    // ------------------------------------------------------------------------
    //for(int iHit=0;iHit<nHit;iHit++)
    //{

    //HEXAGONAL to SQUARE
    for(iCount=0;iCount<nHit;iCount++){
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
    //**************************************これより上*********************************************************************
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // selection of electron TOF, the first electron
    for(int iHit=0; iHit < nElectronHit; iHit++){
        if((dtElectron[iHit]< dMin_eTOF) || (dtElectron[iHit]> dMax_eTOF)){
            iMaster_Flag = -46;
            flagmatrix = 46;
            goto hell_2;
        }
    }
    ////  Calculation of Electron momenta


    //  for(iCount=0; iCount<iHitsTDC1; iCount++)
    double dtElectron0;
    dtElectron0 = electof(0);
    for(iCount=0; iCount < nElectronHit; iCount++){
        //   calculation of Pz
        dDelta_T = dtElectron[iCount] - dElectron_T0 + dtElectron0;
        //find_elec_pz(dDelta_T, -dCOM_z[jn], dElectronPz[iCount]);
        find_elec_pz(dDelta_T, 0, dElectronPz[iCount]);
        if (dElectronPz[iCount] < -9900){
            iMaster_Flag = -47;
            goto hell_2;
        } // end if

        // calculation of Px, Py
        dCOM_x_e = dCOM_x_e0 + dCOM_x_e_tan * dDelta_T;
        dCOM_y_e = dCOM_y_e0 + dCOM_y_e_tan * dDelta_T;
        if(iMaster_Flag < 0){
            dEX=xhex[iCount] - dCOM_x_e;
            dEY=yhex[iCount] - dCOM_y_e;
        } else{
            dEX=xhex[iCount] - dCOM_x_e;
            dEY=yhex[iCount] - dCOM_y_e;
            //dEX=xhex[iCount] - dCOM_x_e - dCOM_x0;
            //dEY=yhex[iCount] - dCOM_y_e + dCOM_y0;
        } // end if

     ///////////////////////////////now modifying 2003.8.11////////////////////////////////////cut out detector region
        if((atan2(dEY,dEX)*180/pi <120) && (atan2(dEY,dEX)*180/pi >60)){
            e_region = 1;
        } // end if

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

        if(dMagnetic_Field > 1.0e-8 ){
            dTcyc = 2.0 * pi * dElectronMass / dMagnetic_Field / dElectron;
            //double dalpha2 = (dtElectron[iCount] - dDelay)/dTcyc;
            dalpha1 = dDelta_T/dTcyc;
            dalpha = (dalpha1 - int(dalpha1))*2.0*pi;
            dphai = - dalpha * 0.5;
            dConst = fabs(dElectron * dMagnetic_Field / sin(dphai) / 2);
            dElectronPx[iCount] = dConst * (cos(dphai)*dEX + sin(dphai)*dEY);
            dElectronPy[iCount] = dConst * (-sin(dphai)*dEX + cos(dphai)*dEY);
        } else{
            //dElectronPx[iCount] = dElectronMass * dEX / (dtElectron[iCount] - dDelay);
            //dElectronPy[iCount] = dElectronMass * dEY / (dtElectron[iCount] - dDelay);
            dElectronPx[iCount] = dElectronMass * dEX / dDelta_T;
            dElectronPy[iCount] = dElectronMass * dEY / dDelta_T;
        } // end for

        square_P = dElectronPx[iCount]*dElectronPx[iCount] + dElectronPy[iCount]*dElectronPy[iCount] + dElectronPz[iCount]*dElectronPz[iCount];

        if(square_P > 0){
            dElectronP[iCount]= sqrt(square_P);    // Total Momentum of Electron
            dElectronPx_norm[iCount]    = dElectronPx[iCount] / dElectronP[iCount]; // Normalized
            dElectronPy_norm[iCount]    = dElectronPy[iCount] / dElectronP[iCount]; // momentum-vectors
            dElectronPz_norm[iCount]    = dElectronPz[iCount] / dElectronP[iCount];
        } else{
            iMaster_Flag = -53; // square_P must not be zero !! kill this event !
            flagmatrix = 53;
            goto hell_2;
        } // end for
    } // END for(iCount=0; iCount < iHItsTDC1; iCount++)

    flagmatrix = 54;
    // Calculation of electron kinetic energies
    //  for(int iHit=0; iHit<iHitsTDC1; iHit++){

    for(int iHit=0; iHit < nElectronHit; iHit++){
        dElectronEnergy[iHit] = 0.5*dElectronP[iHit]*dElectronP[iHit]/dElectronMass;
        dElectronEnergyXY = 0.5*(dElectronPx[iHit]*dElectronPx[iHit] + dElectronPy[iHit]*dElectronPy[iHit] )/dElectronMass;
    } // end for

    // Calculation of electron angles
    //  for(int iHit=0; iHit<iHitsTDC1; iHit++){
    for(int iHit=0; iHit < nElectronHit; iHit++){
        dElectron_theta0[iHit] = acos(dElectronPz_norm[iHit]);
        dElectron_psai[iHit]= atan2(dElectronPy_norm[iHit],dElectronPx_norm[iHit]);
        dElectron_psai_xy= atan2(dElectronPy_norm[iHit],dElectronPx_norm[iHit]);
        //if(!polarization_is_horizontal){dElectron_psai_xy= atan2(dElectronPx_norm[iHit],dElectronPy_norm[iHit]);}
        dElectron_psai_zy= atan2(dElectronPy_norm[iHit],dElectronPz_norm[iHit]);
        dElectron_psai_zx= atan2(dElectronPx_norm[iHit],dElectronPz_norm[iHit]);
        dElectron_posi_xy= atan2(dEY,dEX);
        if(fabs(yhex[iHit]-dCOM_y_e)>1e-6 || fabs(xhex[iHit]-dCOM_x_e)>1e-6)
        dElectron_psai_position[iHit] = atan2(yhex[iHit]-dCOM_y_e,xhex[iHit]-dCOM_x_e); //not used
    } // end for
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    iHit = 0;
    int inplane, iSign_x, iSign_y, iSign_z;
    double theta, dnx, dny, dnz, dn_ion, dn_electron, dn_length;
    double dpx, dpy, dpz, dp_ion, dp_electron;
    inplane = 0; iSign_x = -1; iSign_y = +1; iSign_z = -1;
    if(polarization_is_horizontal){           // horizontal polarization
        dElectron_theta[iHit] = acos(dElectronPz_norm[iHit]);
        dn_length = sin(dAngle_Width/2./180*pi);
        //  for(int it=0; it<int(180*2/dAngle_Width+0.1); it++)
        for(int it=0; it<int(180/dAngle_Width+0.1); it++){
            theta = double(it)*dAngle_Width;
            //  theta = double(it)*dAngle_Width/2;
            theta = theta*pi/180;
            dnx = sin(theta);       //versor orthogonal to the plane that contains the polarization
            dny = cos(theta);       //versor orthogonal to the plane
            dnz = 0.0;              //versor orthogonal to the plane
            dn_ion = dnx*dIon_nPx+dny*dIon_nPy+dnz*dIon_nPz;
            dn_electron = iSign_x*dnx*dElectronPx_norm[0]+iSign_y*dny*dElectronPy_norm[0]+iSign_z*dnz*dElectronPz_norm[0];
            //now checks that both ion and electron are in this plane
            if((fabs(dn_ion)<dn_length) && (fabs(dn_electron)<dn_length)){
                //if so, projects both ion and electron in such plane
                inplane = 1;
                dpx = sin(theta+pi/2);      //this vector is in the plane
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
            } // end if
        } // end if
    } else if(!polarization_is_horizontal){   // Vertical polarization
        dElectron_theta[iHit] = acos(dElectronPy_norm[iHit]);
        dn_length = sin(dAngle_Width/2./180*pi);
        for(int it=0; it<int(180/dAngle_Width+0.1); it++){
            //      for(int it=0; it<int(180*2/dAngle_Width+0.1); it++)
            theta = double(it)*dAngle_Width;
            //          theta = double(it)*dAngle_Width/2;
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
            } // end for
        } // end for
    } else{
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
    } // end if
    //Sellect event for Lucchesse'a analysis, and convert the angle into th lab frame
    double dIon_theta_lab, dIon_psai_lab;
    double dElectron_theta_lab, dElectron_psai_lab, dElectronEnergytemp;
    if(polarization_is_horizontal){
        dIon_theta_lab = dIon_theta;
        dIon_psai_lab = pi + dIon_psai_xy;
        dElectron_theta_lab = acos(iSign_z*dElectronPz_norm[0]);
        dElectron_psai_lab = pi + atan2(iSign_y*dElectronPy_norm[0],iSign_x*dElectronPx_norm[0]);
    }else if(!polarization_is_horizontal){
        dIon_theta_lab = dIon_theta;
        dIon_psai_lab = pi/2 + dIon_psai_zx;
        dElectron_theta_lab = acos(iSign_y*dElectronPy_norm[0]);
        dElectron_psai_lab = pi/2 + atan2(iSign_x*dElectronPx_norm[0],iSign_z*dElectronPz_norm[0]);
    } // end if
    dElectronEnergytemp = dElectronEnergy[0];


    ////////////////////////

    //Export valid data

    file_electron.setf(fstream::scientific);
    file_electron.precision(6);
    if(iMaster_Flag >= 0){
        for(int iHit=0; iHit<nHit; iHit++){
            file_electron.width(14);
            file_electron << dIonPx[0][iHit]/dMomentum_au << ", ";
            file_electron.width(14);
            file_electron << dIonPy[0][iHit]/dMomentum_au << ", ";
            file_electron.width(14);
            file_electron << dIonPz[0][iHit]/dMomentum_au << ", ";
            file_electron.width(14);
            file_electron << dElectronEnergy[iHit]/dElectron << ", ";
        }
        file_electron << endl;
    }

    ////////// select sigma or pi symmetry  ////////modified by macchi    2003.10.2
    ///////////////////////////////////////////////////////////////////////////////
    ////////// using these caluculated momentum ///////////////////////////////////
    ////        dIonPx_norm[iHit] = dIonPx[jn][iHit]/dIonP[iHit];  ////////////////
    ////        dIonPy_norm[iHit] = dIonPy[jn][iHit]/dIonP[iHit];  ////////////////
    ////        dIonPz_norm[iHit] = dIonPz[jn][iHit]/dIonP[iHit];  ////////////////
    ///////////////////////////////////////////////////////////////////////////////


    for(int iHit=0; iHit<1; iHit++){
        //for sigma transition
        if ((dIonPz_norm[iHit]>0.94) || (dIonPz_norm[iHit]<-0.94))
        flagmatrix = 72;
        // for pi transition
        if ((dIonPz_norm[iHit]<0.34) && (dIonPz_norm[iHit]>-0.34))
        flagmatrix = 73;
    } // end for

    hell_2: //this is just a label

    /////////////////////////////////////////////////////////////////////////////
    ///                       calculated data output
    /////////////////////////////////////////////////////////////////////////////
    pEventData->SetAt(18,nHexX1);  // 18+0
    pEventData->SetAt(19,nHexX2);  // 18+1
    pEventData->SetAt(20,nHexY1);  // 18+2
    pEventData->SetAt(21,nHexY2);  // 18+3
    pEventData->SetAt(22,nHexZ1);  // 18+4
    pEventData->SetAt(23,nHexZ2);  // 18+5
    pEventData->SetAt(24,nMCPele); // 18+6
    pEventData->SetAt(25,nMCPion); // 18+7
    pEventData->SetAt(26,nSqX1);   // 18+8
    pEventData->SetAt(27,nSqX2);   // 18+9
    pEventData->SetAt(28,nSqY1);   // 18+10
    pEventData->SetAt(29,nSqY2);   // 18+11
    pEventData->SetAt(30,nBunch);  // 18+12

    for(int iHit=0; iHit<4; iHit++){
        pEventData->SetAt(31+iHit,xhex[iHit]*1000);                 // Hex_X
        pEventData->SetAt(31+4+iHit,yhex[iHit]*1000);               // Hex_Y
        //pEventData->SetAt(31+28+iHit,dxSq[iHit]*1000);                // SQ_x (mm)
        //pEventData->SetAt(31+32+iHit,dySq[iHit]*1000);                // SQ_y (mm)
        pEventData->SetAt(31+28+iHit,dxSqN[jn][iHit]*1000);             // SQ_x (mm)
        pEventData->SetAt(31+32+iHit,dySqN[jn][iHit]*1000);             // SQ_y (mm)
        pEventData->SetAt(67+iHit,dtIon[iHit]*1.0E9);               // Ion TOF (ns)
        pEventData->SetAt(67+4+iHit,dIonEnergy[iHit]/dElectron);    // Ion Kinetic Energy
    } // end for

    pEventData->SetAt(75,dIon_Total_Energy/dElectron);              // Ion Total Kinetic Energy
    //  pEventData->SetAt(75,(dIonEnergy[0]*14/16)/dElectron);              // Ion Total Kinetic Energy

    for(int iHit=0; iHit<4; iHit++){
        pEventData->SetAt(76+iHit,dIonPx[jn][iHit]/dMomentum_au);       // Ion Px
        pEventData->SetAt(80+iHit,dIonPy[jn][iHit]/dMomentum_au);       // Ion Py
        pEventData->SetAt(84+iHit,dIonPz[jn][iHit]/dMomentum_au);       // Ion Pz
    } // end for

    pEventData->SetAt(88,dSum_of_momenta_X[jn]/dMomentum_au);       // Sum_px
    pEventData->SetAt(89,dSum_of_momenta_Y[jn]/dMomentum_au);       // Sum_py
    pEventData->SetAt(90,dSum_of_momenta_Z[jn]/dMomentum_au);       // Sum_pz


    pEventData->SetAt(91,dIon_theta*180.0/pi);                      //

    pEventData->SetAt(92,dIon_psai_xy*180.0/pi);                    //Ion_psai_xy
    pEventData->SetAt(93,dIon_psai_zy*180.0/pi);                    //Ion_psai_yz
    pEventData->SetAt(94,dIon_psai_zx*180.0/pi);                    //Ion_psai_zx

    pEventData->SetAt(95,dIon_nPx);                 // Ion_Pnx
    pEventData->SetAt(96,dIon_nPy);                 // Ion_Pny
    pEventData->SetAt(97,dIon_nPz);                 // Ion_Pnz

    //chek OK
    pEventData->SetAt(102,dElectronEnergyXY/dElectron);         // Electron Kinetic Energy
    for(int iHit=0; iHit<4; iHit++){
        pEventData->SetAt(98+iHit,dtElectron[iHit]*1.0E9);              // Electron TOF
        pEventData->SetAt(103+iHit,dElectronEnergy[iHit]/dElectron);    // Electron Kinetic Energy
        pEventData->SetAt(107+iHit,dElectronPx[iHit]/dMomentum_au);     // Electron Px
        pEventData->SetAt(111+iHit,dElectronPy[iHit]/dMomentum_au);     // Electron Py
        pEventData->SetAt(115+iHit,dElectronPz[iHit]/dMomentum_au);     // Electron Pz
        pEventData->SetAt(199+iHit,dIon_Total_Energy/dElectron + dElectronEnergy[iHit]/dElectron);  // Ion Total Kinetic Energy + Electron Kinetic Energy
    } // end for
    if(dElectronEnergy[0] >= dElectronEnergy[1]){
        dElectronEnergyHigher = dElectronEnergy[0];
        dElectronEnergyLower = dElectronEnergy[1];
    } else{
        dElectronEnergyHigher = dElectronEnergy[1];
        dElectronEnergyLower = dElectronEnergy[0];
    } // end if
    pEventData->SetAt(203,dElectronEnergyHigher/dElectron); // Electron Kinetic Energy Higher
    pEventData->SetAt(204,dElectronEnergyLower/dElectron);  // Electron Kinetic Energy Lower

    pEventData->SetAt(119,dElectronPx_norm[0]);                     // Normalized electron Px
    pEventData->SetAt(120,dElectronPy_norm[0]);                     // Normalized electron Py
    pEventData->SetAt(121,dElectronPz_norm[0]);                     // Normalized electron Pz
    pEventData->SetAt(122,dElectronP[0]/dMomentum_au);              // Sum_P_Electron

    //  pEventData->SetAt(123,dElectron_theta[0]*180.0/pi);
    //  pEventData->SetAt(124,dElectron_psai[0]*180.0/pi);
    //  pEventData->SetAt(125,dElectron_psai_position[0]*180.0/pi);
    pEventData->SetAt(123,dElectron_psai_xy*180.0/pi);
    pEventData->SetAt(124,dElectron_psai_zy*180.0/pi);
    pEventData->SetAt(125,dElectron_psai_zx*180.0/pi);

    pEventData->SetAt(126,iMaster_Flag);
    pEventData->SetAt(127,flagmatrix);

    pEventData->SetAt(128,dCOM_x[jn]/unit_milli);                             // COM_x  mm
    pEventData->SetAt(129,dCOM_y[jn]/unit_milli);                             // COM_y  mm
    pEventData->SetAt(130,dCOM_z[jn]/unit_milli);                             // COM_z  mm




    pEventData->SetAt(131,dCOM_x_e);                                // COM_e_x
    pEventData->SetAt(132,dCOM_y_e);                                // COM_e_y



    pEventData->SetAt(133,dphai/pi*180);
    pEventData->SetAt(134,dEX*1000);
    pEventData->SetAt(135,dEY*1000);

    pEventData->SetAt(136,dElectron_theta0[0]*180.0/pi);

    pEventData->SetAt(137,dElectron_posi_xy*180.0/pi);

    pEventData->SetAt(138,dIon_Electron_Angle0*180.0/pi);
    pEventData->SetAt(139,isellect);                                // COM_e_y
    pEventData->SetAt(140,dtN*1.0E9);                               // COM_e_y
    pEventData->SetAt(141,dtO*1.0E9);                               // COM_e_y

    for(int ii=0; ii<int(180/dAngle_Width+0.1); ii++){
        pEventData->SetAt(142+ii*3,dIon_Angle[ii]/pi*180);
        pEventData->SetAt(142+ii*3+1,dElectron_Angle[ii]/pi*180);
        pEventData->SetAt(142+ii*3+2,dIon_Electron_Angle[ii]/pi*180);
    }
    pEventData->SetAt(196,d_I_Angle*180.0/pi);
    pEventData->SetAt(197,d_e_I_Angle*180.0/pi);
    pEventData->SetAt(198,distancexy*1000);

    pEventData->SetAt(205,(dtIon[0]+dtIon[1])*1.0E9);
    pEventData->SetAt(206,(dtIon[0]+dtIon[2])*1.0E9);
    pEventData->SetAt(207,(dtIon[0]+dtIon[3])*1.0E9);
    pEventData->SetAt(208,(dtIon[1]+dtIon[2])*1.0E9);
    pEventData->SetAt(209,(dtIon[1]+dtIon[3])*1.0E9);
    pEventData->SetAt(210,(dtIon[2]+dtIon[3])*1.0E9);
    return;
} // end CDAN_API
