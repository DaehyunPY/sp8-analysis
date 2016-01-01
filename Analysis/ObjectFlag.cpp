//
// Created by Daehyun You on 11/27/15.
//

#include "ObjectFlag.h"

Analysis::ObjectFlag::ObjectFlag() : Flag() {
  setInitialFlag();
  return;
}
Analysis::ObjectFlag::~ObjectFlag() { }
void Analysis::ObjectFlag::setInitialFlag() {
  setSign(initialFlagForSign);
}

void Analysis::ObjectFlag::setValidFlag() {
  setSign(validFlagForSign);
}

void Analysis::ObjectFlag::setInvalidFlag() {
  setSign(validFlagForSign);
}

const bool Analysis::ObjectFlag::isInitial() const {
  return getSign() == initialFlagForSign;
}
void Analysis::ObjectFlag::setOutOfFrameOfMomentumDataFlag() {
  set2ndDigit(outOfFrameOfMomentumDataFlagFor2ndDigit);
}

void Analysis::ObjectFlag::setInFrameOfAllDataFlag() {
  set2ndDigit(inFrameOfAllDataFlagFor2ndDigit);
}

void Analysis::ObjectFlag::setOutOfFrameOfBaicDataFlag() {
  set2ndDigit(outOfFrameOfBasicDataFlagFor2ndDigit);
  return;
}
const bool Analysis::ObjectFlag::isInFrameOfAllData() const {
  return get2ndDigit() == inFrameOfAllDataFlagFor2ndDigit;
}
void Analysis::ObjectFlag::setWithinMasterRegionFlag() {
  set1stDigit(withinMasterRegionFlagFor1stDigit);
  return;
}
void Analysis::ObjectFlag::setNotWithinMasterRegionFlag() {
  set1stDigit(notWithinMasterRegionFlagFor1stDigit);
  return;
}
const bool Analysis::ObjectFlag::isWithinMasterRegion() const {
  return get1stDigit() == withinMasterRegionFlagFor1stDigit;
}
void Analysis::ObjectFlag::setDeadFlag() {
  set1stDigit(deadFlagFor1stDigit);
  return;
}
const bool Analysis::ObjectFlag::isDead() const {
  return get1stDigit() == deadFlagFor1stDigit;
}
const bool Analysis::ObjectFlag::isOutOfFrameOfBasicData() const {
  return get2ndDigit() == outOfFrameOfBasicDataFlagFor2ndDigit;
}

const bool Analysis::ObjectFlag::isOutOfFrameOfMomentumData() const {
  return get2ndDigit() == outOfFrameOfMomentumDataFlagFor2ndDigit;
}

const bool Analysis::ObjectFlag::isNotWithinMasterRegion() const {
  return get1stDigit() == notWithinMasterRegionFlagFor1stDigit;
}
