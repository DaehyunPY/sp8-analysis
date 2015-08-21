CDAN_API void AnalysisFinalize(CDoubleArray *pEventData,CDoubleArray *pParameters, CDoubleArray *pWeighParameter){
    using namespace std;

    double tctemp[3];
    double dTcyc;
    tctemp[0] = tof(0, 0, dMass[0], dCharge[0]);
    tctemp[1] = tof(0, 0, dMass[1], dCharge[1]);
    tctemp[2] = electof(0);
    dTcyc = 2.0 * pi * dElectronMass / dMagnetic_Field / dElectron;


    ////////////////////////////////////////////////////////////////////////////
    // outptu
    file_electron.close();
    file_log.close();

    return;
} // end CDAN_API
