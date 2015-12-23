//
// Created by Daehyun You on 12/4/15.
//

#include <assert.h>
#include "Objects.h"

Analysis::Objects::Objects(const int &n)
    : numberOfHits(n), numberOfHitsUsed(n) {
  assert(n > 0 && n <= this->maximumOfHits);
  return;
}
Analysis::Objects::Objects(const int &n, const int &m)
    : numberOfHits(n), numberOfHitsUsed(m) {
  assert(n <= m);
  assert(n > 0 && m <= this->maximumOfHits);
  return;
}
Analysis::Objects::~Objects() {
  return;
}
void Analysis::Objects::setObject(const int &i, Object &object) {
  assert(this->isRealObject(i));
  this->pObject[i] = &object;
  return;
}
void Analysis::Objects::setDummyObject(const int &i, Analysis::Object &object) {
  assert(this->isDummyObject(i));
  this->pObject[i] = &object;
}
const int &Analysis::Objects::getNumberOfObjects() const {
  return this->numberOfHits;
}
const double Analysis::Objects::getLocationX() const {
  const int &n = getNumberOfObjects();
  long double sum1 = 0e0, sum2 = 0e0;
  for (int i = 0; i < n; i++) {
    const double tmp = getObject(i).getMass() / getObject(i).getTOF();
    sum1 += tmp;
    sum2 += tmp * getObject(i).getLocationX();
  }
  return double(sum2 / sum1);
}
const double Analysis::Objects::getLocationY() const {
  const int &n = getNumberOfObjects();
  long double sum1 = 0e0, sum2 = 0e0;
  for (int i = 0; i < n; i++) {
    const double tmp = getObject(i).getMass() / getObject(i).getTOF();
    sum1 += tmp;
    sum2 += tmp * getObject(i).getLocationY();
  }
  return double(sum2 / sum1);
}
const double Analysis::Objects::getMomentumX() const {
  const int &n = this->getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumX();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getMomentumY() const {
  const int &n = this->getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumY();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getMomentumZ() const {
  const int &n = this->getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumZ();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getEnergy() const {
  const int &n = this->getNumberOfObjects();
  long double sumOfEnergy = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfEnergy += this->getObject(i).getEnergy();
  }
  return double(sumOfEnergy);
}
const Analysis::Object &Analysis::Objects::getObject(const int &i) const {
  assert(this->isRealObject(i));
  return *(this->pObject[i]);
}
const Analysis::Object &Analysis::Objects::getDummyObject(const int &i) const {
  assert(this->isDummyObject(i));
  return *(this->pObject[i]);
}

const double Analysis::Objects::getMomentum() const {
  return pow(pow(this->getMomentumX(), 2e0)
                 + pow(this->getMomentumY(), 2e0)
                 + pow(this->getMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Objects::getMotionalDirectionX() const {
  return acos(getMomentumX() / getMomentum());
}
const double Analysis::Objects::getMotionalDirectionY() const {
  return acos(getMomentumY() / getMomentum());
}
const double Analysis::Objects::getMotionalDirectionZ() const {
  return acos(getMomentumZ() / getMomentum());
}
void Analysis::Objects::resetEventData() {
  const int &n = this->getNumberOfObjects();
  const int &m = this->getNumberOfRealOrDummyObjects();
  for (int i = 0; i < n; i++) {
    this->setObjectMembers(i).resetEventData();
  }
  for (int i = n; i < m; i++) {
    this->setDummyObjectMembers(i).resetEventData();
  }
  return;
}
Analysis::Object &Analysis::Objects::setObjectMembers(const int &i) {
  assert(this->isRealObject(i));
  return *(this->pObject[i]);
}
Analysis::Object &Analysis::Objects::setDummyObjectMembers(const int &i) {
  assert(this->isDummyObject(i));
  return *(this->pObject[i]);
}
const double Analysis::Objects::getLocationX(Analysis::Unit &unit) const {
  return unit.writeLength(this->getLocationX());
}
const double Analysis::Objects::getLocationY(Analysis::Unit &unit) const {
  return unit.writeLength(this->getLocationY());
}
const double Analysis::Objects::getMomentumX(Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getMomentumX());
}
const double Analysis::Objects::getMomentumY(Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getMomentumY());
}
const double Analysis::Objects::getMomentumZ(Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getMomentumZ());
}
const double Analysis::Objects::getMomentum(Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getMomentum());
}
const double Analysis::Objects::getEnergy(Analysis::Unit &unit) const {
  return unit.writeEnergy(this->getEnergy());
}
const int &Analysis::Objects::getNumberOfRealOrDummyObjects() const {
  return this->numberOfHitsUsed;
}
const bool Analysis::Objects::isDummyObject(const int &i) const {
  const int &n = getNumberOfObjects();
  const int &m = getNumberOfRealOrDummyObjects();
  return (i >= n) && (i < m);
}
const bool Analysis::Objects::isRealObject(const int &i) const {
  const int &n = getNumberOfObjects();
  const int &m = getNumberOfRealOrDummyObjects();
  return (i >= 0) && (i < n);
}
const bool Analysis::Objects::existDeadRealOrDummyObject() const {
  return existDeadObject() || existDeadDummyObject();
}
const bool Analysis::Objects::existDeadObject() const {
  return getNumberOfDeadObjects() != 0;
}
const bool Analysis::Objects::isAllWithinMasterRegion() const {
  bool b = true;
  for(int i = 0; i < numberOfHits; i++) {
    b = b && getObject(i).getObjectFlag().isWithinMasterRegion();
  }
  return b;
}
const int Analysis::Objects::getNumberOfDeadObjects() const {
  int n = 0;
  for (int i = 0; i < numberOfHits; i++) {
    if (getObject(i).getObjectFlag().isDead()) {
      n += 1;
    }
  }
  return n;
}
const int Analysis::Objects::getNumberOfDeadDummyObjects() const {
  int n = 0;
  for (int i = numberOfHits; i < numberOfHitsUsed; i++) {
    if (getDummyObject(i).getObjectFlag().isDead()) {
      n += 1;
    }
  }
  return n;
}
const int Analysis::Objects::getNumberOfDeadRealOrDummyObjects() const {
  return getNumberOfDeadObjects() + getNumberOfDeadDummyObjects();
}
const bool Analysis::Objects::existDeadDummyObject() const {
  return getNumberOfDeadDummyObjects() != 0;
}
const bool Analysis::Objects::isAllDeadObjects() const {
  return getNumberOfDeadObjects() == getNumberOfObjects();
}
const bool Analysis::Objects::isAllDeadDummyObjects() const {
  return getNumberOfDeadDummyObjects() == getNumberOfDeadObjects();
}
const bool Analysis::Objects::isAllDeadRealAndDummyObjects() const {
  return getNumberOfDeadRealOrDummyObjects() == getNumberOfRealOrDummyObjects();
}
const double Analysis::Objects::getMomentumXY() const {
  return pow(pow(getMomentumX(), 2e0)
                 + pow(getMomentumY(), 2e0), 0.5e0);
}
const double Analysis::Objects::getMomentumXY(Analysis::Unit &unit) const {
  return unit.writeMomentum(getMomentumXY());
}
const int Analysis::Objects::getNumberOfDummyObject() const {
  return getNumberOfRealOrDummyObjects()-getNumberOfObjects();
}
const double Analysis::Objects::getMomentumYZ() const {
  return pow(pow(getMomentumY(), 2e0) + pow(getMomentumZ(), 2e0),
             0.5e0);
}
const double Analysis::Objects::getMomentumZX() const {
  return pow(pow(getMomentumZ(), 2e0) + pow(getMomentumX(), 2e0),
             0.5e0);
}
const double Analysis::Objects::getMotionalDirectionXY() const {
  return atan2(getMomentumY(), getMomentumX());
}
const double Analysis::Objects::getMotionalDirectionYZ() const {
  return atan2(getMomentumZ(), getMomentumY());
}
const double Analysis::Objects::getMotionalDirectionZX() const {
  return atan2(getMomentumX(), getMomentumZ());
}
const double Analysis::Objects::getMomentumYZ(Analysis::Unit &unit) const {
  return unit.writeMomentum(getMomentumYZ());
}
const double Analysis::Objects::getMomentumZX(Analysis::Unit &unit) const {
  return unit.writeMomentum(getMomentumZX());
}
const double Analysis::Objects::getMotionalDirectionX(Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionX());
}
const double Analysis::Objects::getMotionalDirectionY(Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionY());
}
const double Analysis::Objects::getMotionalDirectionZ(Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionZ());
}
const double Analysis::Objects::getMotionalDirectionXY(Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionXY());
}
const double Analysis::Objects::getMotionalDirectionYZ(Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionYZ());
}
const double Analysis::Objects::getMotionalDirectionZX(Analysis::Unit &unit) const {
  return unit.writeDegree((getMotionalDirectionZX()));
}
const double Analysis::Objects::getSumOfTOF(int &i1, int &i2) const {
  return getRealOrDummyObject(i1).getTOF() + getRealOrDummyObject(i2).getTOF();
}
const double Analysis::Objects::getSumOfTOF(Analysis::Unit &unit,
                                            int &i1,
                                            int &i2) const {
  return unit.writeTime(getSumOfTOF(i1,i2));
}
const Analysis::Object &Analysis::Objects::getRealOrDummyObject(const int &i) const {
  const Object *pObject;
  if(isRealObject(i)) {
    pObject = &getObject(i);
  } else if(isDummyObject(i)) {
    pObject = &getDummyObject(i);
  } else {
    assert(false);
    pObject = new Object();
  }
  return *pObject;
}
Analysis::Object &Analysis::Objects::setRealOrDummyObjectMembers(const int &i) {
  Object *pObject;
  if(isRealObject(i)) {
    pObject = &setObjectMembers(i);
  } else if(isDummyObject(i)) {
    pObject = &setDummyObjectMembers(i);
  } else {
    assert(false);
    pObject = new Object();
  }
  return *pObject;
}
const bool Analysis::Objects::isRealOrDummyObject(const int &i) const {
  return isRealObject(i) || isDummyObject(i);
}
const double Analysis::Objects::getLocationXY() const {
  return pow(pow(getLocationX(),2e0) + pow(getLocationY(),2e0),0.5e0);
}
const double Analysis::Objects::getLocation() const {
  return getLocationXY();
}
const double Analysis::Objects::getLocationalDirectionX() const {
  return acos(getLocationX()/getLocation());
}
const double Analysis::Objects::getLocationalDirectionY() const {
  return acos(getLocationY()/getLocation());
}
const double Analysis::Objects::getLocationalDirectionXY() const {
  return atan2(getLocationY(), getLocationX());
}
const double Analysis::Objects::getLocationDirectionX(Analysis::Unit &unit) const {
  return unit.writeDegree(getLocationalDirectionX());
}
const double Analysis::Objects::getLocaitonDirectionY(Analysis::Unit &unit) const {
  return unit.writeDegree(getLocationalDirectionY());
}
const double Analysis::Objects::getLocationDirectionXY(Analysis::Unit &unit) const {
  return unit.writeDegree(getLocationalDirectionXY());
}
const double Analysis::Objects::getLocationalDirectionYX() const {
  return atan2(getLocationX(), getLocationY());
}
const double Analysis::Objects::getLocationDirectionYX(Analysis::Unit &unit) const {
  return unit.writeDegree(getLocationalDirectionYX());
}
const double Analysis::Objects::getMotionalDirectionXZ() const {
  return atan2(getMomentumZ(), getMomentumX());
}
const double Analysis::Objects::getMotionalDirectionYX() const {
  return atan2(getMomentumX(), getMomentumY());
}
const double Analysis::Objects::getMotionalDirectionZY() const {
  return atan2(getMomentumY(), getMomentumZ());
}
const double Analysis::Objects::getLocationXY(Analysis::Unit &unit) const {
  return unit.writeLength(getLocationXY());
}
const double Analysis::Objects::getLocation(Analysis::Unit &unit) const {
  return unit.writeLength(getLocation());
}
const double Analysis::Objects::getMotionalDirectionXZ(Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionXZ());
}
const double Analysis::Objects::getMotionalDirectionYX(Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionYX());
}
const double Analysis::Objects::getMotionalDirectionZY(Analysis::Unit &unit) const {
  return unit.writeDegree(getMotionalDirectionZY());
}
