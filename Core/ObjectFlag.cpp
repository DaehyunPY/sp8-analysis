//
// Created by Daehyun You on 11/27/15.
//

#include <cassert>
#include "ObjectFlag.h"

Analysis::ObjectFlag::ObjectFlag() : Flag() {
  resetFlag();
  return;
}
Analysis::ObjectFlag::~ObjectFlag() { }
void Analysis::ObjectFlag::resetFlag() {
  setFlag(initialFlag);
}

void Analysis::ObjectFlag::setWithinMasterRegionFlag() {
  if (get1stDigit() == get1stDigit(initialFlag)) {
    set1stDigit(flagFor1stDigit_withinMasterRegion);
  }
}

void Analysis::ObjectFlag::setOutOfMasterRegionFlag() {
  if (get1stDigit() == get1stDigit(initialFlag)) {
    set1stDigit(flagFor1stDigit_outOfMasterRegion);
  }
}

void Analysis::ObjectFlag::setDeadFlag() {
  if (get1stDigit() < flagFor1stDigit_dead) {
    set1stDigit(flagFor1stDigit_dead);
  }
}

void Analysis::ObjectFlag::setInFrameOfAllDataFlag() {
  if (get2ndDigit() == get2ndDigit(initialFlag)) {
    set2ndDigit(flagFor2ndDigit_inFrameOfAllData);
  }
}

void Analysis::ObjectFlag::setOutOfFrameOfMomentumDataFlag() {
  if (flagFor2ndDigit_inFrameOfAllData <= get2ndDigit()
      && get2ndDigit() < flagFor2ndDigit_outOfFrameOfMomentumData) {
    set2ndDigit(flagFor2ndDigit_outOfFrameOfMomentumData);
  }
}

void Analysis::ObjectFlag::setOutOfFrameOfBasicDataFlag() {
  if (flagFor2ndDigit_inFrameOfAllData <= get2ndDigit()
      && get2ndDigit() < flagFor2ndDigit_outOfFrameOfBasicData) {
    set2ndDigit(
        flagFor2ndDigit_outOfFrameOfBasicData);
  }
}

const bool Analysis::ObjectFlag::isInitial() const {
  return getFlag() == initialFlag;
}
const bool Analysis::ObjectFlag::isInFrameOfAllData() const {
  return get2ndDigit() == flagFor2ndDigit_inFrameOfAllData;
}
const bool Analysis::ObjectFlag::isDead() const {
  return get1stDigit() >= flagFor1stDigit_dead;
}
const bool Analysis::ObjectFlag::isOutOfFrameOfBasicData() const {
  return get2ndDigit()
      >= flagFor2ndDigit_outOfFrameOfBasicData;
}

const bool Analysis::ObjectFlag::isOutOfFrameOfMomentumData() const {
  return get2ndDigit() >= flagFor2ndDigit_outOfFrameOfMomentumData;
}

const bool Analysis::ObjectFlag::isOutOfMasterRegion() const {
  return get1stDigit() >= flagFor1stDigit_outOfMasterRegion;
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
unsigned int Analysis::ObjectFlag::convertCoboldFlag(const int coboldFlag) const {
  if(coboldFlag < flagForResort_theRegion1) {
    return flagFor3rd2Digit_lowerThanTheRegion;
  } else if(coboldFlag > flagForResort_theRegion2) {
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
