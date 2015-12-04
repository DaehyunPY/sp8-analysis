//
// Created by Daehyun You on 12/4/15.
//

#include "Objects.h"

Analysis::Objects::Objects(const int &n) : numberOfHits(n) {
  assert(n > 0 && n < maximumOfHits+1);
  return;
}
Analysis::Objects::~Objects() {
  return;
}
void Analysis::Objects::setObject(const int &i, Object &object) {
  assert(i < this->getNumberOfHits());
  this->pObject[i] = &object;
  return;
}
const int &Analysis::Objects::getNumberOfHits() const {
  return this->numberOfHits;
}
const double Analysis::Objects::getLocationXOfCOM() const {
  const int &n = this->getNumberOfHits();
  long double sumOfMass = 0e0, sumOfMassLocation = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMass += this->getObject(i).getMass();
    sumOfMassLocation +=
        this->getObject(i).getMass() * this->getObject(i).getLocationX();
  }
  return double(sumOfMassLocation / sumOfMass);
}
const double Analysis::Objects::getLocationYOfCOM() const {
  const int &n = this->getNumberOfHits();
  long double sumOfMass = 0e0, sumOfMassLocation = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMass += this->getObject(i).getMass();
    sumOfMassLocation +=
        this->getObject(i).getMass() * this->getObject(i).getLocationY();
  }
  return double(sumOfMassLocation / sumOfMass);
}
const double Analysis::Objects::getMomentumXOfCOM() const {
  const int &n = this->getNumberOfHits();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumX();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getMomentumYOfCOM() const {
  const int &n = this->getNumberOfHits();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumY();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getMomentumZOfCOM() const {
  const int &n = this->getNumberOfHits();
  long double sumOfMomentum = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfMomentum += this->getObject(i).getMomentumZ();
  }
  return double(sumOfMomentum);
}
const double Analysis::Objects::getEnergyOfCOM() const {
  const int &n = this->getNumberOfHits();
  long double sumOfEnergy = 0e0;
  for (int i = 0; i < n; i++) {
    sumOfEnergy += this->getObject(i).getEnergy();
  }
  return double(sumOfEnergy);
}
const Analysis::Object &Analysis::Objects::getObject(const int &i) const {
  assert(i < this->getNumberOfHits());
  return *(this->pObject[i]);
}
const double Analysis::Objects::getMomentumOfCOM() const {
  return pow(pow(this->getMomentumXOfCOM(), 2e0)
                 + pow(this->getMomentumYOfCOM(), 2e0)
                 + pow(this->getMomentumZOfCOM(), 2e0), 0.5e0);
}
const double Analysis::Objects::getDirectionXOfCOM() const {
  return this->getMomentumXOfCOM() / this->getMomentumOfCOM();
}
const double Analysis::Objects::getDirectionYOfCOM() const {
  return this->getMomentumYOfCOM() / this->getMomentumOfCOM();
}
const double Analysis::Objects::getDirectionZOfCOM() const {
  return this->getMomentumZOfCOM() / this->getMomentumOfCOM();
}
