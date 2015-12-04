//
// Created by Daehyun You on 12/4/15.
//

#ifndef ANALYSIS_ELECTRONS_H
#define ANALYSIS_ELECTRONS_H


#include "Objects.h"
#include "Electron.h"

namespace Analysis {
class Electrons : public Objects {
 private:
  Electron *pElectron[MAXIMUM_OF_HITS];
  Electrons(const Unit &unit, const JSONReader &reader, const int &n);

 public:
  Electrons(const Unit &unit, const JSONReader &reader);
  ~Electrons();
  Electron &setElectronMembers(const int &i);
  const Electron &getElectron(const int &i) const;
};
}


#endif //ANALYSIS_ELECTRONS_H
