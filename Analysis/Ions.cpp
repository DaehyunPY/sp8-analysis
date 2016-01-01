//
// Created by Daehyun You on 12/4/15.
//

#include "Ions.h"
Analysis::Ions::Ions(const Analysis::Unit &unit,
                     const Analysis::JSONReader &reader,
                     const int &n,
                     const int &m) : Objects(n, m) {
  ppIon = new Ion *[getNumberOfRealOrDummyObjects()]{nullptr};
  for (int i = 0; i < n; i++) { // for real ions
    ppIon[i] = new Ion(unit, reader, getIonName(i));
    setObject(i, *ppIon[i]);
  }
  for (int i = n; i < m; i++) { // for dummy ions
    ppIon[i] = new Ion();
    setDummyObject(i, *ppIon[i]);
  }
  return;
}
Analysis::Ions::Ions(const Analysis::Unit &unit,
                     const Analysis::JSONReader &reader)
    : Ions(unit,
           reader,
           reader.getIntAt("ions.number_of_hits"),
           reader.getIntAt("ions.number_of_hits")) {
}
Analysis::Ions::Ions(const Analysis::Unit &unit,
                     const Analysis::JSONReader &reader,
                     const int &m)
    : Ions(unit,
           reader,
           reader.getIntAt("ions.number_of_hits"),
           m) {
}
Analysis::Ions::~Ions() {
  for (int i = 0; i < getNumberOfRealOrDummyObjects(); i++) {
    delete ppIon[i];
  }
  delete[] ppIon;
}
Analysis::Ion &Analysis::Ions::setIonMembers(const int &i) {
  assert(i < this->getNumberOfObjects());
  return *(this->ppIon[i]);
}
Analysis::Ion &Analysis::Ions::setDummyIonMembers(const int &i) {
  assert(i >= this->getNumberOfObjects());
  return *(this->ppIon[i]);
}
const Analysis::Ion &Analysis::Ions::getIon(const int &i) const {
  assert(i < this->getNumberOfObjects());
  return *(this->ppIon[i]);
}
const Analysis::Ion &Analysis::Ions::getDummyIon(const int &i) const {
  assert(i >= this->getNumberOfObjects());
  return *(this->ppIon[i]);
}
const std::string Analysis::Ions::getIonName(int i) const {
  i += 1;
  assert(i > 0);
  const int firstDigit = i % 10;
  const int secondDigit = (i / 10) % 10;
  const std::string str = std::to_string(i);
  if (secondDigit == 1) { return str + "th_hit"; }
  else {
    if (firstDigit == 1) { return str + "st_hit"; }
    else if (firstDigit == 2) { return str + "nd_hit"; }
    else if (firstDigit == 3) { return str + "rd_hit"; }
    else { return str + "th_hit"; }
  }
}
Analysis::Ion &Analysis::Ions::setRealOrDummyIonMembers(const int &i) {
  Ion *pIon;
  if (isRealObject(i)) {
    pIon = &setIonMembers(i);
  } else if (isDummyObject(i)) {
    pIon = &setDummyIonMembers(i);
  } else {
    assert(false);
    pIon = new Ion();
  }
  return *pIon;
}
const Analysis::Ion &Analysis::Ions::getRealOrDummyIon(const int &i) const {
  const Ion *pIon;
  if (isRealObject(i)) {
    pIon = &getIon(i);
  } else if (isDummyObject(i)) {
    pIon = &getDummyIon(i);
  } else {
    assert(false);
    pIon = new Ion();
  }
  return *pIon;
}
