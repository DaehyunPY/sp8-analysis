//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_OBJECTFLAG_H
#define ANALYSIS_OBJECTFLAG_H

#include "Flag.h"

namespace Analysis {
class ObjectFlag: protected Flag {
  const int initialFlag = 0;
  const int withinMasterRegionFlagFor1stDigit = 1;
  const int notWithinMasterRegionFlagFor1stDigit = 2;
  const int deadFlagFor1stDigit = 3;
  const int inFrameOfAllDataFlagFor2ndDigit = 1; 
  const int outOfFrameOfMomentumDataFlagFor2ndDigit = 2; 
  const int outOfFrameOfBasicDataFlagFor2ndDigit = 3; 

 public:
  ObjectFlag();
  ~ObjectFlag();
  void resetFlag();
  void setWithinMasterRegionFlag();
  void setNotWithinMasterRegionFlag();
  void setDeadFlag();
  void setInFrameOfAllDataFlag();
  void setOutOfFrameOfMomentumDataFlag();
  void setOutOfFrameOfBasicDataFlag();
  void forceToSetWithinMasterRegionFlag();
  void forceToSetNotWithinMasterRegionFlag();
  void forceToSetDeadFlag();
  void forceToSetOutOfFrameOfBasicDataFlag();
  void forceToSetOutOfFrameOfMomentumDataFlag();
  void forceToSetInFrameOfAllDataFlag(); 
  const bool isInitial() const;
  const bool isDead() const;
  const bool isNotWithinMasterRegion() const;
  const bool isWithinMasterRegion() const;
  const bool isOutOfFrameOfBasicData() const;
  const bool isOutOfFrameOfMomentumData() const; 
  const bool isInFrameOfAllData() const;
};
}

#endif //ANALYSIS_FLAGLIST_H
