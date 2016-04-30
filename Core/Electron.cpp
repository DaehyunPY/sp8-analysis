#include "Electron.h"

Analysis::Electron::Electron(const Analysis::Unit &unit,
                             const double &t0,
                             const double &t1)
    : Object(
    unit.readElectronRestMass(1e0),
    unit.readElementaryCharge(1e0),
    unit.readNanoSec(t0),
    unit.readNanoSec(t1)) {
  return;
}
Analysis::Electron::Electron(const Analysis::Unit &unit,
                             const Analysis::JSONReader &reader)
    : Electron(unit,
               reader.getDoubleAt("electrons.minimum_of_TOF"),
               reader.getDoubleAt("electrons.maximum_of_TOF")) {
  return;
}
Analysis::Electron::~Electron() { return; }
Analysis::Electron::Electron() : Object() { return; }
