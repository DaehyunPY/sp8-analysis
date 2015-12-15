//
// Created by Daehyun You on 12/4/15.
//

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
const double Analysis::Objects::getLocationXOfCOM() const {
  const int &n = this->getNumberOfObjects();
  long double sum1 = 0e0, sum2 = 0e0;
  for (int i = 0; i < n; i++) {
    const double tmp = getObject(i).getMass() / getObject(i).getTOF();
    sum1 += tmp;
    sum2 += tmp * getObject(i).getLocationX();
  }
  return double(sum2 / sum1);
}
const double Analysis::Objects::getLocationYOfCOM() const {
  const int &n = this->getNumberOfObjects();
  long double sum1 = 0e0, sum2 = 0e0;
  for (int i = 0; i < n; i++) {
    const double tmp = getObject(i).getMass() / getObject(i).getTOF();
    sum1 += tmp;
    sum2 += tmp * getObject(i).getLocationY();
  }
  return double(sum2 / sum1);
}
const double Analysis::Objects::getTotalMomentumX() const {
  const int &n = this->getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumX();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getTotalMomentumY() const {
  const int &n = this->getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumY();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getTotalMomentumZ() const {
  const int &n = this->getNumberOfObjects();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumZ();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getTotalEnergy() const {
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

const double Analysis::Objects::getTotalMomentum() const {
  return pow(pow(this->getTotalMomentumX(), 2e0)
                 + pow(this->getTotalMomentumY(), 2e0)
                 + pow(this->getTotalMomentumZ(), 2e0), 0.5e0);
}
const double Analysis::Objects::getDirectionXOfCOM() const {
  return this->getTotalMomentumX() / this->getTotalMomentum();
}
const double Analysis::Objects::getDirectionYOfCOM() const {
  return this->getTotalMomentumY() / this->getTotalMomentum();
}
const double Analysis::Objects::getDirectionZOfCOM() const {
  return this->getTotalMomentumZ() / this->getTotalMomentum();
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
const double Analysis::Objects::getLocationXOfCOM(Analysis::Unit &unit) const {
  return unit.writeLength(this->getLocationXOfCOM());
}
const double Analysis::Objects::getLocationYOfCOM(Analysis::Unit &unit) const {
  return unit.writeLength(this->getLocationYOfCOM());
}
const double Analysis::Objects::getTotalMomentumX(Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getTotalMomentumX());
}
const double Analysis::Objects::getTotalMomentumY(Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getTotalMomentumY());
}
const double Analysis::Objects::getTotalMomentumZ(Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getTotalMomentumZ());
}
const double Analysis::Objects::getTotalMomentum(Analysis::Unit &unit) const {
  return unit.writeMomentum(this->getTotalMomentum());
}
const double Analysis::Objects::getTotalEnergy(Analysis::Unit &unit) const {
  return unit.writeEnergy(this->getTotalEnergy());
}
const int &Analysis::Objects::getNumberOfRealOrDummyObjects() const {
  return this->numberOfHitsUsed;
}
const bool Analysis::Objects::isDummyObject(const int &i) const {
  const int &n = this->getNumberOfObjects();
  const int &m = this->getNumberOfRealOrDummyObjects();
  return (i >= n) && (i < m);
}
const bool Analysis::Objects::isRealObject(const int &i) const {
  const int &n = this->getNumberOfObjects();
  const int &m = this->getNumberOfRealOrDummyObjects();
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
  return 0;
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
const double Analysis::Objects::getTotalMomentumXY() const {
  return pow(pow(getTotalMomentumX(), 2e0)
                 + pow(getTotalMomentumY(), 2e0), 0.5e0);
}
const double Analysis::Objects::getTotalMomentumXY(Analysis::Unit &unit) const {
  return unit.writeMomentum(getTotalMomentumXY());
}
const double Analysis::Objects::getTotalEnergyXY() const {
  long double sum = 0;
  for (int i = 0; i < numberOfHits; i++) {
    sum += getObject(i).getEnergyXY();
  }
  return double(sum);
}
const double Analysis::Objects::getTotalEnergyXY(Analysis::Unit &unit) const {
  return unit.writeEnergy(getTotalEnergyXY());
}
const int Analysis::Objects::getNumberOfDummyObject() const {
  return getNumberOfRealOrDummyObjects()-getNumberOfObjects();
}
