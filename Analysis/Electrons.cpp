#include <sys/cdefs.h>
#include "Electrons.h"
Analysis::Electrons::Electrons(const Analysis::Unit &unit,
                               const Analysis::JSONReader &reader,
                               const int &n,
                               const int &m) : Objects(n, m) {
  for (int i = 0; i < n; i++) { // for real electrons
    pElectron[i] = new Electron(unit, reader);
    setObject(i, *pElectron[i]);
  }
  for (int i = n; i < m; i++) { // for dummy electrons
    pElectron[i] = new Electron();
    setDummyObject(i, *pElectron[i]);
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
  assert(isRealObject(i));
  return *pElectron[i];
}
Analysis::Electron &Analysis::Electrons::setDummyElectronMembers(const int &i) {
  assert(isDummyObject(i));
  return *pElectron[i];
}
const Analysis::Electron &Analysis::Electrons::getElectron(const int &i) const {
  assert(isRealObject(i));
  return *pElectron[i];
}
const Analysis::Electron  __unused &Analysis::Electrons::getDummyElectron(const int &i) const {
  assert(isDummyObject(i));
  return *pElectron[i];
}
