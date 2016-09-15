#include "Object.h"

Analysis::Object::Object(const FlagName f,
                         const JSONReader &reader,
                         const std::string prefix) : ObjectFlag() {
  assert(f == IonObject || f == ElecObject);
  setFlag(f);
  if (f == IonObject) {
	  double m = 0; 
	  if (reader.hasMember(prefix+"mass")) m = reader.getDoubleAt(prefix+"mass");
	  if (m == 0) {
		  setFlag(DummyObject);
		  mass = 0;
		  charge = 0;
	  } else if (m > 0) {
		  double q = reader.getDoubleAt(prefix + "charge");
		  assert(q > 0e0);
		  setFlag(RealObject);
		  mass = kUnit.readAtomicMass(m);
		  charge = kUnit.readElementaryCharge(q);
	} else assert(false);
  } else { // ElecObject
	  setFlag(RealObject);
	  mass = kUnit.readElectronRestMass(1e0);
	  charge = kUnit.readElementaryCharge(1e0);
  }
  double t0 = reader.getDoubleAt(prefix+"TOF", 0);
  double t1 = reader.getDoubleAt(prefix+"TOF", 1);
  assert(0e0 <= t0 && t0 <= t1);
  minTOF = kUnit.readNanoSec(t0);
  maxTOF = kUnit.readNanoSec(t1);

  // read options
  auto read2DoublesIfItIs = [](const JSONReader &rd, double (*unit)(double)) {
    return [&rd, unit](const std::string pos, double &v0, double &v1) {
      if (rd.hasMember(pos)) {
        v0 = unit(rd.getDoubleAt(pos, 0));
        v1 = unit(rd.getDoubleAt(pos, 1));
      } else {
        v0 = 0;
        v1 = 0;
      }
    };
  };
  auto readAt = read2DoublesIfItIs(reader, [&](double v)->double{ return kUnit.readAuMomentum(v); });
  readAt(prefix + "dx_and_dy", dx, dy);
  readAt(prefix + "phi", frPhi, toPhi);
  const auto str = "conservation_raw.";
  readAt(prefix + str + "x", frPx, toPx);
  readAt(prefix + str + "y", frPy, toPy);
  readAt(prefix + str + "z", frPz, toPz);
  readAt(prefix + str + "r", frPr, toPr);
  readAt(prefix + str + "e", frE, toE);
  resetEventData();
  return;
}
Analysis::Object::Object(const FlagName f1, const FlagName f2,
                         double m, double q,
                         const double t0, const double t1) : ObjectFlag() {
  assert(f1 == RealObject || f1 == DummyObject);
  assert(f2 == IonObject || f2 == ElecObject);
  setFlag(f1);
  setFlag(f2);
  if (f1 == DummyObject) {
    mass = 0;
    charge= 0;
    minTOF = 0;
    maxTOF = 0;
  } else { // RealObject
    if (f2 == IonObject) {
      assert(m > 0e0);
      assert(q > 0e0);
      mass = kUnit.readAtomicMass(m);
      charge = kUnit.readElementaryCharge(q);
    } else { // ElecObject
      mass = kUnit.readElectronRestMass(1);
      charge = kUnit.readElementaryCharge(1);
    }
    assert(t0 <= t1);
    minTOF = kUnit.readNanoSec(t0);
    maxTOF = kUnit.readNanoSec(t1);
  }
  dx = 0;
  dy = 0;
  frPhi = 0;
  toPhi = 0;
  frPx = 0;
  toPx = 0;
  frPy = 0;
  toPy = 0;
  frPz = 0;
  toPz = 0;
  frPr = 0;
  toPr = 0;
  frE = 0;
  toE = 0;
  resetEventData();
  return;
}
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
  locationX = x + dx;
  return;
}
void Analysis::Object::setLocationY(const double &y) {
  locationY = y + dy;
  return;
}
void Analysis::Object::setTOF(const double &t) {
  TOF = t;
  if (isFlag(DummyObject) && minTOF == 0 && maxTOF == 0) setFlag(WithinMasterRegion);
  else if (minTOF <= TOF && TOF <= maxTOF) setFlag(WithinMasterRegion);
  else setFlag(OutOfMasterRegion);
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
  if (!(frPhi==0 && toPhi==0)) {
    const double phi = getMotionalDirectionZ();
    if (!(frPhi <= phi && phi <= toPhi)) setFlag(OutOfMasterRegion);
  }
  return;
}
const double &Analysis::Object::getMass() const {
  return mass;
}
const double &Analysis::Object::getCharge() const {
  return charge;
}
const double &Analysis::Object::getMinOfTOF() const { return minTOF; }
const double &Analysis::Object::getMaxOfTOF() const { return maxTOF; }
const double &Analysis::Object::getLocationX() const {
  return locationX;
}
const double &Analysis::Object::getLocationY() const {
  return locationY;
}
const double &Analysis::Object::getTOF() const {
  return TOF;
}
const double &Analysis::Object::getMomentumX() const {
  return momentumX;
}
const double &Analysis::Object::getMomentumY() const {
  return momentumY;
}
const double &Analysis::Object::getMomentumZ() const {
  return momentumZ;
}
const double Analysis::Object::getMomentum() const {
  return pow(pow(getMomentumX(), 2e0)
                 + pow(getMomentumY(), 2e0)
                 + pow(getMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Object::getEnergy() const {
  return pow(getMomentum(), 2e0) / (2e0 * getMass());
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
const double Analysis::Object::getMomentumXY() const {
  return pow(pow(getMomentumX(), 2e0) + pow(getMomentumY(), 2e0), 0.5e0);
}
const double Analysis::Object::getMomentumYZ() const {
  return pow(pow(getMomentumY(), 2e0) + pow(getMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Object::getMomentumZX() const {
  return pow(pow(getMomentumZ(), 2e0) + pow(getMomentumX(), 2e0), 0.5e0);
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
const double Analysis::Object::getLocationXY() const {
  return pow(pow(getLocationX(), 2e0) + pow(getLocationY(), 2e0), 0.5e0);
}
const double Analysis::Object::getLocation() const {
  return getLocationXY();
}
const double Analysis::Object::getLocationalDirectionXY() const {
  return atan2(getLocationY(), getLocationX());
}
const double Analysis::Object::getLocationalDirectionX() const {
  return acos(getLocationX() / getLocation());
}
const double Analysis::Object::getLocationalDirectionY() const {
  return acos(getLocationY() / getLocation());
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

// output
double *const Analysis::Object::outputLocX() const {
  if (isFlag(HavingXYTData))
    return new double(kUnit.writeMilliMeter(getLocationX()));
  return nullptr;
}
double *const Analysis::Object::outputLocY() const {
  if (isFlag(HavingXYTData))
    return new double(kUnit.writeMilliMeter(getLocationY()));
  return nullptr;
}
double *const Analysis::Object::outputLocR() const {
  if (isFlag(HavingXYTData))
    return new double(kUnit.writeMilliMeter(getLocation()));
  return nullptr;
}
double *const Analysis::Object::outputLocDir() const {
  if (isFlag(HavingXYTData))
    return new double(kUnit.writeDegree(getLocationalDirectionXY()));
  return nullptr;
}
double *const Analysis::Object::outputTOF() const {
  if (isFlag(HavingXYTData))
    return new double(kUnit.writeNanoSec(getTOF()));
  return nullptr;
}
double *const Analysis::Object::outputPX() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeAuMomentum(getMomentumX()));
  return nullptr;
}
double *const Analysis::Object::outputPY() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeAuMomentum(getMomentumY()));
  return nullptr;
}
double *const Analysis::Object::outputPZ() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeAuMomentum(getMomentumZ()));
  return nullptr;
}
double *const Analysis::Object::outputPXY() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeAuMomentum(getMomentumXY()));
  return nullptr;
}
double *const Analysis::Object::outputPYZ() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeAuMomentum(getMomentumYZ()));
  return nullptr;
}
double *const Analysis::Object::outputPZX() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeAuMomentum(getMomentumZX()));
  return nullptr;
}
double *const Analysis::Object::outputP() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeAuMomentum(getMomentum()));
  return nullptr;
}
double *const Analysis::Object::outputPDirX() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionX()));
  return nullptr;
}
double *const Analysis::Object::outputPDirY() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionY()));
  return nullptr;
}
double *const Analysis::Object::outputPDirZ() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionZ()));
  return nullptr;
}
double *const Analysis::Object::outputE() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeElectronVolt(getEnergy()));
  return nullptr;
}
double *const Analysis::Object::outputCosPDirZ() const {
  if (isFlag(HavingMomentumData))
    return new double(cos(getMotionalDirectionZ()));
  return nullptr;
}
double *const Analysis::Object::outputPDirXY() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionXY()));
  return nullptr;
}
double *const Analysis::Object::outputPDirXZ() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionXZ()));
  return nullptr;
}
double *const Analysis::Object::outputPDirYX() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionYX()));
  return nullptr;
}
double *const Analysis::Object::outputPDirYZ() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionYZ()));
  return nullptr;
}
double *const Analysis::Object::outputPDirZX() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionZX()));
  return nullptr;
}
double *const Analysis::Object::outputPDirZY() const {
  if (isFlag(HavingMomentumData))
    return new double(kUnit.writeDegree(getMotionalDirectionZY()));
  return nullptr;
}
Analysis::Object Analysis::Object::getCopy() const {
  return *this;
}
double *const Analysis::Object::outputCosPDirX() const {
  if (isFlag(HavingMomentumData))
    return new double(cos(getMotionalDirectionX()));
  return nullptr;
}
double *const Analysis::Object::outputCosPDirY() const {
  if (isFlag(HavingMomentumData))
    return new double(cos(getMotionalDirectionY()));
  return nullptr;
}
bool Analysis::Object::isMomentumAndEnergyConserved() const {
    auto isBtw = [](double v, double fr, double to) {
      if (fr == 0 && to == 0) return true;
      else return fr <= v && v <= to;
    };
    auto isValidPx = [=](double p) { return isBtw(p, frPx, toPx); };
    auto isValidPy = [=](double p) { return isBtw(p, frPy, toPy); };
    auto isValidPz = [=](double p) { return isBtw(p, frPz, toPz); };
    auto isValidPr = [=](double p) { return isBtw(p, frPr, toPr); };
    auto isValidE = [=](double e) { return isBtw(e, frE, toE); };
    return isValidPx(getMomentumX())
        && isValidPy(getMomentumY())
        && isValidPz(getMomentumZ())
        && isValidPr(getMomentum())
        && isValidE(getEnergy());
}



