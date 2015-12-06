//
// Created by Daehyun You on 12/4/15.
//

#include "Electrons.h"
Analysis::Electrons::Electrons(const Analysis::Unit &unit,
                               const Analysis::JSONReader &reader,
                               const int &n,
                               const int &m) : Objects(n, m) {
  for (int i = 0; i < n; i++) { // for real electrons
    this->pElectron[i] = new Electron(unit, reader);
    this->setObject(i, *(this->pElectron[i]));
  }
  for (int i = n; i < m; i++) { // for dummy electrons
    this->pElectron[i] = new Electron();
    this->setDummyObject(i, *(this->pElectron[i]));
  }
  return;
}
Analysis::Electrons::Electrons(const Analysis::Unit &unit,
                               const Analysis::JSONReader &reader)
    : Electrons(unit,
                reader,
                reader.getInt("electrons.number_of_hits"),
                reader.getInt("electrons.number_of_hits")) {
  return;
}
Analysis::Electrons::Electrons(const Analysis::Unit &unit,
                               const Analysis::JSONReader &reader,
                               const int &m)
    : Electrons(unit,
                reader,
                reader.getInt("electrons.number_of_hits"),
                m) {
  return;
}
Analysis::Electrons::~Electrons() { return; }
Analysis::Electron &Analysis::Electrons::setElectronMembers(const int &i) {
  assert(i < this->getNumberOfHits());
  return *(this->pElectron[i]);
}
Analysis::Electron &Analysis::Electrons::setDummyElectronMembers(const int &i) {
  assert(i >= this->getNumberOfHits());
  return *(this->pElectron[i]);
}
const Analysis::Electron &Analysis::Electrons::getElectron(const int &i) const {
  assert(i < this->getNumberOfHits());
  return *(this->pElectron[i]);
}
const Analysis::Electron &Analysis::Electrons::getDummyElectron(const int &i) const {
  assert(i >= this->getNumberOfHits());
  return *(this->pElectron[i]);
}
