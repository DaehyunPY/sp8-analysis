//
// Created by Daehyun You on 12/4/15.
//

#include "Ions.h"
Analysis::Ions::Ions(const Analysis::Unit &unit,
                     const Analysis::JSONReader &reader,
                     const int &n,
                     const int &m) : Objects(n, m) {
  for (int i = 0; i < n; i++) { // for real ions
    this->pIon[i] = new Ion(unit, reader, this->getIonName(i));
    this->setObject(i, *(this->pIon[i]));
  }
  for (int i = n; i < m; i++) { // for dummy ions
    this->pIon[i] = new Ion();
    this->setObject(i, *(this->pIon[i]));
  }
  return;
}
Analysis::Ions::Ions(const Analysis::Unit &unit,
                     const Analysis::JSONReader &reader)
    : Ions(unit,
           reader,
           reader.getInt("ions.number_of_hits"),
           reader.getInt("ions.number_of_hits")) {
  return;
}
Analysis::Ions::Ions(const Analysis::Unit &unit,
                     const Analysis::JSONReader &reader,
                     const int &m)
    : Ions(unit,
           reader,
           reader.getInt("ions.number_of_hits"),
           m) {
  return;
}
Analysis::Ions::~Ions() { return; }
Analysis::Ion &Analysis::Ions::setIonMembers(const int &i) {
  assert(i < this->getNumberOfHits());
  return *(this->pIon[i]);
}
Analysis::Ion &Analysis::Ions::setDummyIonMembers(const int &i) {
  assert(i >= this->getNumberOfHits());
  return *(this->pIon[i]);
}
const Analysis::Ion &Analysis::Ions::getIon(const int &i) const {
  assert(i < this->getNumberOfHits());
  return *(this->pIon[i]);
}
const Analysis::Ion &Analysis::Ions::getDummyIon(const int &i) const {
  assert(i >= this->getNumberOfHits());
  return *(this->pIon[i]);
}
const std::string Analysis::Ions::getIonName(const int &i) const {
  assert(i > 0);
  const int firstDigit = i % 10;
  const int secondDigit = (i / 10) % 10;
  if (secondDigit == 1) { return i + "th_hit"; }
  else {
    if (firstDigit == 1) { return i + "st_hit"; }
    else if (firstDigit == 2) { return i + "nd_hit"; }
    else if (firstDigit == 3) { return i + "rd_hit"; }
    else { return i + "th_hit"; }
  }
}
