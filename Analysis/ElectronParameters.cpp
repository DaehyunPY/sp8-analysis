#include "ElectronParameters.h"

Analysis::ElectronParameters::ElectronParameters(const Analysis::Unit &_Unit,
                                                 const Analysis::JSONReader
                                                 &_Reader)
    : ObjectParameters(_Unit, _Reader, "electron_parameters.") {
  return;
}
Analysis::ElectronParameters::~ElectronParameters() { return; }
