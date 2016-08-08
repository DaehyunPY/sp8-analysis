//
// Created by Daehyun You on 11/27/15.
//

#include "ObjectFlag.h"

Analysis::ObjectFlag::ObjectFlag() : Flag() {
  return;
}
void Analysis::ObjectFlag::setWithinMasterRegion() {
  if (get1stDigit() == initFlag) {
    set1stDigit(flagFor1stDigit_withinMasterRegion);
  }
}
void Analysis::ObjectFlag::setOutOfMasterRegion() {
  if (get1stDigit() == initFlag) {
    set1stDigit(flagFor1stDigit_outOfMasterRegion);
  }
}
void Analysis::ObjectFlag::setDead() {
  const int n = get1stDigit();
  if (n == initFlag
      || n == flagFor1stDigit_withinMasterRegion
      || n == flagFor1stDigit_outOfMasterRegion) {
    set1stDigit(flagFor1stDigit_dead);
  }
}
const bool Analysis::ObjectFlag::isDead() const {
	const auto f = get1stDigit();
  return (f == initFlag) || (f == flagFor1stDigit_dead);
}
const bool Analysis::ObjectFlag::isOutOfMasterRegion() const {
  const int n = get1stDigit();
  return n == flagFor1stDigit_outOfMasterRegion
      || n == flagFor1stDigit_dead;
}
const bool Analysis::ObjectFlag::isWithinMasterRegion() const {
  return get1stDigit() == flagFor1stDigit_withinMasterRegion;
}
void Analysis::ObjectFlag::setResortFlag(const int coboldFlag) {
  setNthNumDigit(3, 2, convertCoboldFlag(coboldFlag));
}
const bool Analysis::ObjectFlag::isResortFlag(const int coboldFlag) const {
  return getNthNumDigit(3, 2) == convertCoboldFlag(coboldFlag);
}
const unsigned int Analysis::ObjectFlag::getResortFlag() const {
  return convertToCoboldFlag(getNthNumDigit(3, 2));
}
const unsigned int Analysis::ObjectFlag::convertCoboldFlag(const int coboldFlag) const {
  if(coboldFlag < (int) flagForResort_theRegion1) {
    return flagFor3rd2Digit_lowerThanTheRegion;
  } else if(coboldFlag > (int) flagForResort_theRegion2) {
    return flagFor3rd2Digit_upperThanTheRegion;
  } else {
    return (coboldFlag - flagForResort_theRegion1 + 1);
  }
}
const unsigned int Analysis::ObjectFlag::convertToCoboldFlag(const unsigned int storedFlag) const {
  if(!(storedFlag >= flagFor3rd2Digit_inTheRegion1 && storedFlag <= flagFor3rd2Digit_inTheRegion2)) {
    return flagForResort_outOfTheRegion;
  }
  return storedFlag + flagForResort_theRegion1 - 1;
}
const bool Analysis::ObjectFlag::isMostReliable() const {
  return flagForResort_mostReliableRegion1 <= getResortFlag() &&  getResortFlag() <= flagForResort_mostReliableRegion2;
}
const bool Analysis::ObjectFlag::isMostOrSecondMostReliable() const {
  return flagForResort_mostReliableRegion1 <= getResortFlag() &&  getResortFlag() <= flagForResort_secondMostReliableRegion2;
}
const bool Analysis::ObjectFlag::isRisky() const {
  return flagForResort_riskyRegion1 <= getResortFlag() && getResortFlag() <= flagForResort_outOfTheRegion;
}
Analysis::ObjectFlag::~ObjectFlag() {

}
void Analysis::ObjectFlag::setHavingNotProperData() {
	const auto f0 = get2ndDigit();
  if ((f0 == initFlag) || (f0 == flagFor2ndDigit_havingXYTData) || (f0 == flagFor2ndDigit_havingMomentumData)) {
	  set2ndDigit(flagFor2ndDigit_havingNotProperData);
  }
}
void Analysis::ObjectFlag::setHavingXYTData() {
  if(get2ndDigit() == initFlag) {
    set2ndDigit(flagFor2ndDigit_havingXYTData);
  }
}
void Analysis::ObjectFlag::setHavingMomentumData() {
  if(get2ndDigit() == flagFor2ndDigit_havingXYTData) {
    set2ndDigit(flagFor2ndDigit_havingMomentumData);
  }
}
const bool Analysis::ObjectFlag::isHavingNotProperData() const {
	const auto f0 = get2ndDigit();
	return (f0 == initFlag) || (f0 == flagFor2ndDigit_havingNotProperData);
}
const bool Analysis::ObjectFlag::isHavingXYTData() const {
  const unsigned int f0 = get2ndDigit();
  return (f0 == flagFor2ndDigit_havingXYTData) || (f0 == flagFor2ndDigit_havingMomentumData);
}
const bool Analysis::ObjectFlag::isHavingMomentumData() const {
  const unsigned int f0 = get2ndDigit();
  return (f0 == flagFor2ndDigit_havingMomentumData);
}
#define ANALYSIS_OBJECTFLAG_CASESET(X) case X: set ## X(); break;
void Analysis::ObjectFlag::setFlag(const FlagName flagName) {
  switch(flagName) {
    ANALYSIS_OBJECTFLAG_CASESET(WithinMasterRegion)
    ANALYSIS_OBJECTFLAG_CASESET(OutOfMasterRegion)
    ANALYSIS_OBJECTFLAG_CASESET(Dead)
    ANALYSIS_OBJECTFLAG_CASESET(HavingNotProperData)
    ANALYSIS_OBJECTFLAG_CASESET(HavingXYTData)
    ANALYSIS_OBJECTFLAG_CASESET(HavingMomentumData)
    ANALYSIS_OBJECTFLAG_CASESET(RealObject)
    ANALYSIS_OBJECTFLAG_CASESET(IonObject)
    ANALYSIS_OBJECTFLAG_CASESET(ElecObject)
    ANALYSIS_OBJECTFLAG_CASESET(DummyObject)
    default:
      assert(false);
      break;
  }
}
void Analysis::ObjectFlag::setFlag(const FlagName flagName,
                                   const int arg) {
  switch(flagName) {
    case ResortFlag:
      setResortFlag(arg);
      break;
    default:
      assert(false);
      break;
  }
}
#define ANALYSIS_OBJECTFLAG_CASEIS(X) case X: output = is ## X(); break;
const bool Analysis::ObjectFlag::isFlag(const FlagName flagName) const {
  bool output = false;
  switch(flagName) {
    ANALYSIS_OBJECTFLAG_CASEIS(WithinMasterRegion)
    ANALYSIS_OBJECTFLAG_CASEIS(OutOfMasterRegion)
    ANALYSIS_OBJECTFLAG_CASEIS(Dead)
    ANALYSIS_OBJECTFLAG_CASEIS(HavingNotProperData)
    ANALYSIS_OBJECTFLAG_CASEIS(HavingXYTData)
    ANALYSIS_OBJECTFLAG_CASEIS(HavingMomentumData)
    ANALYSIS_OBJECTFLAG_CASEIS(MostReliable)
    ANALYSIS_OBJECTFLAG_CASEIS(MostOrSecondMostReliable)
    ANALYSIS_OBJECTFLAG_CASEIS(Risky)
    ANALYSIS_OBJECTFLAG_CASEIS(RealObject)
    ANALYSIS_OBJECTFLAG_CASEIS(IonObject)
    ANALYSIS_OBJECTFLAG_CASEIS(ElecObject)
    ANALYSIS_OBJECTFLAG_CASEIS(DummyObject)
    default:
      assert(false);
  }
  return output;
}
const bool Analysis::ObjectFlag::isFlag(const FlagName flagName,
                                        const int arg) {
  bool output = false;
  switch(flagName) {
    case ResortFlag:
      output = isResortFlag(arg);
      break;
    default:
      assert(false);
  }
  return output;
}
void Analysis::ObjectFlag::setRealObject() {
  if(getNthDigit(5) == initFlag) {
    setNthDigit(5, flagFor5thDigit_realObject);
  }
}
void Analysis::ObjectFlag::setDummyObject() {
  if(getNthDigit(5) == initFlag) {
    setNthDigit(5, flagFor5thDigit_dummyObject);
  }
}
const bool Analysis::ObjectFlag::isRealObject() const {
  const int n = getNthDigit(5);
  return n == flagFor5thDigit_realObject
      || n == flagFor5thDigit_ionObject
      || n == flagFor5thDigit_elecObject;
}
const bool Analysis::ObjectFlag::isDummyObject() const {
  return getNthDigit(5) == flagFor5thDigit_dummyObject;
}
void Analysis::ObjectFlag::setIonObject() {
  const int n = getNthDigit(5);
  if(n == initFlag || n == flagFor5thDigit_realObject) {
    setNthDigit(5, flagFor5thDigit_ionObject);
  }
}
void Analysis::ObjectFlag::setElecObject() {
  const int n = getNthDigit(5);
  if (n == initFlag
      || n == flagFor5thDigit_realObject) {
    setNthDigit(5, flagFor5thDigit_elecObject);
  }
}
const bool Analysis::ObjectFlag::isIonObject() const {
  const int n = getNthDigit(5);
  return n == flagFor5thDigit_ionObject;
}
const bool Analysis::ObjectFlag::isElecObject() const {
  return getNthDigit(5) == flagFor5thDigit_elecObject;
}
void Analysis::ObjectFlag::resetFlag() {
  setNthDigit(1, initFlag);
  setNthDigit(2, initFlag);
  setNthNumDigit(3, 2, initFlag);
}
