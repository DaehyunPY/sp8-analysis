#include "Unit.h"

Analysis::Unit::Unit() { return; }
Analysis::Unit::~Unit() { return; }
const double Analysis::Unit::readAtomicMass(const double d) const {
  return d * this->atomicMass;
}
const double Analysis::Unit::readCharge(const double d) const {
  return d * this->elementaryCharge;
}
const double Analysis::Unit::readTime(const double d) const {
  return d * this->nano;
}
const double Analysis::Unit::writeTime(const double d) const {
  return d / this->nano;
}
const double Analysis::Unit::readLength(const double d) const {
  return d * this->milli;
}
const double Analysis::Unit::writeLength(const double d) const {
  return d / this->milli;
}
const double Analysis::Unit::readElectricPotential(const double d) const {
  return d;
}
const double Analysis::Unit::readMagnaticField(const double d) const {
  return d * this->gauss;
}
const double Analysis::Unit::readDegree(const double d) const {
  return d * this->degree;
}
const double Analysis::Unit::writeDegree(const double d) const {
  return d / this->degree;
}
const double Analysis::Unit::readElectronRestMass(const double d) const {
  return d * this->electronRestMass;
}
const double Analysis::Unit::writeMomentum(const double d) const {
  return d / this->auMomentum;
}
const double Analysis::Unit::writeEnergy(const double d) const {
  return d / this->elementaryCharge;
}

