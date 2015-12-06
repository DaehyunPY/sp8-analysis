#include "Ion.h"

Analysis::Ion::Ion(const Analysis::Unit &unit,
                   const double &m,
                   const double &q,
                   const double &t0,
                   const double &t1)
    : Object(
    unit.readAtomicMass(m),
    unit.readCharge(q),
    unit.readTime(t0),
    unit.readTime(t1)) {
  return;
}
Analysis::Ion::Ion(const Analysis::Unit &unit,
                   const Analysis::JSONReader &reader,
                   const std::string &prefix)

    : Ion(unit,
          reader.getDouble("ions." + prefix + ".mass"),
          reader.getDouble("ions." + prefix + ".charge"),
          reader.getDouble("ions." + prefix + ".minimum_of_TOF"),
          reader.getDouble("ions." + prefix + ".maximum_of_TOF")) {
  return;
}
Analysis::Ion::~Ion() { return; }
Analysis::Ion::Ion() : Object() { return; }
