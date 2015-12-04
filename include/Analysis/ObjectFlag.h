//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_OBJECTFLAG_H
#define ANALYSIS_OBJECTFLAG_H

#include "Flag.h"

namespace Analysis {
class ObjectFlag: public Flag {
 private:
  int _flag;
  const int _initialFlagForSign = 0;
  const int _validOnFlagForSign = 1;
  const int _validOffFlagForSign = -1;
  const int _masterOnFlagFor1stDigit = 2;
  const int _masterOffFlagFor1stDigit = 1;

 public:
  ObjectFlag();
  ~ObjectFlag();
  const int getFlag() const;
  void loadInitialFlager();
  void loadValidOnFlager();
  void loadValidOffFlager();
  void loadMasterOnFlager();
  void loadMasterOffFlager();
  const bool isInitialFlag() const;
  const bool isValidOnFlag() const;
  const bool isMasterOnFlag() const;
};
}

#endif //ANALYSIS_FLAGLIST_H
