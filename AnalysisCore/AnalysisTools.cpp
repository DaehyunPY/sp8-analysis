#include "AnalysisTools.h"

Analysis::AnalysisTools::AnalysisTools(const EquipmentParameters &equip,
                                       const ObjectParameters &ion,
                                       const ObjectParameters &elec)
    : equipParameters(equip),
      ionParameters(ion),
      elecParameters(elec) {
  if (elecParameters.getParameterType()
      == ObjectParameters::legacy_elec_parameters_not_corrected)
    elecParameters.correctLegacyParameters(
        calculateTOF(
            Object(ObjectFlag::RealObject, ObjectFlag::ElecObject), 0));
  resetCounter();
  return;
}
Analysis::AnalysisTools::~AnalysisTools() { return; }
void Analysis::AnalysisTools::resetCounter() {
  this->eventNumber = 0;
  return;
}
const double Analysis::AnalysisTools::calculateTOF(const Object obj,
                                                   const double p0) const {
  if (obj.isFlag(ObjectFlag::IonObject)) {
    const double &len1 = getEquipmentParameters().getLengthOfL1();
    const double &len2 = getEquipmentParameters().getLengthOfL2();
    const double &len3 = getEquipmentParameters().getLengthOfL3();
    const double &E1 = getEquipmentParameters().getElectricFiledOfL1();
    const double &E2 = getEquipmentParameters().getElectricFiledOfL2();
    const double &E3 = getEquipmentParameters().getElectricFiledOfL3();
    const double &m = obj.getMass();
    const double &q = obj.getCharge();
    const double p1 = pow(pow(p0, 2e0) + 2e0 * m * q * E1 * len1, 0.5e0);
    const double p2 = pow(pow(p1, 2e0) + 2e0 * m * q * E2 * len2, 0.5e0);
    const double p3 = pow(pow(p2, 2e0) + 2e0 * m * q * E3 * len3, 0.5e0);
    const double t1 = (p1 - p0) / q / E1;
    const double t2 = (p2 - p1) / q / E2;
    const double t3 = (p3 - p2) / q / E3;
    return t1 + t2 + t3;
  } else if (obj.isFlag(ObjectFlag::ElecObject)) {
    const double &len1 = getEquipmentParameters().getLengthOfD1();
    const double &len2 = getEquipmentParameters().getLengthOfD2();
    const double &E1 = getEquipmentParameters().getElectricFiledOfD1();
    const double &m = obj.getMass();
    const double &q = obj.getCharge();
    const double p1 = pow(pow(p0, 2e0) + 2e0 * m * q * E1 * len1, 0.5e0);
    const double t1 = (p1 - p0) / q / E1;
    const double t2 = len2 * m / p1;
    return t1 + t2;
  } else {
    assert(false);
  }
}
const double Analysis::AnalysisTools::calculateDiffTOF(const Object &obj,
                                                       const double &p0) const {
  if (obj.isFlag(ObjectFlag::IonObject)) {
    const double &len1 = getEquipmentParameters().getLengthOfL1();
    const double &len2 = getEquipmentParameters().getLengthOfL2();
    const double &len3 = getEquipmentParameters().getLengthOfL3();
    const double &E1 = getEquipmentParameters().getElectricFiledOfL1();
    const double &E2 = getEquipmentParameters().getElectricFiledOfL2();
    const double &E3 = getEquipmentParameters().getElectricFiledOfL3();
    const double &m = obj.getMass();
    const double &q = obj.getCharge();
    return -1.0 * pow(p0, 1.0)
        * pow(2.0 * E2 * len2 * m * q + pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), 1.0), -0.5) / (E3 * q)
        + 1.0 * pow(p0, 1.0)
            * pow(2.0 * E3 * len3 * m * q
                      + pow(2.0 * E2 * len2 * m * q + pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), 1.0), 1.0), -0.5)
            / (E3 * q)
        - 1.0 * pow(p0, 1.0) * pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), -0.5) / (E2 * q)
        + 1.0 * pow(p0, 1.0)
            * pow(2.0 * E2 * len2 * m * q + pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), 1.0), -0.5) / (E2 * q)
        + 1.0 * pow(p0, 1.0) * pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), -0.5) / (E1 * q) - 1 / (E1 * q);
  } else if (obj.isFlag(ObjectFlag::ElecObject)) {
    const double &len1 = getEquipmentParameters().getLengthOfD1();
    const double &len2 = getEquipmentParameters().getLengthOfD2();
    const double &E1 = getEquipmentParameters().getElectricFiledOfD1();
    const double &m = obj.getMass();
    const double &q = obj.getCharge();
    return -1.0 * len2 * m * pow(p0, 1.0) * pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), -1.5)
        + 1.0 * pow(p0, 1.0) * pow(2.0 * E1 * len1 * m * q + pow(p0, 2.0), -0.5) / (E1 * q)
        - 1 / (E1 * q);
  } else {
    assert(false);
  }
}
const double Analysis::AnalysisTools::calculateMomentumZ(const Object &obj,
                                                         bool &info) const {
  if (obj.isFlag(ObjectFlag::IonObject)) {
    double x0, x1, dx, f, df;
    const double &f0 = obj.getTOF();
    x1 = 0e0;
    info = false;
    for (int i = 0; i < LIMITATION_NUMBER_OF_LOOP; i++) {
      x0 = x1;
      f = calculateTOF(obj, x0) - f0;
      df = calculateDiffTOF(obj, x0);
      dx = -f / df;
      x1 = x0 + dx;
      if (x0 == x1) {
        info = true;
        return x1;
      }
    }
    return 0e0;
  } else if (obj.isFlag(ObjectFlag::ElecObject)) {
    double x0, x1, dx, f, df;
    const double &f0 = obj.getTOF();
    x1 = 0e0;
    info = false;
    for (int i = 0; i < LIMITATION_NUMBER_OF_LOOP; i++) {
      x0 = x1;
      f = calculateTOF(obj, x0) - f0;
      df = calculateDiffTOF(obj, x0);
      dx = -f / df;
      x1 = x0 + dx;
      if (x0 == x1) {
        info = true;
        return x1;
      }
    }
    return 0e0;
  } else {
    info = false;
    assert(false);
  }
}
const Analysis::EquipmentParameters
&Analysis::AnalysisTools::getEquipmentParameters() const {
  return this->equipParameters;
}

const Analysis::ObjectParameters &Analysis::AnalysisTools::getIonParameters()
const {
  return this->ionParameters;
}

const Analysis::ObjectParameters
&Analysis::AnalysisTools::getElectronParameters() const {
  return this->elecParameters;
}
void Analysis::AnalysisTools::loadEventCounter() { this->eventNumber += 1; }
Analysis::AnalysisTools::AnalysisTools(const Analysis::Unit &unit,
                                       const Analysis::JSONReader &reader)
    : AnalysisTools(Analysis::EquipmentParameters(unit, reader),
                    Analysis::ObjectParameters(reader, "ion_parameters."),
                    Analysis::ObjectParameters(reader, "electron_parameters.")) {
  return;
}
const Analysis::AnalysisTools::XY Analysis::AnalysisTools::calculateMomentumXY(
    const Object &object) const {
  const double pi = atan2(0e0, -1e0);
  const double &m = object.getMass();
  const double &q = object.getCharge();
  const double &t = object.getTOF();
  const double &x = object.getLocationX();
  const double &y = object.getLocationY();
  const double &B = getEquipmentParameters().getMagneticFiled();
  if (float(calculateFrequencyOfCycle(object))
      == 0e0) { // small magnetic filed
    return {m * x / t, m * y / t};
  } else { // big magnetic filed 
    const double period = calculatePeriodOfCycle(object);
    double theta = t / period;
    theta = 2e0 * pi * (theta - int(theta));
    theta = 0.5e0 * theta;
    const double p0 = fabs(q * B / sin(theta) / 2e0);
    return calculateRotation({p0 * x, p0 * y}, theta);
  }
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Object &obj,
                                                   const double &x1,
                                                   const double &y1,
                                                   const double &t1,
                                                   const int &f1) const {
  // set par
  const ObjectParameters *par = nullptr;
  if (obj.isFlag(ObjectFlag::IonObject)) par = &getIonParameters();
  else if (obj.isFlag(ObjectFlag::ElecObject)) par = &getElectronParameters();
  else assert(false);

  // read par
  const double &theta = par->getAngleOfDetector();
  const double &dx = par->getPixelSizeOfX();
  const double &dy = par->getPixelSizeOfY();
  const double &deadTime = par->getDeadTime();
  const double &x0 = par->getXZeroOfCOM();
  const double &y0 = par->getYZeroOfCOM();
  const double &t0 = par->getTimeZeroOfTOF();

  const XY xy = calculateRotation({x1, y1}, theta);
  const double x = dx * (xy.x-x0);
  const double y = dy * (xy.y-y0);
  const double t = t1 - t0;
  obj.setLocationX(x);
  obj.setLocationY(y);
  obj.setTOF(t);
  obj.setFlag(ObjectFlag::ResortFlag, f1);
  if (obj.getTOF() > deadTime || obj.getTOF() < 0e0) {
    obj.setFlag(ObjectFlag::Dead);
    obj.setFlag(ObjectFlag::HavingNotProperData);
  } else {
    obj.setFlag(ObjectFlag::HavingXYTData);
  }
}
void Analysis::AnalysisTools::loadMomentumCalculator(Object &obj) const {
  if (obj.isFlag(ObjectFlag::DummyObject)) return;
  if (!obj.isFlag(ObjectFlag::WithinMasterRegion)) return;
  if (obj.isFlag(ObjectFlag::IonObject) || obj.isFlag(ObjectFlag::ElecObject)) {
    bool isHavingProperPz;
    const XY &pxy = calculateMomentumXY(obj);
    const double &pz = calculateMomentumZ(obj, isHavingProperPz);
    if (isHavingProperPz) {
      obj.setMomentumX(pxy.x);
      obj.setMomentumY(pxy.y);
      obj.setMomentumZ(pz);
      obj.setFlag(ObjectFlag::HavingMomentumData);
      if (!obj.isMomentumAndEnergyConserved()) obj.setFlag(ObjectFlag::OutOfMasterRegion);
    } else obj.setFlag(ObjectFlag::OutOfMasterRegion);
  } else assert(false);
}
const int &Analysis::AnalysisTools::getEventNumber() const {
  return eventNumber;
}
const double Analysis::AnalysisTools::calculatePeriodOfCycle(
    const Object &object) const {
  return calculatePeriodOfCycle(object.getMass(),
                                object.getCharge(),
                                getEquipmentParameters().getMagneticFiled());
}
void Analysis::AnalysisTools::loadMomentumCalculator(Objects &objs) const {
  const int &n = objs.getNumberOfObjects();
  for (int i = 0; i < n; i++) loadMomentumCalculator(objs.setObjectMembers(i));
  if (!objs.isMomentumAndEnergyConserved()) objs.setAllFlag(ObjectFlag::OutOfMasterRegion);
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Object &obj,
                                                   const EventDataReader &reader,
                                                   const int &iHit) const {
  if (obj.isFlag(ObjectFlag::IonObject))
    loadEventDataInputer(obj,
                         kUnit.readMilliMeter(reader.getEventDataAt(EventDataReader::IonX, iHit)),
                         kUnit.readMilliMeter(reader.getEventDataAt(EventDataReader::IonY, iHit)),
                         kUnit.readNanoSec(reader.getEventDataAt(EventDataReader::IonT, iHit)),
                         reader.getFlagDataAt(EventDataReader::IonFlag, iHit));
  else if (obj.isFlag(ObjectFlag::ElecObject))
    loadEventDataInputer(obj,
                         kUnit.readMilliMeter(reader.getEventDataAt(EventDataReader::ElecX, iHit)),
                         kUnit.readMilliMeter(reader.getEventDataAt(EventDataReader::ElecY, iHit)),
                         kUnit.readNanoSec(reader.getEventDataAt(EventDataReader::ElecT, iHit)),
                         reader.getFlagDataAt(EventDataReader::ElecFlag, iHit));
}
void Analysis::AnalysisTools::loadEventDataInputer(Analysis::Objects &objs,
                                                   const EventDataReader &reader) const {
  // todo: need to review
  const int m = objs.getNumberOfRealOrDummyObjects();
  for (int i = 0; i < m; i++) {
    loadEventDataInputer(objs.setRealOrDummyObjectMembers(i), reader, i);
  }
  if (objs.isType(Objects::ions)) {
    const int n = reader.getNumObjs(EventDataReader::IonNum);
    for (int i=n; i<m; i++) {
      objs.setRealOrDummyObjectMembers(i).setFlag(Object::Dead);
      objs.setRealOrDummyObjectMembers(i).setFlag(ObjectFlag::HavingNotProperData);
    }
  } else if (objs.isType(Objects::elecs)) {
    const int n = reader.getNumObjs(EventDataReader::ElecNum);
    for (int i=n; i<m; i++) {
      objs.setRealOrDummyObjectMembers(i).setFlag(Object::Dead);
      objs.setRealOrDummyObjectMembers(i).setFlag(ObjectFlag::HavingNotProperData);
    }
  }
}
const double Analysis::AnalysisTools::calculateFrequencyOfCycle(const double &m,
                                                                const double &q,
                                                                const double &B) const {
  const double pi = atan2(0e0, -1e0);
  return 0.5e0 / pi / m * B * q;
}

const double
Analysis::AnalysisTools::calculateFrequencyOfCycle(const Object &object) const {
  return calculateFrequencyOfCycle(object.getMass(),
                                   object.getCharge(),
                                   getEquipmentParameters().getMagneticFiled());
}

const double Analysis::AnalysisTools::calculatePeriodOfCycle(const double &m,
                                                             const double &q,
                                                             const double &B) const {
  const double pi = atan2(0e0, -1e0);
  return 2e0 * pi * m / B / q;
}
const Analysis::AnalysisTools::XY Analysis::AnalysisTools::calculateRotation(
    const XY &xy,
    const double &theta) const {
  return {xy.x * cos(theta) - xy.y * sin(theta),
          xy.x * sin(theta) + xy.y * cos(theta)};
}
