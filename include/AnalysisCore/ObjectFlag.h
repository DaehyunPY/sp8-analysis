//
// Created by Daehyun You on 11/27/15.
//

#ifndef ANALYSIS_OBJECTFLAG_H
#define ANALYSIS_OBJECTFLAG_H

#include <string>
#include <map>
#include <cassert>
#include "Core/Flag.h"

namespace Analysis {
class ObjectFlag: protected Flag {
 protected:
  ObjectFlag();
  virtual ~ObjectFlag();
  void resetFlag();
 public:
  enum FlagName {
    WithinMasterRegion,
    OutOfMasterRegion,
    Dead,
    HavingNotProperData,
    HavingXYTData,
    HavingMomentumData,
    ResortFlag,
    MostReliable,
    MostOrSecondMostReliable,
    Risky,
    RealObject,
    DummyObject,
    IonObject,
    ElecObject
  };
  void setFlag(const FlagName flagName);
  void setFlag(const FlagName flagName, const int arg);
  const bool isFlag(const FlagName flagName) const;
  const bool isFlag(const FlagName flagName, const int arg);

 private:
  const unsigned int flagFor1stDigit_withinMasterRegion = 1;
  const unsigned int flagFor1stDigit_outOfMasterRegion = 2;
  const unsigned int flagFor1stDigit_dead = 3;
  void setWithinMasterRegion();
  void setOutOfMasterRegion();
  void setDead();
  const bool isWithinMasterRegion() const;
  const bool isOutOfMasterRegion() const;
  const bool isDead() const;

 private:
  const unsigned int flagFor2ndDigit_havingNotProperData = 1;
  const unsigned int flagFor2ndDigit_havingXYTData = 2;
  const unsigned int flagFor2ndDigit_havingMomentumData = 3;
  void setHavingNotProperData();
  void setHavingXYTData();
  void setHavingMomentumData();
  const bool isHavingNotProperData() const;
  const bool isHavingXYTData() const;
  const bool isHavingMomentumData() const;

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
  const unsigned int flagFor3rd2Digit_init = 0;
  const unsigned int flagFor3rd2Digit_inTheRegion1 = 1;
  const unsigned int flagFor3rd2Digit_inTheRegion2 =
      flagForResort_theRegion2 - flagForResort_theRegion1 + 1; // 21
  const unsigned int
      flagFor3rd2Digit_lowerThanTheRegion = flagFor3rd2Digit_inTheRegion2 + 1;
  const unsigned int
      flagFor3rd2Digit_upperThanTheRegion = flagFor3rd2Digit_inTheRegion2 + 2;
  const unsigned int convertCoboldFlag(const int coboldFlag) const;
  const unsigned int convertToCoboldFlag(const unsigned int storedFlag) const;
  const unsigned int getResortFlag() const;
  void setResortFlag(const int coboldFlag);
  const bool isResortFlag(const int coboldFlag) const;
  const bool isMostReliable() const;
  const bool isMostOrSecondMostReliable() const;
  const bool isRisky() const;

 private:
  const unsigned int flagFor5thDigit_realObject = 1;
  const unsigned int flagFor5thDigit_dummyObject = 2;
  void setRealObject();
  void setDummyObject();
  const bool isRealObject() const;
  const bool isDummyObject() const;

 private:
  const unsigned int flagFor6thDigit_ionObject = 3;
  const unsigned int flagFor6thDigit_elecObject = 4;
  void setIonObject();
  void setElecObject();
  const bool isIonObject() const;
  const bool isElecObject() const;
};
}

#endif //ANALYSIS_FLAGLIST_H
