#include "Unit.h"

const double Analysis::Unit::kPi = atan2(0e0, -1e0);
const double Analysis::Unit::kAtomicMass = 1.66053892173e-27;
const double Analysis::Unit::kSpeedOfLight = 299792458;
const double Analysis::Unit::kElectronRestMass = 9.1093829140e-31;
const double Analysis::Unit::kElementaryCharge = 1.60217656535e-19;
const double Analysis::Unit::kReducedPlanckConstant = 1.05457172647e-34;
const double Analysis::Unit::kCoulombForceConstant = 8.9875517873681e9;
const double Analysis::Unit::kFineStructureConstant =
    pow(kElementaryCharge, 2) * kCoulombForceConstant / kReducedPlanckConstant
        / kSpeedOfLight;
const double Analysis::Unit::kBohrRadius =
    kReducedPlanckConstant / kElectronRestMass / kSpeedOfLight
        / kFineStructureConstant;
const double Analysis::Unit::kHartreeEnergy =
    pow(kFineStructureConstant, 2) * kElectronRestMass * pow(kSpeedOfLight, 2);
const double Analysis::Unit::kAuTime = kReducedPlanckConstant / kHartreeEnergy;
const double Analysis::Unit::kAuMomentum = kReducedPlanckConstant / kBohrRadius;
const double Analysis::Unit::kAuElectricPotential = kHartreeEnergy / kElementaryCharge;
const double Analysis::Unit::kAuMagneticField = kReducedPlanckConstant / kElementaryCharge / pow(kBohrRadius, 2);
const double Analysis::Unit::kGauss = 1e-4;
const double Analysis::Unit::kNano = 1e-9;
const double Analysis::Unit::kMilli = 1e-3;
const double Analysis::Unit::kDegree = kPi / 180e0;
Analysis::Unit::Unit() { return; }
Analysis::Unit::~Unit() { return; }
const double Analysis::Unit::readAtomicMass(const double d) const {
  return d * kAtomicMass / kElectronRestMass;
}
const double Analysis::Unit::readElementaryCharge(const double d) const {
  return d;
}
const double Analysis::Unit::readNanoSec(const double d) const {
  return d * kNano / kAuTime;
}
const double Analysis::Unit::writeNanoSec(const double d) const {
  return d * kAuTime / kNano;
}
const double Analysis::Unit::readMilliMeter(const double d) const {
  return d * kMilli / kBohrRadius;
}
const double Analysis::Unit::readAuMomentum(const double d) const {
  return d;
}
const double Analysis::Unit::writeMilliMeter(const double d) const {
  return d * kBohrRadius / kMilli;
}
const double Analysis::Unit::readVolt(const double d) const {
  return d / kAuElectricPotential;
}
const double Analysis::Unit::readGauss(const double d) const {
  return d * kGauss / kAuMagneticField;
}
const double Analysis::Unit::readDegree(const double d) const {
  return d * kDegree;
}
const double Analysis::Unit::writeDegree(const double d) const {
  return d / kDegree;
}
const double Analysis::Unit::readElectronRestMass(const double d) const {
  return d;
}
const double Analysis::Unit::writeAuMomentum(const double d) const {
  return d;
}
const double Analysis::Unit::writeElectronVolt(const double d) const {
  return d * kHartreeEnergy / kElementaryCharge;
}
const double Analysis::Unit::writeVolt(const double d) const {
  return d * kAuElectricPotential;
}
const double Analysis::Unit::writeGauss(const double d) const {
  return d * kAuMagneticField / kGauss;
}
const double Analysis::Unit::writeElectronRestMass(const double d) const {
  return d;
}
const double Analysis::Unit::writeAtomicMass(const double d) const {
  return d * kElectronRestMass / kAtomicMass;
}
const double Analysis::Unit::writeElementaryCharge(const double d) const {
  return d;
}
