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
void Analysis::ObjectFlag::setResortFlag(const int f0) {
  setNthNumDigit(3, 2, convertCoboldFlag(f0));
}
const bool Analysis::ObjectFlag::isResortFlag(const int f0) const {
  return getNthNumDigit(3, 2) == convertCoboldFlag(f0);
}
const unsigned int Analysis::ObjectFlag::getResortFlag() const {
  return convertToCoboldFlag(getNthNumDigit(3, 2));
}
unsigned int Analysis::ObjectFlag::convertCoboldFlag(const int f0) const {
  if(f0 < flagForCobold_theRegion1) {
    return (const unsigned int) flagFor3rd2Digit_lowerThanTheRegion;
  } else if(f0 > flagForCobold_theRegion2) {
    return (const unsigned int) flagFor3rd2Digit_upperThanTheRegion;
  } else {
    return (const unsigned int) (f0 - flagForCobold_theRegion1 + 1);
  }
}
const unsigned int Analysis::ObjectFlag::convertToCoboldFlag(const unsigned int f0) const {
  assert(f0 >= flagFor3rd2Digit_inTheRegion1);
  assert(f0 <= flagFor3rd2Digit_inTheRegion2);
  return f0 + flagForCobold_mostReliableRegion1 - 1;
}
const bool Analysis::ObjectFlag::isMostReliable() const {
  return flagForCobold_mostReliableRegion1 <= getResortFlag() &&  getResortFlag() <= flagForCobold_mostReliableRegion2;
}
const bool Analysis::ObjectFlag::isMostOrSecondMostReliable() const {
  return flagForCobold_mostReliableRegion1 <= getResortFlag() &&  getResortFlag() <= flagForCobold_secondMostReliableRegion2;
}
const bool Analysis::ObjectFlag::isRisky() const {
  return flagForCobold_riskyRegion1 <= getResortFlag() && getResortFlag() <= flagForCobold_riskyRegion2;
}
