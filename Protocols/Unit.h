#ifndef ANALYSIS_UNIT_H
#define ANALYSIS_UNIT_H

#include <math.h>

namespace Analysis {
class Unit {
 private:
  static const double kPi;
  static const double kAtomicMass;
  static const double kSpeedOfLight;
  static const double kElectronRestMass;
  static const double kElementaryCharge;
  static const double kReducedPlanckConstant;
  static const double kCoulombForceConstant;
  static const double kFineStructureConstant;
  static const double kBohrRadius;
  static const double kHartreeEnergy;
  static const double kAuTime;
  static const double kAuMomentum;
  static const double kAuElectricPotential;
  static const double kAuMagneticField;
  static const double kGauss;
  static const double kNano;
  static const double kMilli;
  static const double kDegree;

 public:
  // read
  const double readElectronRestMass(const double d) const;
  const double readAtomicMass(const double d) const;
  const double readElementaryCharge(const double d) const;
  const double readNanoSec(const double d) const;
  const double readMilliMeter(const double d) const;
  const double readAuMomentum(const double d) const;
  const double readVolt(const double d) const;
  const double readGauss(const double d) const; // magnetic field
  const double readElectronVolt(const double d) const; // energy
  const double readDegree(const double d) const;
  // write
  const double writeElectronRestMass(const double d) const;
  const double writeAtomicMass(const double d) const;
  const double writeElementaryCharge(const double d) const;
  const double writeNanoSec(const double d) const;
  const double writeMilliMeter(const double d) const;
  const double writeAuMomentum(const double d) const;
  const double writeVolt(const double d) const;
  const double writeGauss(const double d) const; // magnetic field
  const double writeElectronVolt(const double d) const; // energy
  const double writeDegree(const double d) const;

 public:
  Unit();
  ~Unit();
};
static Unit kUnit;
}

#endif