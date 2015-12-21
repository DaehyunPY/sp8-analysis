#include "Unit.h"

Analysis::Unit::Unit() { return; }
Analysis::Unit::~Unit() { return; }
const double Analysis::Unit::readAtomicMass(const double d) const {
  return d * atomicMass;
}
const double Analysis::Unit::readCharge(const double d) const {
  return d * elementaryCharge;
}
const double Analysis::Unit::readTime(const double d) const {
  return d * nano;
}
const double Analysis::Unit::writeTime(const double d) const {
  return d / nano;
}
const double Analysis::Unit::readLength(const double d) const {
  return d * milli;
}

const double Analysis::Unit::readMomentum(const double d) const
{
	return d * auMomentum;
}

const double Analysis::Unit::writeLength(const double d) const {
  return d / milli;
}
const double Analysis::Unit::readElectricPotential(const double d) const {
  return d;
}
const double Analysis::Unit::readMagnaticField(const double d) const {
  return d * gauss;
}
const double Analysis::Unit::readDegree(const double d) const {
  return d * degree;
}
const double __unused Analysis::Unit::writeDegree(const double d) const {
  return d / degree;
}
const double Analysis::Unit::readElectronRestMass(const double d) const {
  return d * electronRestMass;
}
const double Analysis::Unit::writeMomentum(const double d) const {
  return d / auMomentum;
}
const double Analysis::Unit::writeEnergy(const double d) const {
  return d / elementaryCharge;
}

