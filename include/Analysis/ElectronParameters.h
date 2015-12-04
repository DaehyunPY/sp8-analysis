#ifndef ANALYSIS_ELECTRONPARAMETERS_H
#define ANALYSIS_ELECTRONPARAMETERS_H

#include "ObjectParameters.h"

namespace Analysis {
class ElectronParameters: public ObjectParameters {
 private:
 public:
  ElectronParameters(const Unit &unit, const JSONReader &reader);
  ~ElectronParameters();
};
}

#endif
