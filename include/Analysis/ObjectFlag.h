//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_OBJECTFLAG_H
#define ANALYSIS_OBJECTFLAG_H

#include "Flag.h"

namespace Analysis {
class ObjectFlag: public Flag {
 private:
  int flag;
  const int initialFlagForSign = 0;
  const int validOnFlagForSign = 1;
  const int validOffFlagForSign = -1;
  const int masterOnFlagFor1stDigit = 2;
  const int masterOffFlagFor1stDigit = 1;

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
