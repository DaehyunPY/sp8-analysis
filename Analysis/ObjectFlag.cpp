//
// Created by Daehyun You on 11/27/15.
//

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
  if (get1stDigit(initialFlag) == get1stDigit()) {
    set1stDigit(flagFor1stDigit_withinMasterRegion);
  }
}

void Analysis::ObjectFlag::setOutOfMasterRegionFlag() {
  if (get1stDigit(initialFlag) == get1stDigit()) {
    set1stDigit(flagFor1stDigit_outOfMasterRegionButNotDead);
  }
}

void Analysis::ObjectFlag::setDeadFlag() {
  if (get1stDigit() < flagFor1stDigit_dead) {
    set1stDigit(flagFor1stDigit_dead);
  }
}

void Analysis::ObjectFlag::setInFrameOfAllDataFlag() {
  if (get2ndDigit(initialFlag) == get2ndDigit()) {
    set2ndDigit(flagFor2ndDigit_inFrameOfAllData);
  }
}

void Analysis::ObjectFlag::setOutOfFrameOfMomentumDataFlag() {
  if (get2ndDigit() >= get2ndDigit(flagFor2ndDigit_inFrameOfAllData)
      || get2ndDigit() < flagFor2ndDigit_outOfFrameOfMomentumData) {
    set2ndDigit(flagFor2ndDigit_outOfFrameOfMomentumData);
  }
}

void Analysis::ObjectFlag::setOutOfFrameOfBasicDataFlag() {
  if (get2ndDigit() >= get2ndDigit(flagFor2ndDigit_inFrameOfAllData)
      || get2ndDigit() < flagFor2ndDigit_outOfFrameOfBasicDataButNotOutOfFrameOfMomentumData) {
    set2ndDigit(
        flagFor2ndDigit_outOfFrameOfBasicDataButNotOutOfFrameOfMomentumData);
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
  return get2ndDigit() >= flagFor2ndDigit_outOfFrameOfBasicDataButNotOutOfFrameOfMomentumData;
}

const bool Analysis::ObjectFlag::isOutOfFrameOfMomentumData() const {
  return get2ndDigit() >= flagFor2ndDigit_outOfFrameOfMomentumData;
}

const bool Analysis::ObjectFlag::isOutOfMasterRegion() const {
  return get1stDigit() >= flagFor1stDigit_outOfMasterRegionButNotDead;
}
const bool Analysis::ObjectFlag::isWithinMasterRegion() const {
  return get1stDigit() == flagFor1stDigit_withinMasterRegion;
}
