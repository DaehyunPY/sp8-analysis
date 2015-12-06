#include "Object.h"

Analysis::Object::Object(const double &m,
                         const double &q,
                         const double &t0,
                         const double &t1)
    : mass(m), charge(q), minOfTOF(t0), maxOfTOF(t1) {
  assert(this->getMass() >= 0e0);
  assert(this->getMinOfTOF() <= this->getMaxOfTOF());
  this->resetEventData();
  return;
};
Analysis::Object::~Object() { return; }
void Analysis::Object::resetEventData() {
  this->locationX = 0e0;
  this->locationY = 0e0;
  this->TOF = 0e0;
  this->momentumX = 0e0;
  this->momentumY = 0e0;
  this->momentumZ = 0e0;
  this->objectFlag.loadInitialFlager();
  return;
}
void Analysis::Object::setLocationX(const double &x) {
  this->locationX = x;
  return;
}
void Analysis::Object::setLocationY(const double &y) {
  this->locationY = y;
  return;
}
void Analysis::Object::setTOF(const double &t) { this->TOF = t; }
void Analysis::Object::setMomentumX(const double &px) {
  this->momentumX = px;
  return;
}
void Analysis::Object::setMomentumY(const double &py) {
  this->momentumY = py;
  return;
}
void Analysis::Object::setMomentumZ(const double &pz) {
  this->momentumZ = pz;
  return;
}
Analysis::ObjectFlag &Analysis::Object::setObjectFlagMembers() {
  return this->objectFlag;
}
const double &Analysis::Object::getMass() const { return this->mass; }
const double &Analysis::Object::getCharge() const { return this->charge; }
const double &Analysis::Object::getMinOfTOF() const { return this->minOfTOF; }
const double &Analysis::Object::getMaxOfTOF() const { return this->maxOfTOF; }
const double &Analysis::Object::getLocationX() const { return this->locationX; }
const double &Analysis::Object::getLocationY() const { return this->locationY; }
const double &Analysis::Object::getTOF() const { return this->TOF; }
const double &Analysis::Object::getMomentumX() const { return this->momentumX; }
const double &Analysis::Object::getMomentumY() const { return this->momentumY; }
const double &Analysis::Object::getMomentumZ() const { return this->momentumZ; }
const double Analysis::Object::getAbsoluteMomentum() const {
  return pow(pow(this->getMomentumX(), 2e0)
                 + pow(this->getMomentumY(), 2e0)
                 + pow(this->getMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Object::getEnergy() const {
  return pow(this->getAbsoluteMomentum(), 2e0) / (2e0 * this->getMass());
}
const Analysis::ObjectFlag &Analysis::Object::getObjectFlag() const {
  return this->objectFlag;
}
const bool Analysis::Object::isWithinMasterRegion() const {
  return (this->getTOF() < this->getMaxOfTOF())
      && (this->getTOF() > this->getMinOfTOF());
}
const double Analysis::Object::getDirectionX() const {
  return this->getMomentumX() / this->getAbsoluteMomentum();
}
const double Analysis::Object::getDirectionY() const {
  return this->getMomentumY() / this->getAbsoluteMomentum();
}
const double Analysis::Object::getDirectionZ() const {
  return this->getMomentumZ() / this->getAbsoluteMomentum();
}
const double Analysis::Object::getLocationX(const Analysis::Unit &unit) const {
  return unit.writeLength(this->getLocationX());
}
const double Analysis::Object::getLocationY(const Analysis::Unit &unit) const {
  return unit.writeLength(this->getLocationY());
}
const double Analysis::Object::getTOF(const Analysis::Unit &unit) const {
  return unit.writeTime(this->getTOF());
}
const double Analysis::Object::getMomentumX(const Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getMomentumX());
}
const double Analysis::Object::getMomentumY(const Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getMomentumY());
}
const double Analysis::Object::getMomentumZ(const Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getMomentumZ());
}
const double Analysis::Object::getMomentum(const Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getAbsoluteMomentum());
}
const double Analysis::Object::getEnergy(const Analysis::Unit &unit) const {
  return unit.writeEnergy(this->getEnergy());
}
Analysis::Object::Object() : Object(0e0, 0e0, 0e0, 0e0) { return; }
