#include <assert.h>
#include "Object.h"

Analysis::Object::Object(const double &m,
                         const double &q,
                         const double &t0,
                         const double &t1)
    : mass(m), charge(q), minOfTOF(t0), maxOfTOF(t1) {
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
  objectFlag.loadInitialFlager();
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
void Analysis::Object::setTOF(const double &t) { this->TOF = t; }
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
Analysis::ObjectFlag &Analysis::Object::setObjectFlagMembers() {
  return objectFlag;
}
const double &Analysis::Object::getMass() const { return mass; }
const double &Analysis::Object::getCharge() const { return charge; }
const double &Analysis::Object::getMinOfTOF() const { return minOfTOF; }
const double &Analysis::Object::getMaxOfTOF() const { return maxOfTOF; }
const double &Analysis::Object::getLocationX() const { return locationX; }
const double &Analysis::Object::getLocationY() const { return locationY; }
const double &Analysis::Object::getTOF() const { return TOF; }
const double &Analysis::Object::getMomentumX() const { return momentumX; }
const double &Analysis::Object::getMomentumY() const { return momentumY; }
const double &Analysis::Object::getMomentumZ() const { return momentumZ; }
const double Analysis::Object::getMomentum() const {
  return pow(pow(getMomentumX(), 2e0)
                 + pow(getMomentumY(), 2e0)
                 + pow(getMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Object::getEnergy() const {
  return pow(getMomentum(), 2e0) / (2e0 * getMass());
}
const Analysis::ObjectFlag &Analysis::Object::getObjectFlag() const {
  return objectFlag;
}
const bool Analysis::Object::isWithinMasterRegion() const {
  return (getTOF() < getMaxOfTOF()) && (getTOF() > getMinOfTOF());
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
const double Analysis::Object::getLocationX(const Analysis::Unit &unit) const {
  return unit.writeLength(getLocationX());
}
const double Analysis::Object::getLocationY(const Analysis::Unit &unit) const {
  return unit.writeLength(getLocationY());
}
const double Analysis::Object::getTOF(const Analysis::Unit &unit) const {
  return unit.writeTime(getTOF());
}
const double Analysis::Object::getMomentumX(const Analysis::Unit &unit) const {
  return unit.writeMomentum(getMomentumX());
}
const double Analysis::Object::getMomentumY(const Analysis::Unit
                                                               &unit) const {
  return unit.writeMomentum(getMomentumY());
}
const double Analysis::Object::getMomentumZ(const Analysis::Unit
                                                               &unit) const {
  return unit.writeMomentum(getMomentumZ());
}
const double Analysis::Object::getMomentum(const Analysis::Unit
                                                              &unit) const {
  return unit.writeMomentum(getMomentum());
}
const double Analysis::Object::getEnergy(const Analysis::Unit &unit)
const {
  return unit.writeEnergy(getEnergy());
}
Analysis::Object::Object() : Object(0e0, 0e0, 0e0, 0e0) { return; }
const double Analysis::Object::getMomentumXY() const {
  return pow(pow(getMomentumX(), 2e0) + pow(getMomentumY(), 2e0), 0.5e0);
}
const double Analysis::Object::getMomentumXY(const Analysis::Unit
                                                                &unit) const {
  return unit.writeMomentum(getMomentumXY());
}
const bool Analysis::Object::isDead() const {
  return getObjectFlag().isDead();
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
const double Analysis::Object::getMomentumYZ(const Analysis::Unit &unit) const {
  return unit.writeMomentum(getMomentumYZ());
}
const double Analysis::Object::getMomentumZX(const Analysis::Unit &unit) const {
  return unit.writeMomentum(getMomentumZX());
}
const double Analysis::Object::getMotionalDirectionX(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionX());
}
const double Analysis::Object::getMotionalDirectionY(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionY());
}
const double Analysis::Object::getMotionalDirectionZ(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionZ());
}
const double Analysis::Object::getMotionalDirectionXY(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionXY());
}
const double Analysis::Object::getMotionalDirectionYZ(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionYZ());
}
const double Analysis::Object::getMotionalDirectionZX(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionZX());
}
const double Analysis::Object::getLocationXY() const {
  return pow(pow(getLocationX(),2e0) + pow(getLocationY(),2e0), 0.5e0);
}
const double Analysis::Object::getLocationXY(const Analysis::Unit &unit) const {
  return unit.writeLength(getLocationXY());
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
const double Analysis::Object::getLocation(const Analysis::Unit &unit) const {
  return unit.writeLength(getLocation());
}
const double Analysis::Object::getLocationalDirectionXY(const Analysis::Unit &unit) const {
  return unit.writeDegree(getLocationalDirectionXY());
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
const double Analysis::Object::getLocationalDirectionX(const Analysis::Unit &unit) const {
  return unit.writeDegree(getLocationalDirectionX());
}
const double Analysis::Object::getLocationalDirectionY(const Analysis::Unit &unit) const {
  return unit.writeDegree(getLocationalDirectionY());
}
const double Analysis::Object::getLocationalDirectionYX(const Analysis::Unit &unit) const {
  return unit.writeDegree(getLocationalDirectionYX());
}
const double Analysis::Object::getMotionalDirectionXZ(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionXZ());
}
const double Analysis::Object::getMotionalDirectionYX(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionYX());
}
const double Analysis::Object::getMotionalDirectionZY(const Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionZY());
}
