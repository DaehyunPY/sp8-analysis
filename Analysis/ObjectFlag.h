//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_OBJECTFLAG_H
#define ANALYSIS_OBJECTFLAG_H

#include "Flag.h"

namespace Analysis {
class ObjectFlag: protected Flag {
 private:
  const int initialFlagForSign = 0;
  const int validFlagForSign = 1;
  const int invalidFlagForSign = -1;
  const int withinMasterRegionFlagFor1stDigit = 1;
  const int notWithinMasterRegionFlagFor1stDigit = 2;
  const int deadFlagFor1stDigit = 3;

 public:
  ObjectFlag();
  ~ObjectFlag();
  void loadInitialFlager();
  void loadValidFlager();
  void loadInvalidFlager();
  void loadWithinMasterRegionFlager();
  void loadNotWithinMasterRegionFlager();
  void loadDeadFlager();
  const bool isInitial() const;
  const bool isValid() const;
  const bool isInvalid() const;
  const bool isWithinMasterRegion() const;
  const bool isNotWithinMasterRegion() const;
  const bool isDead() const;
};
}

#endif //ANALYSIS_FLAGLIST_H
