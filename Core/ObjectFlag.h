//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_OBJECTFLAG_H
#define ANALYSIS_OBJECTFLAG_H

#include <string>
#include "Flag.h"

namespace Analysis {
class ObjectFlag: protected Flag {
 protected:
  ObjectFlag();
  virtual ~ObjectFlag();
  // todo: make only one pulic method using string option
 public:
//  void setFlag(const std::string str);
//  const unsigned int getFlag(const std::string str) const;

 private:
  const unsigned int flagFor1stDigit_withinMasterRegion = 1;
  const unsigned int flagFor1stDigit_outOfMasterRegion = 2;
  const unsigned int flagFor1stDigit_dead = 3;
 public:
  void setWithinMasterRegion();
  void setOutOfMasterRegion();
  void setDead();
  const bool isWithinMasterRegion() const;
  const bool isOutOfMasterRegionOrDead() const;
  const bool isDead() const;

 private:
  const unsigned int flagFor2ndDigit_hasXYTData = 1;
  const unsigned int flagFor2ndDigit_hasMomentumData = 2;
  const unsigned int flagFor2ndDigit_hasProperPzData = 3;
 public:
  void setHavingXYTData();
  void setHavingMomentumData();
  void setHavingProperPzData();
  const bool isHavingXYTData() const;
  const bool isHavingMomentumData() const;
  const bool isHavingProperPzData() const;

 private:
  const unsigned int flagForResort_theRegion1 = 0;
  const unsigned int flagForResort_theRegion2 = 20;
  const unsigned int flagForResort_outOfTheRegion = 21;
  const unsigned int flagForResort_mostReliableRegion1 = 0;
  const unsigned int flagForResort_mostReliableRegion2 = 3;
  const unsigned int flagForResort_secondMostReliableRegion1 = 4;
  const unsigned int flagForResort_secondMostReliableRegion2 = 14;
  const unsigned int flagForResort_riskyRegion1 = 15;
  const unsigned int flagForResort_riskyRegion2 = 20;
  // most reliable region: 0 to 3
  // second most reliable region: 4 to 14
  // risky region: 15 to 20
  const unsigned int flagFor3rd2Digit_init = 0;
  const unsigned int flagFor3rd2Digit_inTheRegion1 = 1;
  const unsigned int flagFor3rd2Digit_inTheRegion2 = flagForResort_theRegion2 - flagForResort_theRegion1
      + 1; // 21
  const unsigned int flagFor3rd2Digit_lowerThanTheRegion = flagFor3rd2Digit_inTheRegion2 + 1;
  const unsigned int flagFor3rd2Digit_upperThanTheRegion = flagFor3rd2Digit_inTheRegion2 + 2;
  unsigned int convertCoboldFlag(const int coboldFlag) const;
  const unsigned int convertToCoboldFlag(const unsigned int storedFlag) const;
 public:
  // flag for 3rd digit
  void setResortFlag(const int coboldFlag);
  const bool isResortFlag(const int coboldFlag) const;
  const bool isMostReliable() const;
  const bool isMostOrSecondMostReliable() const;
  const bool isRisky() const;
  const unsigned int getResortFlag() const;

 private:
  const unsigned int flagFor5thDigit_inFrameOfAllData = 1;
  const unsigned int flagFor5thDigit_outOfFrameOfMomentumData = 2;
  const unsigned int flagFor5thDigit_outOfFrameOfBasicData = 3;
 public:
  void setInFrameOfAllDataFlag();
  void setOutOfFrameOfMomentumDataFlag();
  void setOutOfFrameOfBasicDataFlag();
  const bool isInFrameOfAllData() const;
  const bool isOutOfFrameOfMomentumData() const;
  const bool isOutOfFrameOfBasicData() const;
};
}

#endif //ANALYSIS_FLAGLIST_H
