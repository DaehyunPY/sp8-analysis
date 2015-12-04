#include "Unit.h"

const double Analysis::Unit::readMass(const double _data) const {
  return _data * this->atomicMass;
}
const double Analysis::Unit::readCharge(const double _data) const {
  return _data * this->elementaryCharge;
}
const double Analysis::Unit::readTime(const double _data) const {
  return _data * this->nano;
}
const double Analysis::Unit::writeTime(const double _data) const {
  return _data / this->nano;
}
const double Analysis::Unit::readLength(const double _data) const {
  return _data * this->milli;
}
const double Analysis::Unit::writeLength(const double _data) const {
  return _data / this->milli;
}
const double Analysis::Unit::readElectricPotential(const double _data) const {
  return _data;
}
const double Analysis::Unit::readMagnaticField(const double _data) const {
  return _data * this->gauss;
}
const double Analysis::Unit::readDegree(const double _data) const {
  return _data * this->degree;
}
const double Analysis::Unit::writeDegree(const double _data) const {
  return _data / this->degree;
}
Analysis::Unit::Unit() : arg("") { return; }
Analysis::Unit::~Unit() { return; }
Analysis::Unit::Unit(const std::string &arg) : arg(arg) { return; }
const double Analysis::Unit::readElectronRestMass(const double data) const {
  return this->electronRestMass * data;
}
