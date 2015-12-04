#ifndef ANALYSIS_IONPARAMETERS_H
#define ANALYSIS_IONPARAMETERS_H

#include "ObjectParameters.h"

namespace Analysis {
class IonParameters: public ObjectParameters {
 private:
 public:
  IonParameters(const Unit &unit, const JSONReader &reader);
  ~IonParameters();
};
}

#endif
