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
  Ion *pIon[MAXIMUM_OF_HITS]; // todo: think a better way
  Ions(const Unit &unit, const JSONReader &reader, const int &n, const int &m);
  const std::string getIonName(int i) const;

 public:
  Ions(const Unit &, const JSONReader &);
  Ions(const Unit &, const JSONReader &, const int &);
  ~Ions();
  Ion &setIonMembers(const int &i);
  Ion &setDummyIonMembers(const int &i);
  Ion &setRealOrDummyIonMembers(const int &i);
  const Ion &getIon(const int &i) const;
  const Ion &getDummyIon(const int &i) const;
  const Ion &getRealOrDummyIon(const int &i) const;
};
}


#endif //ANALYSIS_OBJECTS_H
