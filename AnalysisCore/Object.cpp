#include "Object.h"

Analysis::Object::Object(const double &m,
                         const double &q,
                         const double &t0,
                         const double &t1)
    : ObjectFlag(), mass(m), charge(q), minOfTOF(t0), maxOfTOF(t1) {
  if( getMass() == 0e0
      && getCharge() == 0e0
      && getMinOfTOF() == 0e0
      && getMaxOfTOF() == 0e0 ) {
    setFlag(DummyObject);
  } else {
    assert(getMass() > 0e0);
    assert(getMinOfTOF() >= 0e0);
    assert(getMinOfTOF() <= getMaxOfTOF());
    setFlag(RealObject);
  }
  resetEventData();
  return;
};
Analysis::Object::~Object() { return; }
void Analysis::Object::resetEventData() {
  locationX = 0e0;
  locationY = 0e0;
  TOF = 0e0;
  momentumX = 0e0;
  momentumY = 0e0;
  momentumZ = 0e0;
  resetFlag();
  return;
}
void Analysis::Object::setLocationX(const double &x) {
  locationX = x;
  return;
}
void Analysis::Object::setLocationY(const double &y) {
  locationY = y;
  return;
}
void Analysis::Object::setTOF(const double &t) {
  TOF = t;
  if((TOF < maxOfTOF) && (TOF > minOfTOF)) {
    setFlag(WithinMasterRegion);
  } else {
    setFlag(OutOfMasterRegion);
  }
}
void Analysis::Object::setMomentumX(const double &px) {
  momentumX = px;
  return;
}
void Analysis::Object::setMomentumY(const double &py) {
  momentumY = py;
  return;
}
void Analysis::Object::setMomentumZ(const double &pz) {
  momentumZ = pz;
  return;
}
const double &Analysis::Object::getMass() const
{ 
	return mass; 
}
const double &Analysis::Object::getCharge() const
{
	return charge;
}
const double &Analysis::Object::getMinOfTOF() const { return minOfTOF; }
const double &Analysis::Object::getMaxOfTOF() const { return maxOfTOF; }
const double &Analysis::Object::getLocationX() const
{
	return locationX;
}
const double &Analysis::Object::getLocationY() const
{
	return locationY;
}
const double &Analysis::Object::getTOF() const
{
	return TOF;
}
const double &Analysis::Object::getMomentumX() const
{
	return momentumX;
}
const double &Analysis::Object::getMomentumY() const
{
	return momentumY;
}
const double &Analysis::Object::getMomentumZ() const
{
	return momentumZ;
}
const double Analysis::Object::getMomentum() const {
  return pow(pow(getMomentumX(), 2e0)
                 + pow(getMomentumY(), 2e0)
                 + pow(getMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Object::getEnergy() const {
  return pow(getMomentum(), 2e0) / (2e0 * getMass());
}
const double Analysis::Object::getMotionalDirectionX() const {
  return acos(getMomentumX() / getMomentum());
}
const double Analysis::Object::getMotionalDirectionY() const {
  return acos(getMomentumY() / getMomentum());
}
const double Analysis::Object::getMotionalDirectionZ() const {
  return acos(getMomentumZ() / getMomentum());
}
Analysis::Object::Object() : Object(0e0, 0e0, 0e0, 0e0) { return; }
const double Analysis::Object::getMomentumXY() const {
  return pow(pow(getMomentumX(), 2e0) + pow(getMomentumY(), 2e0), 0.5e0);
}
const double Analysis::Object::getMomentumYZ() const {
  return pow(pow(getMomentumY(),2e0) + pow(getMomentumZ(),2e0), 0.5e0);
}
const double Analysis::Object::getMomentumZX() const {
  return pow(pow(getMomentumZ(),2e0) + pow(getMomentumX(),2e0), 0.5e0);
}
const double Analysis::Object::getMotionalDirectionXY() const {
  return atan2(getMomentumY(), getMomentumX());
}
const double Analysis::Object::getMotionalDirectionYZ() const {
  return atan2(getMomentumZ(), getMomentumY());
}
const double Analysis::Object::getMotionalDirectionZX() const {
  return atan2(getMomentumX(), getMomentumZ());
}
const double Analysis::Object::getLocationXY() const {
  return pow(pow(getLocationX(),2e0) + pow(getLocationY(),2e0), 0.5e0);
}
const double Analysis::Object::getLocation() const {
  return getLocationXY();
}
const double Analysis::Object::getLocationalDirectionXY() const {
  return atan2(getLocationY(), getLocationX());
}
const double Analysis::Object::getLocationalDirectionX() const {
  return acos(getLocationX()/getLocation());
}
const double Analysis::Object::getLocationalDirectionY() const {
  return acos(getLocationY()/getLocation());
}
const double Analysis::Object::getLocationalDirectionYX() const {
  return atan2(getLocationX(), getLocationY());
}
const double Analysis::Object::getMotionalDirectionXZ() const {
  return atan2(getMomentumZ(), getMomentumX());
}
const double Analysis::Object::getMotionalDirectionYX() const {
  return atan2(getMomentumX(), getMomentumY());
}
const double Analysis::Object::getMotionalDirectionZY() const {
  return atan2(getMomentumY(), getMomentumZ());
}

// output
double * const Analysis::Object::outputLocX() const
{
	if (isFlag(HavingXYTData)) return new double(kUnit.writeMilliMeter(getLocationX()));
  return nullptr;
}
double * const Analysis::Object::outputLocY() const
{
	if (isFlag(HavingXYTData)) return new double(kUnit.writeMilliMeter(getLocationY()));
  return nullptr;
}
double * const Analysis::Object::outputLocR() const
{
	if (isFlag(HavingXYTData)) return new double(kUnit.writeMilliMeter(getLocation()));
  return nullptr;
}
double * const Analysis::Object::outputLocDir() const
{
	if (isFlag(HavingXYTData)) return new double(kUnit.writeDegree(getLocationalDirectionXY()));
  return nullptr;
}
double * const Analysis::Object::outputTOF() const
{
	if (isFlag(HavingXYTData)) return new double(kUnit.writeNanoSec(getTOF()));
  return nullptr;
}
double * const Analysis::Object::outputPX() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeAuMomentum(getMomentumX()));
  return nullptr;
}
double * const Analysis::Object::outputPY() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeAuMomentum(getMomentumY()));
  return nullptr;
}
double * const Analysis::Object::outputPZ() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeAuMomentum(getMomentumZ()));
  return nullptr;
}
double * const Analysis::Object::outputPXY() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeAuMomentum(getMomentumXY()));
  return nullptr;
}
double * const Analysis::Object::outputPYZ() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeAuMomentum(getMomentumYZ()));
  return nullptr;
}
double * const Analysis::Object::outputPZX() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeAuMomentum(getMomentumZX()));
  return nullptr;
}
double * const Analysis::Object::outputP() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeAuMomentum(getMomentum()));
  return nullptr;
}
double * const Analysis::Object::outputPDirX() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionX()));
  return nullptr;
}
double * const Analysis::Object::outputPDirY() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionY()));
  return nullptr;
}
double * const Analysis::Object::outputPDirZ() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionZ()));
  return nullptr;
}
double * const Analysis::Object::outputE() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeElectronVolt(getEnergy()));
  return nullptr;
}
double* const Analysis::Object::outputCosPDirZ() const
{
	if (isFlag(HavingMomentumData)) return new double(cos(getMotionalDirectionZ()));
	return nullptr;
}
double* const Analysis::Object::outputPDirXY() const
{
	if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionXY()));
	return nullptr;
}
double* const Analysis::Object::outputPDirXZ() const
{
  if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionXZ()));
  return nullptr;
}
double* const Analysis::Object::outputPDirYX() const
{
  if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionYX()));
  return nullptr;
}
double* const Analysis::Object::outputPDirYZ() const
{
  if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionYZ()));
  return nullptr;
}
double* const Analysis::Object::outputPDirZX() const
{
  if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionZX()));
  return nullptr;
}
double* const Analysis::Object::outputPDirZY() const
{
  if (isFlag(HavingMomentumData)) return new double(kUnit.writeDegree(getMotionalDirectionZY()));
  return nullptr;
}
