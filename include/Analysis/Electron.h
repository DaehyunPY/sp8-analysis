#ifndef ANALYSIS_ELECTRON_H
#define ANALYSIS_ELECTRON_H

#include "Object.h"

namespace Analysis {

class Electron: public Object {
 private:
  Electron(const Unit &unit, const double &t0, const double &t1);
 public:
  Electron(); // for dummy electron
  Electron(const Analysis::Unit &unit, const Analysis::JSONReader &reader);
  ~Electron();
};
}
#endif
