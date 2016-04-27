#include "IonParameters.h"

Analysis::IonParameters::IonParameters(const Analysis::Unit &unit,
                                       const Analysis::JSONReader &reader)
    : ObjectParameters(unit, reader, "ion_parameters.") {
  return;
}
Analysis::IonParameters::~IonParameters() { return; }
