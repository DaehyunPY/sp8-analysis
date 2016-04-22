#include "Electrons.h"
Analysis::Electrons::Electrons(const Analysis::Unit &unit,
                               const Analysis::JSONReader &reader,
                               const int &n,
                               const int &m) : Objects(n, m) {
  ppElectron = new Electron *[getNumberOfRealOrDummyObjects()]{nullptr};
  for (int i = 0; i < n; i++) { // for real electrons
    ppElectron[i] = new Electron(unit, reader);
    setObject(i, *ppElectron[i]);
  }
  for (int i = n; i < m; i++) { // for dummy electrons
    ppElectron[i] = new Electron();
    setDummyObject(i, *ppElectron[i]);
  }
}
Analysis::Electrons::Electrons(const Analysis::Unit &unit,
                               const Analysis::JSONReader &reader)
    : Electrons(unit,
                reader,
                reader.getIntAt("electrons.number_of_hits"),
                reader.getIntAt("electrons.number_of_hits")) {
}
Analysis::Electrons::Electrons(const Analysis::Unit &unit,
                               const Analysis::JSONReader &reader,
                               const int &m)
    : Electrons(unit,
                reader,
                reader.getIntAt("electrons.number_of_hits"),
                m) {
}
Analysis::Electrons::~Electrons() {
  for (int i = 0; i < getNumberOfRealOrDummyObjects(); i++) {
    delete ppElectron[i];
  }
  delete[] ppElectron;
}
Analysis::Electron &Analysis::Electrons::setElectronMembers(const int &i) {
  assert(isRealObject(i));
  return *ppElectron[i];
}
Analysis::Electron &Analysis::Electrons::setDummyElectronMembers(const int &i) {
  assert(isDummyObject(i));
  return *ppElectron[i];
}
const Analysis::Electron &Analysis::Electrons::getElectron(const int &i) const {
  assert(isRealObject(i));
  return *ppElectron[i];
}
const Analysis::Electron &Analysis::Electrons::getDummyElectron(const int &i) const {
  assert(isDummyObject(i));
  return *ppElectron[i];
}
Analysis::Electron &Analysis::Electrons::setRealOrDummyElectronMembers(const int &i) {
  Electron *pElectron;
  if(isRealObject(i)) {
    pElectron = &setElectronMembers(i);
  } else if(isDummyObject(i)) {
    pElectron = &setDummyElectronMembers(i);
  } else {
    assert(false);
    pElectron = new Electron();
  }
  return *pElectron;
}
const Analysis::Electron &Analysis::Electrons::getRealOrDummyElectron(const int &i) const {
  const Electron *pElectron;
  if(isRealObject(i)) {
    pElectron = &getElectron(i);
  } else if(isDummyObject(i)) {
    pElectron = &getDummyElectron(i);
  } else {
    assert(false);
    pElectron = new Electron();
  }
  return *pElectron;
}
