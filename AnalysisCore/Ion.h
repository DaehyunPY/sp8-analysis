#ifndef ANALYSIS_ION_H
#define ANALYSIS_ION_H

#include "Object.h"
#include "../Protocols/JSONReader.h"

namespace Analysis {
class Ion: public Object {
 private:
  Ion(const Unit &, const double &, const double &, const double &, const double &);

 public:
  Ion(); // for dummy ion
  Ion(const Unit &, const JSONReader &, const std::string &);
  ~Ion();
};
}

#endif
