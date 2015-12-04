#include "Electron.h"

Analysis::Electron::Electron(const Analysis::Unit &unit,
                             const double &t0,
                             const double &t1)
    : Object(
    unit.readElectronRestMass(1e0),
    unit.readCharge(1e0),
    unit.readTime(t0),
    unit.readTime(t1)) {
  return;
}
Analysis::Electron::Electron(const Analysis::Unit &unit,
                             const Analysis::JSONReader &reader)
    : Electron(unit,
               reader.getDouble("electrons.minimum_of_TOF"),
               reader.getDouble("electrons.maximum_of_TOF")) {
  return;
}
Analysis::Electron::~Electron() { return; }
