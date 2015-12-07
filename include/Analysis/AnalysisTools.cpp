#include "AnalysisTools.h"

Analysis::AnalysisTools::AnalysisTools(const EquipmentParameters &equip,
                                       const IonParameters &ion,
                                       const ElectronParameters &elec)
    : equipmentParameters(equip), ionParameters(ion),
      electronParameters(elec) {
  this->resetCounter();
  return;
}
Analysis::AnalysisTools::~AnalysisTools() { return; }
void Analysis::AnalysisTools::resetCounter() {
  this->eventNumber = 0;
  return;
}
const double Analysis::AnalysisTools::calculateTOF(const Ion &ion,
                                                   const double &p0) const {
  const double &len1 = this->getEquipmentParameters().getLengthOfL1();
  const double &len2 = this->getEquipmentParameters().getLengthOfL2();
  const double &len3 = this->getEquipmentParameters().getLengthOfL3();
  const double &E1 = this->getEquipmentParameters().getElectricFiledOfL1();
  const double &E2 = this->getEquipmentParameters().getElectricFiledOfL2();
  const double &E3 = this->getEquipmentParameters().getElectricFiledOfL3();
  const double &m = ion.getMass();
  const double &q = ion.getCharge();
  const double p1 = pow(pow(p0, 2e0) + 2e0 * m * q * E1 * len1, 0.5e0);
  const double p2 = pow(pow(p1, 2e0) + 2e0 * m * q * E2 * len2, 0.5e0);
  const double p3 = pow(pow(p2, 2e0) + 2e0 * m * q * E3 * len3, 0.5e0);
  const double t1 = (p1 - p0) / q / E1;
  const double t2 = (p2 - p1) / q / E2;
  const double t3 = (p3 - p2) / q / E3;
  return t1 + t2 + t3;
}
const double Analysis::AnalysisTools::calculateDiffTOF(const Ion &ion,
                                                       const double &p0) const {
  const double &len1 = this->getEquipmentParameters().getLengthOfL1();
  const double &len2 = this->getEquipmentParameters().getLengthOfL2();
  const double &len3 = this->getEquipmentParameters().getLengthOfL3();
  const double &E1 = this->getEquipmentParameters().getElectricFiledOfL1();
  const double &E2 = this->getEquipmentParameters().getElectricFiledOfL2();
  const double &E3 = this->getEquipmentParameters().getElectricFiledOfL3();
  const double &m = ion.getMass();
  const double &q = ion.getCharge();
  return
      -1.0 * pow(p0, 1.0)
          * pow(2.0 * E2 * len2 * m * q
                    + pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), 1.0), -0.5)
          / (E3 * q)
          + 1.0 * pow(p0, 1.0)
              * pow(2.0 * E3 * len3 * m * q
                        + pow(2.0 * E2 * len2 * m * q
                                  + pow(2.0 * E1 * len1 * m * q
                                            + pow(p0, 2.0), 1.0), 1.0), -0.5)
              / (E3 * q)
          - 1.0 * pow(p0, 1.0)
              * pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), -0.5)
              / (E2 * q)
          + 1.0 * pow(p0, 1.0)
              * pow(2.0 * E2 * len2 * m * q
                        + pow(2.0 * E1 * len1 * m * q
                                  + pow(p0, 2.0), 1.0), -0.5) / (E2 * q)
          + 1.0 * pow(p0, 1.0)
              * pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), -0.5)
              / (E1 * q) - 1 / (E1 * q);
}
const double Analysis::AnalysisTools::calculateMomentumZ(const Ion &ion)
const {
  double x0, x1, dx, f, df;
  const double &f0 = ion.getTOF();
  x1 = 0e0;
  for (int i = 0; i < 100; i++) {
    x0 = x1;
    f = calculateTOF(ion, x0) - f0;
    df = calculateDiffTOF(ion, x0);
    dx = -f / df;
    x1 = x0 + dx;
    if (x0 == x1) {
      break;
    }
  }
  return x1;
}
const double Analysis::AnalysisTools::calculateTOF(
    const Analysis::Electron &electron, const double &p0) const {
  const double &len1 = this->getEquipmentParameters().getLengthOfD1();
  const double &len2 = this->getEquipmentParameters().getLengthOfD2();
  const double &E1 = this->getEquipmentParameters().getElectricFiledOfD1();
  const double &m = electron.getMass();
  const double &q = electron.getCharge();
  const double p1 = pow(pow(p0, 2e0) + 2e0 * m * q * E1 * len1, 0.5e0);
  const double t1 = (p1 - p0) / q / E1;
  const double t2 = len2 * m / p1;
  return t1 + t2;
}
const double Analysis::AnalysisTools::calculateDiffTOF
    (const Electron &elec, const double &p0) const {
  const double &len1 = this->getEquipmentParameters().getLengthOfD1();
  const double &len2 = this->getEquipmentParameters().getLengthOfD2();
  const double &E1 = this->getEquipmentParameters().getElectricFiledOfD1();
  const double &m = elec.getMass();
  const double &q = elec.getCharge();
  return -1.0 * len2 * m * pow(p0, 1.0)
      * pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), -1.5)
      + 1.0 * pow(p0, 1.0) * pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), -0.5)
          / (E1 * q)
      - 1 / (E1 * q);
}
const double Analysis::AnalysisTools::calculateMomentumZ
    (const Electron &elec) const {
  double x0, x1, dx, f, df;
  const double &f0 = elec.getTOF();
  x1 = 0.e0;
  for (int i = 0; i < 100; i++) {
    x0 = x1;
    f = calculateTOF(elec, x0) - f0;
    df = calculateDiffTOF(elec, x0);
    dx = -f / df;
    x1 = x0 + dx;
    if (x0 == x1) {
      break;
    }
  }
  return x1;
}
const Analysis::EquipmentParameters
&Analysis::AnalysisTools::getEquipmentParameters() const {
  return this->equipmentParameters;
}

const Analysis::IonParameters &Analysis::AnalysisTools::getIonParameters()
const {
  return this->ionParameters;
}

const Analysis::ElectronParameters
&Analysis::AnalysisTools::getElectronParameters() const {
  return this->electronParameters;
}
void Analysis::AnalysisTools::loadEventCounter() { this->eventNumber += 1; }
Analysis::AnalysisTools::AnalysisTools(const Analysis::Unit &unit,
                                       const Analysis::JSONReader &reader)
    : AnalysisTools(
    Analysis::EquipmentParameters(unit, reader),
    Analysis::IonParameters(unit, reader),
    Analysis::ElectronParameters(unit, reader)) {
  return;
}
const Analysis::AnalysisTools::XY Analysis::AnalysisTools::calculateMomentumXY(
    const Object &obj) const {
  const double pi = atan2(0e0, -1e0);
  const double &m = obj.getMass();
  const double &q = obj.getCharge();
  const double &t = obj.getTOF();
  const double &x = obj.getLocationX();
  const double &y = obj.getLocationY();
  const double &B = this->getEquipmentParameters().getMagneticFiled();
  if (float(B) == 0e0) {
    return {m * x / t, m * y / t};
  } else {
    const double period
        = this->calculatePeriodOfCycle(obj);
    double theta = t / period;
    theta = theta - floor(theta);
    theta = 2e0 * pi * theta;
    theta = 0.5e0 * theta;
    const double p0 = fabs(q * B / sin(theta) / 2e0);
    return this->calculateRotation({p0 * x, p0 * y}, theta);
  }
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Ion &ion,
                                                   const double &x1,
                                                   const double &y1,
                                                   const double &t1,
                                                   const int &f1) const {
  const double &theta = this->getIonParameters().getAngleOfDetector();
  const double &dx = this->getIonParameters().getPixelSizeOfX();
  const double &dy = this->getIonParameters().getPixelSizeOfY();
  const double &deadTime = this->getIonParameters().getDeadTime();
  const double &x0 = this->getIonParameters().getXZeroOfCOM();
  const double &y0 = this->getIonParameters().getYZeroOfCOM();
  const double &t0 = this->getIonParameters().getTimeZeroOfTOF();
  const XY xy = this->calculateRotation({x1 * dx, y1 * dy}, theta);
  const double x = xy.x - x0;
  const double y = xy.y - y0;
  const double t = t1 - t0;
  ion.setLocationX(x);
  ion.setLocationY(y);
  ion.setTOF(t);
  if (ion.getTOF() > deadTime) {
    ion.setObjectFlagMembers().loadDeadFlager();
  } else {
    if (ion.isWithinMasterRegion()) {
      ion.setObjectFlagMembers().loadWithinMasterRegionFlager();
    } else {
      ion.setObjectFlagMembers().loadNotWithinMasterRegionFlager();
    }
  }
  return;
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Electron &electron,
                                                   const double &x1,
                                                   const double &y1,
                                                   const double &t1,
                                                   const int &flag1) const {
  const double &theta = this->getElectronParameters().getAngleOfDetector();
  const double &dx = this->getElectronParameters().getPixelSizeOfX();
  const double &dy = this->getElectronParameters().getPixelSizeOfY();
  const double &deadTime = this->getElectronParameters().getDeadTime();
  const double &x0 = this->getElectronParameters().getXZeroOfCOM();
  const double &y0 = this->getElectronParameters().getYZeroOfCOM();
  const double &t0 = this->getElectronParameters().getTimeZeroOfTOF();
  const XY xy = this->calculateRotation({x1 * dx, y1 * dy}, theta);
  const double x = xy.x - x0;
  const double y = xy.y - y0;
  const double t = t1 - t0;
  electron.setLocationX(x);
  electron.setLocationY(y);
  electron.setTOF(t);
  if (electron.getTOF() > deadTime) {
    electron.setObjectFlagMembers().loadDeadFlager();
  } else {
    if (electron.isWithinMasterRegion()) {
      electron.setObjectFlagMembers().loadWithinMasterRegionFlager();
    } else {
      electron.setObjectFlagMembers().loadNotWithinMasterRegionFlager();
    }
  }
  return;
}
void Analysis::AnalysisTools::loadMomentumCalculator(Analysis::Ion &ion) const {
  const XY &pxy = this->calculateMomentumXY(ion);
  const double &pz = this->calculateMomentumZ(ion);
  ion.setMomentumX(pxy.x);
  ion.setMomentumY(pxy.y);
  ion.setMomentumZ(pz);
  return;
}
void Analysis::AnalysisTools::loadMomentumCalculator(
    Analysis::Electron &elec) const {
  const XY &pxy = this->calculateMomentumXY(elec);
  const double &pz = this->calculateMomentumZ(elec);
  elec.setMomentumX(pxy.x);
  elec.setMomentumY(pxy.y);
  elec.setMomentumZ(pz);
  return;
}
const int &Analysis::AnalysisTools::getEventNumber() const {
  return this->eventNumber;
}
const double Analysis::AnalysisTools::calculatePeriodOfCycle(
    const Object &object) const {
  const double pi = atan2(0e0, -1e0);
  const double &m = object.getMass();
  const double &q = object.getCharge();
  const double &B = this->getEquipmentParameters().getMagneticFiled();
  return 2e0 * pi * m / B / q;
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Ion &ion,
                                                   const Unit &unit,
                                                   const double &x,
                                                   const double &y,
                                                   const double &t,
                                                   const int &f) const {
  this->loadEventDataInputer(ion,
                             unit.readLength(x),
                             unit.readLength(y),
                             unit.readTime(t),
                             f);
  return;
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Electron &electron,
                                                   const Unit &unit,
                                                   const double &x,
                                                   const double &y,
                                                   const double &t,
                                                   const int &f) const {
  this->loadEventDataInputer(electron,
                             unit.readLength(x),
                             unit.readLength(y),
                             unit.readTime(t),
                             f);
  return;
}
void Analysis::AnalysisTools::loadMomentumCalculator(Analysis::Ions &ions) const {
  const int &n = ions.getNumberOfHits();
  for (int i = 0; i < n; i++) {
    loadMomentumCalculator(ions.setIonMembers(i));
  }
  return;
}
void Analysis::AnalysisTools::loadMomentumCalculator(
    Analysis::Electrons &elecs) const {
  const int &n = elecs.getNumberOfHits();
  for (int i = 0; i < n; i++) {
    loadMomentumCalculator(elecs.setElectronMembers(i));
  }
  return;
}

#ifdef FOR_COBOLDPC2002
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Ion &ion,
                                                   const Unit &unit,
                                                   const LMFReader &reader,
                                                   const int &iHit) const {
  this->loadEventDataInputer(ion,
                             unit,
                             reader.getAt(1, 1, iHit)/1000e0,
                             reader.getAt(2, 0, iHit)/1000e0,
                             reader.getAt(2, 1, iHit)/1000e0,
                             0);
  return;
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Electron &electron,
                                                   const Unit &unit,
                                                   const LMFReader &reader,
                                                   const int &iHit) const {
  this->loadEventDataInputer(electron,
                             unit,
                             reader.getAt(0, 0, iHit)/1000e0,
                             reader.getAt(0, 1, iHit)/1000e0,
                             reader.getAt(1, 0, iHit)/1000e0,
                             0);
  return;
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Ions &ions,
                                                   const Unit &unit,
                                                   const LMFReader &reader) const {
  const int &n = ions.getNumberOfHits();
  const int &m = ions.getNumberOfHitsUsed();
  for(int i = 0; i < n; i++) {
    loadEventDataInputer(ions.setIonMembers(i), unit, reader, i);
  }
  for(int i = n; i < m; i++) {
    loadEventDataInputer(ions.setDummyIonMembers(i), unit, reader, i);
  }
  return;
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Electrons &electrons,
                                                   const Unit &unit,
                                                   const LMFReader &reader) const {
  const int &n = electrons.getNumberOfHits();
  const int &m = electrons.getNumberOfHitsUsed();
  for(int i = 0; i < n; i++) {
    loadEventDataInputer(electrons.setElectronMembers(i), unit, reader, i);
  }
  for(int i = n; i < m; i++) {
    loadEventDataInputer(electrons.setDummyElectronMembers(i), unit, reader, i);
  }
  return;
}
#endif

const double Analysis::AnalysisTools::calculateTOF(const Analysis::Unit &unit,
                                                   const Analysis::Ion &ion,
                                                   const double &d) const {
  return unit.writeTime(this->calculateTOF(ion, d));
}
const double Analysis::AnalysisTools::calculateTOF(const Analysis::Unit &unit,
                                                   const Analysis::Electron &electron,
                                                   const double &d) const {
  return unit.writeTime(this->calculateTOF(electron, d));
}
const double Analysis::AnalysisTools::calculatePeriodOfCycle(
    const Analysis::Unit &unit,
    const Analysis::Object &object) const {
  return unit.writeTime(this->calculatePeriodOfCycle(object));
}
const Analysis::AnalysisTools::XY Analysis::AnalysisTools::calculateRotation(
    const XY &xy,
    const double &theta) const {
  return {xy.x * cos(theta) - xy.y * sin(theta),
          xy.x * sin(theta) + xy.y * cos(theta)};
}
