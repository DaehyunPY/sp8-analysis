#ifndef VARIABLES_C
#define VARIABLES_C

TObject *targetObj; 
TCanvas *targetCanvas; 
enum Axis {
   axisX, axisY, axisZ
} targetAxis; 
double region1, region2; 
bool wasCrosshairOn; 

#endif 
