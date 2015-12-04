#ifndef ANALYSIS_ION_H
#define ANALYSIS_ION_H

#include "Object.h"

namespace Analysis {
class Ion: public Object {
 private:
  Ion(const Unit &unit, const double &m, const double &q, const double &t0,
      const double &t1);
 public:
  Ion(const Unit &unit, const JSONReader &reader, const std::string &prefix);
  ~Ion();
};
}

#endif
