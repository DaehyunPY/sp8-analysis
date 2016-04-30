#include <assert.h>
#include "Object.h"

#define ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1 if(isOutOfFrameOfBasicData()) { return outOfFrame; }
#define ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2 if(isOutOfFrameOfMomentumData()) { return outOfFrame; }

Analysis::Object::Object(const double &m,
                         const double &q,
                         const double &t0,
                         const double &t1)
    : ObjectFlag(), mass(m), charge(q), minOfTOF(t0), maxOfTOF(t1) {
  assert(getMass() >= 0e0);
  assert(getMinOfTOF() <= getMaxOfTOF());
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
    setWithinMasterRegion();
  } else {
    setOutOfMasterRegion();
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
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
	return locationX;
}
const double &Analysis::Object::getLocationY() const
{
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
	return locationY;
}
const double &Analysis::Object::getTOF() const
{
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
	return TOF;
}
const double &Analysis::Object::getMomentumX() const
{
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
	return momentumX;
}
const double &Analysis::Object::getMomentumY() const
{
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
	return momentumY;
}
const double &Analysis::Object::getMomentumZ() const
{
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
	return momentumZ;
}
const double Analysis::Object::getMomentum() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(pow(getMomentumX(), 2e0)
                 + pow(getMomentumY(), 2e0)
                 + pow(getMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Object::getEnergy() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(getMomentum(), 2e0) / (2e0 * getMass());
}
const double Analysis::Object::getMotionalDirectionX() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return acos(getMomentumX() / getMomentum());
}
const double Analysis::Object::getMotionalDirectionY() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return acos(getMomentumY() / getMomentum());
}
const double Analysis::Object::getMotionalDirectionZ() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return acos(getMomentumZ() / getMomentum());
}
const double Analysis::Object::getLocationX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeMilliMeter(getLocationX());
}
const double Analysis::Object::getLocationY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeMilliMeter(getLocationY());
}
const double Analysis::Object::getTOF(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeNanoSec(getTOF());
}
const double Analysis::Object::getMomentumX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeAuMomentum(getMomentumX());
}
const double Analysis::Object::getMomentumY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeAuMomentum(getMomentumY());
}
const double Analysis::Object::getMomentumZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeAuMomentum(getMomentumZ());
}
const double Analysis::Object::getMomentum(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeAuMomentum(getMomentum());
}
const double Analysis::Object::getEnergy(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeElectronVolt(getEnergy());
}
Analysis::Object::Object() : Object(0e0, 0e0, 0e0, 0e0) { return; }
const double Analysis::Object::getMomentumXY() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(pow(getMomentumX(), 2e0) + pow(getMomentumY(), 2e0), 0.5e0);
}
const double Analysis::Object::getMomentumXY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeAuMomentum(getMomentumXY());
}
const double Analysis::Object::getMomentumYZ() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(pow(getMomentumY(),2e0) + pow(getMomentumZ(),2e0), 0.5e0);
}
const double Analysis::Object::getMomentumZX() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return pow(pow(getMomentumZ(),2e0) + pow(getMomentumX(),2e0), 0.5e0);
}
const double Analysis::Object::getMotionalDirectionXY() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumY(), getMomentumX());
}
const double Analysis::Object::getMotionalDirectionYZ() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumZ(), getMomentumY());
}
const double Analysis::Object::getMotionalDirectionZX() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumX(), getMomentumZ());
}
const double Analysis::Object::getMomentumYZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeAuMomentum(getMomentumYZ());
}
const double Analysis::Object::getMomentumZX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeAuMomentum(getMomentumZX());
}
const double Analysis::Object::getMotionalDirectionX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionX());
}
const double Analysis::Object::getMotionalDirectionY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionY());
}
const double Analysis::Object::getMotionalDirectionZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionZ());
}
const double Analysis::Object::getMotionalDirectionXY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionXY());
}
const double Analysis::Object::getMotionalDirectionYZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionYZ());
}
const double Analysis::Object::getMotionalDirectionZX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionZX());
}
const double Analysis::Object::getLocationXY() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return pow(pow(getLocationX(),2e0) + pow(getLocationY(),2e0), 0.5e0);
}
const double Analysis::Object::getLocationXY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeMilliMeter(getLocationXY());
}
const double Analysis::Object::getLocation() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return getLocationXY();
}
const double Analysis::Object::getLocationalDirectionXY() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return atan2(getLocationY(), getLocationX());
}
const double Analysis::Object::getLocationalDirectionX() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return acos(getLocationX()/getLocation());
}
const double Analysis::Object::getLocationalDirectionY() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return acos(getLocationY()/getLocation());
}
const double Analysis::Object::getLocation(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeMilliMeter(getLocation());
}
const double Analysis::Object::getLocationalDirectionXY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeDegree(getLocationalDirectionXY());
}
const double Analysis::Object::getLocationalDirectionYX() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return atan2(getLocationX(), getLocationY());
}
const double Analysis::Object::getMotionalDirectionXZ() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumZ(), getMomentumX());
}
const double Analysis::Object::getMotionalDirectionYX() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumX(), getMomentumY());
}
const double Analysis::Object::getMotionalDirectionZY() const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return atan2(getMomentumY(), getMomentumZ());
}
const double Analysis::Object::getLocationalDirectionX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeDegree(getLocationalDirectionX());
}
const double Analysis::Object::getLocationalDirectionY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeDegree(getLocationalDirectionY());
}
const double Analysis::Object::getLocationalDirectionYX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS1
  return unit.writeDegree(getLocationalDirectionYX());
}
const double Analysis::Object::getMotionalDirectionXZ(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionXZ());
}
const double Analysis::Object::getMotionalDirectionYX(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionYX());
}
const double Analysis::Object::getMotionalDirectionZY(const Analysis::Unit &unit) const {
	ANALYSIS_OBJECT_RETURN_OUT_OF_FRAME_IF_IT_IS2
  return unit.writeDegree(getMotionalDirectionZY());
}
const double Analysis::Object::getMass(const Analysis::Unit &unit) const {
  return unit.writeAtomicMass(getMass());
}
const double Analysis::Object::getCharge(const Analysis::Unit &unit) const {
  return unit.writeElementaryCharge(getCharge());
}
const double Analysis::Object::getMinOfTOF(const Analysis::Unit &unit) const {
  return unit.writeNanoSec(getMinOfTOF());
}
const double Analysis::Object::getMaxOfTOF(const Analysis::Unit &unit) const {
  return unit.writeNanoSec(getMaxOfTOF());
}
