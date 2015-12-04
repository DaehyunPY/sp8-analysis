//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_IONS_H
#define ANALYSIS_IONS_H


#include "Objects.h"
#include "Ion.h"

namespace Analysis {
class Ions : public Objects {
 private:
  Ion *pIon[MAXIMUM_OF_HITS];
  Ions(const Unit &unit, const JSONReader &reader, const int &n);

 public:
  Ions(const Unit &unit, const JSONReader &reader);
  ~Ions();
  Ion &setIonMembers(const int &i);
  const Ion &getIon(const int &i) const;
};
}


#endif //ANALYSIS_OBJECTS_H
